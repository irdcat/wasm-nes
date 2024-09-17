#pragma once

#include "../../src/core/Cpu.hpp"
#include "../../src/core/Ppu.hpp"
#include "../../src/core/Mmu.hpp"
#include "../../src/core/Cartridge.hpp"
#include "../../src/core/Controllers.hpp"
#include "../../src/core/Apu.hpp"

class SystemUnderTest
{
    public:
        SystemUnderTest();

        ~SystemUnderTest() = default;

        Cpu* getCpu();

        Ppu* getPpu();

        Mmu* getMmu();

        Cartridge* getCartridge();

        Controllers* getControllers();

        Apu* getApu();

    private:
        std::shared_ptr<Apu> apu;
        std::shared_ptr<Controllers> controllers;
        std::shared_ptr<Cartridge> cartridge;
        std::shared_ptr<Ppu> ppu;
        std::shared_ptr<Mmu> mmu;
        std::unique_ptr<Cpu> cpu;
};