#include "Emulator.hpp"
#include <emscripten.h>
#include <iostream>

Emulator::Emulator()
    : shouldRun(false)
{
    auto irqTriggerCallback = [this]() {
        cpu->interrupt(InterruptType::IRQ);
    };

    apu = std::make_shared<Apu>(irqTriggerCallback);
    controllers = std::make_shared<Controllers>();
    cartridge = std::make_shared<Cartridge>();
    
    auto nmiTriggerCallback = [this](){
        cpu->interrupt(InterruptType::NMI);
    };

    auto vblankInterruptCallback = [this](){
        updateScreen();
    };

    ppu = std::make_shared<Ppu>(cartridge, nmiTriggerCallback, vblankInterruptCallback);
    mmu = std::make_shared<Mmu>(ppu, apu, cartridge, controllers);
    cpu = std::make_shared<Cpu>(mmu);

    window = make_sdl_resource(SDL_CreateWindow, SDL_DestroyWindow, "Wasm-NES",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        Ppu::SCREEN_WIDTH, Ppu::SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    renderer = make_sdl_resource(SDL_CreateRenderer, SDL_DestroyRenderer, window.get(),
        -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(renderer.get(), Ppu::SCREEN_WIDTH, Ppu::SCREEN_HEIGHT);
    currentViewport = SDL_Rect { 0, 0, Ppu::SCREEN_WIDTH, Ppu::SCREEN_HEIGHT };

    texture = make_sdl_resource(SDL_CreateTexture, SDL_DestroyTexture, renderer.get(),
        SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, Ppu::SCREEN_WIDTH, Ppu::SCREEN_HEIGHT);

    u32 format = 0;
    SDL_QueryTexture(texture.get(), &format, nullptr, nullptr, nullptr);
    auto pixelFormat = make_sdl_resource(SDL_AllocFormat, SDL_FreeFormat, format);

    colors = {
        SDL_MapRGB(pixelFormat.get(), 0x66, 0x66, 0x66), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x2A, 0x88), 
        SDL_MapRGB(pixelFormat.get(), 0x14, 0x12, 0xA7), 
        SDL_MapRGB(pixelFormat.get(), 0x3B, 0x00, 0xA4),
        SDL_MapRGB(pixelFormat.get(), 0x5C, 0x00, 0x7E), 
        SDL_MapRGB(pixelFormat.get(), 0x6E, 0x00, 0x40), 
        SDL_MapRGB(pixelFormat.get(), 0x6C, 0x06, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x56, 0x1D, 0x00),
        SDL_MapRGB(pixelFormat.get(), 0x33, 0x35, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x0B, 0x48, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x52, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x4F, 0x08),
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x40, 0x4D), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x00, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x00, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x00, 0x00),
        SDL_MapRGB(pixelFormat.get(), 0xAD, 0xAD, 0xAD), 
        SDL_MapRGB(pixelFormat.get(), 0x15, 0x5F, 0xD9), 
        SDL_MapRGB(pixelFormat.get(), 0x42, 0x40, 0xFF), 
        SDL_MapRGB(pixelFormat.get(), 0x75, 0x27, 0xFE),
        SDL_MapRGB(pixelFormat.get(), 0xA0, 0x1A, 0xCC), 
        SDL_MapRGB(pixelFormat.get(), 0xB7, 0x1E, 0x7B), 
        SDL_MapRGB(pixelFormat.get(), 0xB5, 0x31, 0x20), 
        SDL_MapRGB(pixelFormat.get(), 0x99, 0x3E, 0x00),
        SDL_MapRGB(pixelFormat.get(), 0x6B, 0x6D, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x38, 0x87, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x0C, 0x93, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x8F, 0x32),
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x7C, 0x8D), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x00, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x00, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x00, 0x00),
        SDL_MapRGB(pixelFormat.get(), 0xFF, 0xFE, 0xFF), 
        SDL_MapRGB(pixelFormat.get(), 0x64, 0xB0, 0xFF), 
        SDL_MapRGB(pixelFormat.get(), 0x92, 0x90, 0xFF), 
        SDL_MapRGB(pixelFormat.get(), 0xC6, 0x76, 0xFF),
        SDL_MapRGB(pixelFormat.get(), 0xF3, 0x6A, 0xFF), 
        SDL_MapRGB(pixelFormat.get(), 0xFE, 0x6E, 0xCC), 
        SDL_MapRGB(pixelFormat.get(), 0xFE, 0x81, 0x70), 
        SDL_MapRGB(pixelFormat.get(), 0xEA, 0x9E, 0x22),
        SDL_MapRGB(pixelFormat.get(), 0xBC, 0xBE, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x88, 0xD8, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x5C, 0xE4, 0x30), 
        SDL_MapRGB(pixelFormat.get(), 0x45, 0xE0, 0x82),
        SDL_MapRGB(pixelFormat.get(), 0x48, 0xCD, 0xDE), 
        SDL_MapRGB(pixelFormat.get(), 0x4F, 0x4F, 0x4F), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x00, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x00, 0x00),
        SDL_MapRGB(pixelFormat.get(), 0xFF, 0xFE, 0xFF), 
        SDL_MapRGB(pixelFormat.get(), 0xC0, 0xDF, 0xFF), 
        SDL_MapRGB(pixelFormat.get(), 0xD3, 0xD2, 0xFF), 
        SDL_MapRGB(pixelFormat.get(), 0xE8, 0xC8, 0xFF),
        SDL_MapRGB(pixelFormat.get(), 0xFB, 0xC2, 0xFF), 
        SDL_MapRGB(pixelFormat.get(), 0xFE, 0xC4, 0xEA), 
        SDL_MapRGB(pixelFormat.get(), 0xFE, 0xCC, 0xC5), 
        SDL_MapRGB(pixelFormat.get(), 0xF7, 0xD8, 0xA5),
        SDL_MapRGB(pixelFormat.get(), 0xE4, 0xE5, 0x94), 
        SDL_MapRGB(pixelFormat.get(), 0xCF, 0xEF, 0x96), 
        SDL_MapRGB(pixelFormat.get(), 0xBD, 0xF4, 0xAB), 
        SDL_MapRGB(pixelFormat.get(), 0xB3, 0xF3, 0xCC),
        SDL_MapRGB(pixelFormat.get(), 0xB5, 0xEB, 0xF2), 
        SDL_MapRGB(pixelFormat.get(), 0xB8, 0xB8, 0xB8), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x00, 0x00), 
        SDL_MapRGB(pixelFormat.get(), 0x00, 0x00, 0x00)
    };
}

