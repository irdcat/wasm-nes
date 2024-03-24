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
        updatePatternTables();
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
        786, // TODO: Change later to DISPLAY_WIDTH * PIXEL_SIZE
        518, // TODO: Change later to DISPLAY_WIDTH * PIXEL_SIZE
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

    SDL_RenderSetLogicalSize(renderer.get(), 786, 518);

    screenTexture = make_sdl_resource(
        SDL_CreateTexture,
        SDL_DestroyTexture,
        renderer.get(),
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT);

    if(!screenTexture) {
        std::cerr << "Failed to initialize SDL Texture" << std::endl;
        return;
    }

    patternTableZeroTexture = make_sdl_resource(
        SDL_CreateTexture,
        SDL_DestroyTexture,
        renderer.get(),
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        128,
        128
    );

    patternTableOneTexture = make_sdl_resource(
        SDL_CreateTexture,
        SDL_DestroyTexture,
        renderer.get(),
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        128,
        128
    );
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
    auto screenDestinationRect = SDL_Rect{ 6, 18, DISPLAY_WIDTH * PIXEL_SIZE, DISPLAY_HEIGHT * PIXEL_SIZE };
    auto patternTableZeroDestinationRect = SDL_Rect{ 524, 2, 128 * PIXEL_SIZE, 128 * PIXEL_SIZE };
    auto patternTableOneDestinationRect = SDL_Rect{ 524, 260, 128 * PIXEL_SIZE, 128 * PIXEL_SIZE };

    auto rendererRect = SDL_Rect{ 0, 0, 786, 584 };

    SDL_RenderClear(renderer.get());
    SDL_SetRenderDrawColor(renderer.get(), 20, 20, 130, 255);
    SDL_RenderFillRect(renderer.get(), &rendererRect);
    SDL_RenderCopy(renderer.get(), screenTexture.get(), nullptr, &screenDestinationRect);
    SDL_RenderCopy(renderer.get(), patternTableZeroTexture.get(), nullptr, &patternTableZeroDestinationRect);
    SDL_RenderCopy(renderer.get(), patternTableOneTexture.get(), nullptr, &patternTableOneDestinationRect);
    SDL_RenderPresent(renderer.get());
}

bool Emulator::shouldBeRunning() const
{
    return shouldRun && window && renderer && screenTexture;
}

void Emulator::updateScreen()
{
    u32* renderBuffer = nullptr;
    u32 format = 0;
    int pitch = 0;
    SDL_QueryTexture(screenTexture.get(), &format, nullptr, nullptr, nullptr);
    auto pixelFormat = make_sdl_resource(SDL_AllocFormat, SDL_FreeFormat, format);
    SDL_LockTexture(screenTexture.get(), nullptr, reinterpret_cast<void**>(&renderBuffer), &pitch);
    for(auto x = 0; x < DISPLAY_WIDTH; x++) {
        for(auto y = 0; y < DISPLAY_HEIGHT; y++) {
            auto nesColorIndex = ppuFramebuffer->getColor(x, y);
            auto color = SdlColor::fromNesColorIndex(nesColorIndex).mapToColor(pixelFormat.get());
            const unsigned renderBufferIndex = y * (pitch / sizeof(u32)) + x;
            renderBuffer[renderBufferIndex] = color; 
        }
    }
    SDL_UnlockTexture(screenTexture.get());
}

void Emulator::updatePatternTables()
{
    auto& patternTables = cartridge->getChrRom();
    for(unsigned patternTableId = 0; patternTableId < 2; patternTableId++) {
        u32* renderBuffer = nullptr;
        u32 format = 0;
        int pitch = 0;
        auto patternTableTexture = patternTableId > 0 ? patternTableOneTexture.get() : patternTableZeroTexture.get();
        SDL_QueryTexture(patternTableTexture, &format, nullptr, nullptr, nullptr);
        auto pixelFormat = make_sdl_resource(SDL_AllocFormat, SDL_FreeFormat, format);
        SDL_LockTexture(patternTableTexture, nullptr, reinterpret_cast<void**>(&renderBuffer), &pitch);
        for(unsigned verticalTileIndex = 0; verticalTileIndex < 16; verticalTileIndex++) {
            for(unsigned horizontalTileIndex = 0; horizontalTileIndex < 16; horizontalTileIndex++) {
                u16 offset = verticalTileIndex * 256 + horizontalTileIndex * 16;
                for(unsigned row = 0; row < 8; row++) {
                    u8 lsb = patternTables[patternTableId * 0x1000 + offset + row];
                    u8 msb = patternTables[patternTableId * 0x1000 + offset + row + 8];
                    for(unsigned col = 0; col < 8; col++) {
                        u8 pixel = (lsb & 1) + (msb & 1);
                        lsb >>= 1;
                        msb >>= 1;
                        unsigned posX = horizontalTileIndex * 8 + (7 - col);
                        unsigned posY = verticalTileIndex * 8 + row;
                        auto sdlPixel = SdlColor::fromNesColorIndex(ppu->getColorFromPalette(1, pixel)).mapToColor(pixelFormat.get());
                        renderBuffer[posY * (pitch / sizeof(u32)) + posX] = sdlPixel;
                    }
                }
            }
        }
        SDL_UnlockTexture(patternTableTexture);
    }
}
