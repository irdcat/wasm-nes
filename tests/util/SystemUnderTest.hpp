#pragma once

#include "../../src/Cpu.hpp"
#include "../../src/Ppu.hpp"
#include "../../src/Mmu.hpp"
#include "../../src/Cartridge.hpp"
#include "../../src/Framebuffer.hpp"

class SystemUnderTest
{
    public:
        SystemUnderTest();

        ~SystemUnderTest() = default;

        Cpu* getCpu();

        Ppu* getPpu();

        Mmu* getMmu();

        Cartridge* getCartridge();

    private:
        std::shared_ptr<Framebuffer> ppuFramebuffer;
        std::shared_ptr<Cartridge> cartridge;
        std::shared_ptr<Ppu> ppu;
        std::shared_ptr<Mmu> mmu;
        std::unique_ptr<Cpu> cpu;
};