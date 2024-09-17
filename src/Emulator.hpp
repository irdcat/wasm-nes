#pragma once

#include "core/Cpu.hpp"
#include "core/Mmu.hpp"
#include "core/Ppu.hpp"
#include "core/Cartridge.hpp"
#include "core/Controllers.hpp"
#include "core/Apu.hpp"
#include "SdlResource.hpp"

class Emulator
{
    public:
        Emulator();

        ~Emulator() = default;

        void reset();

        void loadRom(const std::string& filename);

        void handleEvents();

        void update(u32 millisElapsed);

        void render();

        bool shouldBeRunning() const;

    private:
        std::shared_ptr<Cpu> cpu;
        std::shared_ptr<Mmu> mmu;
        std::shared_ptr<Ppu> ppu;
        std::shared_ptr<Cartridge> cartridge;
        std::shared_ptr<Controllers> controllers;
        std::shared_ptr<Apu> apu;

        bool shouldRun;

        SdlResource<SDL_Texture> texture;
        SdlResource<SDL_Window> window;
        SdlResource<SDL_Renderer> renderer;

        void updateScreen();
        u8 sdlKeyToNesIndex(SDL_Scancode scancode);

        void handleInputEvent(const SDL_Event& e);
        void handleWindowEvent(const SDL_WindowEvent& e);

        std::array<u32, 64> colors;
        SDL_Rect currentViewport;

        static constexpr const unsigned CPU_CYCLES_PER_SECOND = 1790000;
};