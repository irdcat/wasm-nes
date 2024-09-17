#pragma once

#include "Types.hpp"
#include "Ppu.hpp"
#include "Apu.hpp"
#include "Cartridge.hpp"
#include "Controllers.hpp"
#include <memory>
#include <array>

/**
 * MMU - Memory Mapping Unit.
 * This class represents main bus used by the CPU that connects it with the memory and other peripherials. 
 */
class Mmu
{
    public:
        Mmu();

        Mmu(const std::shared_ptr<Ppu>& ppu,
            const std::shared_ptr<Apu>& apu,
            const std::shared_ptr<Cartridge>& cartridge,
            const std::shared_ptr<Controllers>& controllers);

        virtual ~Mmu() = default;

        virtual u8 readFromMemory(u16 addr);

        virtual void writeIntoMemory(u16 addr, u8 value);

        void signalReset(bool signal);

        unsigned getAndResetTickCounterValue();

    private:
        std::shared_ptr<Ppu> ppu;
        std::shared_ptr<Apu> apu;
        std::shared_ptr<Cartridge> cartridge;
        std::shared_ptr<Controllers> controllers;
        std::array<u8, 0x800> internalRam;
        bool resetSignalled;

        void tick();

        unsigned tickCounter;
};