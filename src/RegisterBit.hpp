#pragma once

#include "Types.hpp"

template <unsigned BitPosition, unsigned BitCount = 1>
struct RegisterBit
{
    RegisterBit& operator=(u8 value) 
    {
        // Keep the values of bits that are not managed by this instance
        data &= ~(mask << BitPosition);
        // Sum it with new value
        data |= (value & mask) << BitPosition;
        
        return *this;
    };

    operator u8() const
    {
        return (data >> BitPosition) & mask;
    };

    RegisterBit& operator++()
    {
        data++;
        return *this;
    };

    u8 operator++(int)
    {
        auto old = data;
        data++;
        return old;
    };

    private:
        u8 data;
        static constexpr const unsigned mask = (1 << BitCount) - 1;
};