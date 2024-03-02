#pragma once

#include "Types.hpp"

template <unsigned BitPosition, unsigned BitCount = 1, typename T = u8>
struct RegisterBit
{
    RegisterBit& operator=(T value) 
    {
        // Keep the values of bits that are not managed by this instance
        data &= ~(mask << BitPosition);
        // Sum it with new value
        data |= (value & mask) << BitPosition;
        
        return *this;
    };

    operator T() const
    {
        return (data >> BitPosition) & mask;
    };

    RegisterBit& operator++()
    {
        data++;
        return *this;
    };

    template <typename T2>
    RegisterBit& operator+=(T2 value)
    {
        data += value;
    }

    T operator++(int)
    {
        auto old = data;
        data++;
        return old;
    };

    private:
        T data;
        static constexpr const unsigned mask = (1 << BitCount) - 1;
};