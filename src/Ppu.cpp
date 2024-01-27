#include "Ppu.hpp"

Ppu::Ppu(const std::shared_ptr<Cpu>& cpu)
    : cpuWeak(cpu)
{
    openBusDecayTimer = 0;
    openBusContents = 0;
    vramReadBuffer = 0;
    scanline = -1;
    renderingPositionX = 0;
}

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
    auto& ppuStatus = registers.getPpuStatus();

    if(openBusDecayTimer > 0) {
        openBusDecayTimer--;
        if(openBusDecayTimer == 0) {
            openBusContents = 0;
        }
    }

    renderingPositionX++;
    if(renderingPositionX >= 341) {
        
        renderingPositionX = 0;
        scanline++;

        if(scanline == 241) {
            ppuStatus.inVBlank = 1;
            ppuStatus.spriteZeroHit = 0;
            auto cpu = cpuWeak.lock();
            cpu->interrupt(InterruptType::NMI);
        }

        if(scanline == 262) {
            scanline = 0;
            ppuStatus.spriteZeroHit = 0;
            ppuStatus.inVBlank = 0;
        }
    }
}
