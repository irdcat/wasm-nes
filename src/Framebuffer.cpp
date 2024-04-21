#include "Framebuffer.hpp"

void Framebuffer::setColor(unsigned x, unsigned y, u8 colorIndex)
{
    buffer[x + y * WIDTH] = colorIndex;
}

u8 Framebuffer::getColor(unsigned x, unsigned y) 
{
    return buffer[x + y * WIDTH];
}