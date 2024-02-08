#include "PpuFramebuffer.hpp"

void PpuFramebuffer::setColor(unsigned x, unsigned y, u8 colorIndex)
{
    colorIndex &= COLOR_MASK;
    const auto bitPosition = x * y * BITS_PER_COLOR;
    const auto shiftOffset = bitPosition % BITS_PER_BYTE;
    auto& dataLower = buffer[bitPosition / BITS_PER_BYTE];
    auto& dataUpper = buffer[(bitPosition + BITS_PER_COLOR) / BITS_PER_BYTE];
    if(shiftOffset < SHIFT_OFFSET_THRESHOLD) {
        dataLower |= colorIndex << shiftOffset;
    } else {
        dataLower |= colorIndex >> (shiftOffset - SHIFT_OFFSET_THRESHOLD);
        dataUpper |= colorIndex << (BITS_PER_BYTE + SHIFT_OFFSET_THRESHOLD - shiftOffset);
    }
}

u8 PpuFramebuffer::getColor(unsigned x, unsigned y) 
{
    const auto bitPosition = x * y * BITS_PER_COLOR;
    const auto shiftOffset = bitPosition % BITS_PER_BYTE;
    auto& dataLower = buffer[bitPosition / BITS_PER_BYTE];
    auto& dataUpper = buffer[(bitPosition + BITS_PER_COLOR) / BITS_PER_BYTE];
    auto result = 0;
    if(shiftOffset < SHIFT_OFFSET_THRESHOLD) {
        result = dataLower >> shiftOffset; 
    } else {
        result = dataLower << shiftOffset - SHIFT_OFFSET_THRESHOLD
            | dataUpper >> BITS_PER_BYTE + SHIFT_OFFSET_THRESHOLD - shiftOffset;
    }
    return result & COLOR_MASK;
}