void Emulator::reset()
{
    cpu->reset();
    ppu->reset();
}

void Emulator::loadRom(const std::string &filename)
{
    auto file = std::ifstream(filename, std::ios::binary);
    cartridge->loadFromFile(std::move(file));
    reset();
}

void Emulator::handleEvents()
{
    static SDL_Event event;
    while(SDL_PollEvent(&event) != 0) {
        if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            handleInputEvent(event);
        } else if(event.type == SDL_WINDOWEVENT) {
            handleWindowEvent(event.window);
        } else if(event.type == SDL_QUIT) {
            shouldRun = false;
        }
    }
}

void Emulator::handleInputEvent(const SDL_Event& event)
{
    auto key = event.key.keysym.scancode;
    auto pressed = event.type == SDL_KEYDOWN;
    auto nesIndex = sdlKeyToNesIndex(key);
    if(nesIndex != 0xFF) {
        controllers->updateKeyPressStatus(0, nesIndex, pressed);
    }
}

void Emulator::handleWindowEvent(const SDL_WindowEvent& windowEvent)
{
    if(windowEvent.event == SDL_WINDOWEVENT_RESIZED) {
        auto newWidth = windowEvent.data1;
        auto newHeight = windowEvent.data2;
        std::cout << "Wievport adjusted (W: " << newWidth << ", H: " << newHeight << ")" << std::endl;
        currentViewport = SDL_Rect { 0, 0, newWidth, newHeight };
        SDL_RenderSetLogicalSize(renderer.get(), newWidth, newHeight);
    }
}

void Emulator::update(u32 millisElapsed)
{
    static constexpr unsigned const CYCLES_PER_MILLISECOND = CPU_CYCLES_PER_SECOND / 1000;
    unsigned cyclesToExecute = CYCLES_PER_MILLISECOND * millisElapsed;
    unsigned cyclesExecuted = 0;
    while(cyclesExecuted < cyclesToExecute) {
        cyclesExecuted += cpu->step();
    }
}

void Emulator::render()
{
    SDL_RenderClear(renderer.get());
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &currentViewport);
    SDL_RenderPresent(renderer.get());
}

bool Emulator::shouldBeRunning() const
{
    return window && renderer && texture;
}

void Emulator::updateScreen()
{
    u32* renderBuffer = nullptr;
    SDL_LockTexture(texture.get(), nullptr, reinterpret_cast<void**>(&renderBuffer), nullptr);
    const auto framebuffer = ppu->getFramebuffer();
    for(auto i = 0; i < framebuffer.size(); i++) {
        renderBuffer[i] = colors[framebuffer[i]];
    }
    SDL_UnlockTexture(texture.get());
}

u8 Emulator::sdlKeyToNesIndex(SDL_Scancode scancode)
{
    u8 result = 0xFF;
    std::array<SDL_Scancode, 8> scancodes = {
        SDL_SCANCODE_Z,
        SDL_SCANCODE_X,
        SDL_SCANCODE_SPACE,
        SDL_SCANCODE_RETURN,
        SDL_SCANCODE_UP,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_RIGHT
    };
    for(auto i = 0; i < scancodes.size(); i++) {
        if(scancode == scancodes[i]) {
            return i;
        }
    }
    return result;
}
