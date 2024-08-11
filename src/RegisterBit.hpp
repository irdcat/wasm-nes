#pragma once

#include "Types.hpp"

/**
 * Structure used to create convienent to use bitfields, specifically in registers.
 * This structure is meant to be used in the union.
 * Advantage of this compared to bitfields provided by the language 
 * is the fact that instances that share the bits between each other can coexist in a single structure.
 * 
 * See: PpuInternalRegister in PpuRegisters.hpp
 */
template <unsigned BitPosition, unsigned BitCount = 1, typename T = u8>
struct RegisterBit
{
    RegisterBit& operator=(T value) 
    {
        // Keep the values of bits that are not managed by this instance
        data &= ~(mask << BitPosition);
        // Sum it with new value
        data |= (static_cast<T>(value) & mask) << BitPosition;
        
        return *this;
    };

    operator T() const
    {
        // Return bits managed by this instance
        return (data >> BitPosition) & mask;
    };

    RegisterBit& operator++()
    {
        // Use assignment operator to increment the value, 
        // to wrap the managed value based on the specified bit count.
        return *this = *this + 1;
    };

    template <typename T2>
    RegisterBit& operator+=(T2 value)
    {
        data += value;
    }

    T operator++(int)
    {
        auto old = data;
        // Use assignment operator to increment the value, 
        // to wrap the managed value based on the specified bit count.
        *this = *this + 1;
        return old;
    };

    private:
        T data;
        static constexpr const unsigned mask = (1 << BitCount) - 1;
};