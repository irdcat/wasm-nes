#pragma once

#include "Types.hpp"

class Mmu
{
    public:
        Mmu() = default;

        ~Mmu() = default;

        u8 readFromMemory(u16 addr);

        void writeIntoMemory(u16 addr, u8 value);
};