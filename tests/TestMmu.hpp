#pragma once

#include <array>

#include "../src/Mmu.hpp"

class TestMmu
    : public Mmu
{
    public:
        TestMmu() = default;

        ~TestMmu() = default;

        u8 readFromMemory(u16 addr) override;

        void writeIntoMemory(u16 addr, u8 value) override;

        void clearMemory();

        unsigned getCycles();

        void resetCycleCounter(unsigned initial);

    private:
        std::array<u8, 0xFFFF> memory;
        unsigned cycles;
};