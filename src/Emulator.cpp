#include "Emulator.hpp"
#include "SdlColor.hpp"
#include <emscripten.h>
#include <iostream>

Emulator::Emulator()
    : shouldRun(false)
{
    cartridge = std::make_shared<Cartridge>();
    ppuFramebuffer = std::make_shared<PpuFramebuffer>();
    
    auto nmiTriggerCallback = [this](){
        cpu->interrupt(InterruptType::NMI);
    };

    auto vblankInterruptCallback = [this](){
        updateScreen();
    };

    ppu = std::make_shared<Ppu>(cartridge, ppuFramebuffer, nmiTriggerCallback, vblankInterruptCallback);
    mmu = std::make_shared<Mmu>(ppu, cartridge);
    cpu = std::make_shared<Cpu>(mmu);

    window = make_sdl_resource(
        SDL_CreateWindow, 
        SDL_DestroyWindow, 
        "Wasm-NES",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * PIXEL_SIZE,
        DISPLAY_HEIGHT * PIXEL_SIZE,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    if(!window) {
        std::cerr << "Failed to initialize SDL Window" << std::endl;
        return;
    }

    renderer = make_sdl_resource(
        SDL_CreateRenderer,
        SDL_DestroyRenderer,
        window.get(),
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(!renderer) {
        std::cerr << "Failed to initialize SDL Renderer" << std::endl;
        return;
    }

    SDL_RenderSetLogicalSize(renderer.get(), DISPLAY_WIDTH * PIXEL_SIZE, DISPLAY_HEIGHT * PIXEL_SIZE);

    texture = make_sdl_resource(
        SDL_CreateTexture,
        SDL_DestroyTexture,
        renderer.get(),
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT);

    if(!texture) {
        std::cerr << "Failed to initialize SDL Texture" << std::endl;
        return;
    }
}

void Emulator::reset()
{
    cpu->reset();
}

void Emulator::loadRom(const std::string &filename)
{
    auto file = std::ifstream(filename, std::ios::binary);
    shouldRun = cartridge->loadFromFile(std::move(file));
    reset();
}

void Emulator::handleInput()
{
    static SDL_Event event;
    while(SDL_PollEvent(&event) != 0) {
        if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            auto key = event.key.keysym.scancode;
            auto pressed = event.type == SDL_KEYDOWN;
            // TODO: Handling key events
        } else if(event.type == SDL_QUIT) {
            shouldRun = false;
        }
    }
}

void Emulator::update(u32 millisElapsed)
{
    unsigned cyclesToExecute = CPU_CYCLES_PER_SECOND * millisElapsed / 1000;
    while(cyclesToExecute--) {
        cpu->step();
    }
}

void Emulator::render()
{
    auto viewport = SDL_Rect{ 0, 0, DISPLAY_WIDTH * PIXEL_SIZE, DISPLAY_HEIGHT * PIXEL_SIZE };
    SDL_RenderClear(renderer.get());
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &viewport);
    SDL_RenderPresent(renderer.get());
}

bool Emulator::shouldBeRunning() const
{
    return shouldRun && window && renderer && texture;
}

void Emulator::updateScreen()
{
    u32* renderBuffer = nullptr;
    u32 format = 0;
    int pitch = 0;
    SDL_QueryTexture(texture.get(), &format, nullptr, nullptr, nullptr);
    auto pixelFormat = make_sdl_resource(SDL_AllocFormat, SDL_FreeFormat, format);
    SDL_LockTexture(texture.get(), nullptr, reinterpret_cast<void**>(&renderBuffer), &pitch);
    for(auto x = 0; x < DISPLAY_WIDTH; x++) {
        for(auto y = 0; y < DISPLAY_HEIGHT; y++) {
            auto nesColorIndex = ppuFramebuffer->getColor(x, y);
            auto color = SdlColor::fromNesColorIndex(nesColorIndex).mapToColor(pixelFormat.get());
            const unsigned renderBufferIndex = y * (pitch / sizeof(int)) + x;
            renderBuffer[renderBufferIndex] = color; 
        }
    }
    SDL_UnlockTexture(texture.get());
}
