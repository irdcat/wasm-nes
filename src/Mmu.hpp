#pragma once

#include "Types.hpp"
#include "Ppu.hpp"
#include "Cartridge.hpp"
#include <memory>
#include <array>

class Mmu
{
    public:
        Mmu();

        Mmu(const std::shared_ptr<Ppu>& ppu, const std::shared_ptr<Cartridge>& cartridge);

        virtual ~Mmu() = default;

        virtual u8 readFromMemory(u16 addr);

        virtual void writeIntoMemory(u16 addr, u8 value);

        void signalReset(bool signal);

    private:
        std::shared_ptr<Ppu> ppu;
        std::shared_ptr<Cartridge> cartridge;
        std::array<u8, 0x800> internalRam;
        bool resetSignalled;

        void tick();
};