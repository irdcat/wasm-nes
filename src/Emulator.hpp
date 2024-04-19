#pragma once

#include "Cpu.hpp"
#include "Mmu.hpp"
#include "Ppu.hpp"
#include "Cartridge.hpp"
#include "PpuFramebuffer.hpp"
#include "SdlResource.hpp"

class Emulator
{
    public:
        Emulator();

        ~Emulator() = default;

        void reset();

        void loadRom(const std::string& filename);

        void handleInput();

        void update(u32 millisElapsed);

        void render();

        bool shouldBeRunning() const;

    private:
        std::shared_ptr<Cpu> cpu;
        std::shared_ptr<Mmu> mmu;
        std::shared_ptr<Ppu> ppu;
        std::shared_ptr<Cartridge> cartridge;
        std::shared_ptr<PpuFramebuffer> ppuFramebuffer;

        bool shouldRun;

        SdlResource<SDL_Texture> texture;
        SdlResource<SDL_Window> window;
        SdlResource<SDL_Renderer> renderer;

        void updateScreen();

        static constexpr const unsigned DISPLAY_WIDTH = 256;
        static constexpr const unsigned DISPLAY_HEIGHT = 240;
        static constexpr const unsigned PIXEL_SIZE = 2;

        static constexpr const unsigned CPU_CYCLES_PER_SECOND = 1790000;
};