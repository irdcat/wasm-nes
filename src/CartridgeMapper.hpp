#pragma once

#include "Types.hpp"

class CartridgeMapper
{
    public:
        virtual ~CartridgeMapper() = default;

        virtual void write(u16 addr, u8 value) = 0;

        virtual u8 read(u16 addr) = 0;
};