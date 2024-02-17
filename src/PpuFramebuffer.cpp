#include "PpuFramebuffer.hpp"

void PpuFramebuffer::setColor(unsigned x, unsigned y, u8 colorIndex)
{
    colorIndex &= COLOR_MASK;
    const auto bitPosition = x * y * BITS_PER_COLOR;
    const auto shiftOffset = bitPosition % BITS_PER_BYTE;
    if(shiftOffset < SHIFT_OFFSET_THRESHOLD) {
        buffer[bitPosition / BITS_PER_BYTE] |= colorIndex << shiftOffset;
    } else {
        buffer[bitPosition / BITS_PER_BYTE] |= colorIndex >> (shiftOffset - SHIFT_OFFSET_THRESHOLD);
        buffer[(bitPosition + BITS_PER_COLOR) / BITS_PER_BYTE] |= colorIndex << (BITS_PER_BYTE + SHIFT_OFFSET_THRESHOLD - shiftOffset);
    }
}

u8 PpuFramebuffer::getColor(unsigned x, unsigned y) 
{
    const auto bitPosition = x * y * BITS_PER_COLOR;
    const auto shiftOffset = bitPosition % BITS_PER_BYTE;
    auto result = 0;
    if(shiftOffset < SHIFT_OFFSET_THRESHOLD) {
        result = buffer[bitPosition / BITS_PER_BYTE] >> shiftOffset; 
    } else {
        result = buffer[bitPosition / BITS_PER_BYTE] << shiftOffset - SHIFT_OFFSET_THRESHOLD
            | buffer[(bitPosition + BITS_PER_COLOR) / BITS_PER_BYTE] >> BITS_PER_BYTE + SHIFT_OFFSET_THRESHOLD - shiftOffset;
    }
    return result & COLOR_MASK;
}