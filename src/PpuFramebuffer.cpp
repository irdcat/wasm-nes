#include "PpuFramebuffer.hpp"

void PpuFramebuffer::setColor(unsigned x, unsigned y, u8 colorIndex)
{
    buffer[x + y * WIDTH] = colorIndex;
}

u8 PpuFramebuffer::getColor(unsigned x, unsigned y) 
{
    return buffer[x + y * WIDTH];
}