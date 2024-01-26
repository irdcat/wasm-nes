#pragma once

#include "Types.hpp"
#include "Ppu.hpp"
#include <memory>
#include <array>

class Mmu
{
    public:
        Mmu() = default;

        Mmu(const std::shared_ptr<Ppu>& ppu);

        virtual ~Mmu() = default;

        virtual u8 readFromMemory(u16 addr);

        virtual void writeIntoMemory(u16 addr, u8 value);

        void signalReset(bool signal);

    private:
        std::shared_ptr<Ppu> ppu;
        std::array<u8, 0x800> internalRam;
        bool resetSignalled;

        void tick();

        template <bool IsWrite>
        u8 memoryAccess(u16 addr, u8 value = 0);
};

#include "Mmu.inl"