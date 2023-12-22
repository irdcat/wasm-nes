#pragma once

#include "Types.hpp"

class Mmu
{
    public:
        Mmu() = default;

        virtual ~Mmu() = default;

        virtual u8 readFromMemory(u16 addr);

        virtual void writeIntoMemory(u16 addr, u8 value);
};