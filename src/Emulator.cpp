#include "Emulator.hpp"
#include "SdlColor.hpp"
#include <emscripten.h>
#include <iostream>

Emulator::Emulator()
    : shouldRun(false)
    , initialized(false)
{
    ppuFramebuffer = std::make_shared<PpuFramebuffer>();
    cartridge = std::make_shared<Cartridge>();
    ppu = std::make_shared<Ppu>(cpu, cartridge, ppuFramebuffer);
    mmu = std::make_shared<Mmu>(ppu, cartridge);
    cpu = std::make_shared<Cpu>(mmu);
}

void Emulator::reset()
{
    cpu->reset();
}

void Emulator::loadRom(const std::string &filename)
{
    auto file = std::ifstream(filename, std::ios::binary);
    shouldRun = init(!initialized) && cartridge->loadFromFile(std::move(file));
    reset();
    run();
}

bool Emulator::init(bool doInitialization)
{
    if(!doInitialization) {
        return initialized;
    }

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        return false;
    }
    
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
        return false;
    }

    renderer = make_sdl_resource(
        SDL_CreateRenderer,
        SDL_DestroyRenderer,
        window.get(),
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(!renderer) {
        return false;
    }

    SDL_RenderSetLogicalSize(renderer.get(), DISPLAY_WIDTH, DISPLAY_HEIGHT);

    texture = make_sdl_resource(
        SDL_CreateTexture,
        SDL_DestroyTexture,
        renderer.get(),
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT);

    if(!texture) {
        return false;
    }

    initialized = true;
    return initialized;
}

void Emulator::run()
{
    u64 currentTime = 0;
    u64 lastTime = 0;
    u64 deltaTime = 0;
    while(shouldRun) {
        currentTime = SDL_GetTicks64();
        deltaTime = currentTime - lastTime;

        handleInput();
        update(deltaTime);
        render();

        emscripten_sleep(16 - currentTime);
        lastTime = currentTime;
    }
}

void Emulator::handleInput()
{
    static SDL_Event event;
    while(SDL_PollEvent(&event) != 0) {
        if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            handleKeyEvent(event);
        }
    }
}

void Emulator::update(u32 millisElapsed)
{
    unsigned cyclesToExecute = CPU_CYCLES_PER_SECOND * millisElapsed / 1000;
    while(cyclesToExecute--) {
        cpu->step();
        if(ppu->isInVblank()) {
            updateScreen();
        }
    }
}

void Emulator::render()
{
    SDL_RenderClear(renderer.get());
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
    SDL_RenderPresent(renderer.get());
}

void Emulator::handleKeyEvent(const SDL_Event &event)
{
    auto key = event.key.keysym.scancode;
    auto pressed = event.type == SDL_KEYDOWN;
    // TODO: Handle key events
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
            const unsigned renderBufferIndex = y * (pitch / sizeof(u32)) + x;
            renderBuffer[renderBufferIndex] = color; 
        }
    }
    SDL_UnlockTexture(texture.get());
}