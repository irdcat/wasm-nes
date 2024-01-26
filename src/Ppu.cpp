#include "Ppu.hpp"

u8 Ppu::read(u8 index)
{
    return access<0>(index);
}

void Ppu::write(u8 index, u8 data)
{
    access<1>(index, data);
}

void Ppu::tick()
{
    if(openBusDecayTimer > 0) {
        openBusDecayTimer--;
        if(openBusDecayTimer == 0) {
            openBusContents = 0;
        }
    }
}
