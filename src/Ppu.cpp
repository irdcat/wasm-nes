#include "Ppu.hpp"

u8 Ppu::read(u8 index)
{
    // TODO: Simulate open bus behaviour
    if(index == 2)
        return ppuRegisters.getPpuStatus();
    if(index == 4)
        return ppuRegisters.getOamData();
    if(index == 7)
        return ppuRegisters.getPpuData();

    return u8();
}

void Ppu::write(u8 index, u8 data)
{
    // TODO: Simulate open bus behaviour

    // PPUSCROLL and PPUADDR share the common offset toggle latch
    static bool offsetToggleLatch = false;

    if(index == 0) {
        auto& ppuCtrl = ppuRegisters.getPpuCtrl();
        ppuCtrl = data;
    } else if(index == 1) {
        auto& ppuMask = ppuRegisters.getPpuMask();
        ppuMask = data;
    } else if(index == 3) {
        auto& oamAddr = ppuRegisters.getOamAddr();
        oamAddr = data;
    } else if(index == 4) {
        auto& oamData = ppuRegisters.getOamData();
        oamData = data;
    } else if(index == 5) {
        auto& ppuScroll = ppuRegisters.getPpuScroll();
        if(offsetToggleLatch) {
            ppuScroll.y = data;
        } else {
            ppuScroll.x = data;
        }
        offsetToggleLatch = !offsetToggleLatch;
    } else if(index == 6) {
        auto ppuAddress = ppuRegisters.getPpuAddr();
        if(offsetToggleLatch) {
            ppuAddress.low = data;
        } else {
            ppuAddress.high = data;
        }
        offsetToggleLatch = !offsetToggleLatch;
    } else if(index == 7) {
        auto& ppuData = ppuRegisters.getPpuData();
        ppuData = data;
    }
}

void Ppu::tick()
{
    // TODO: PPU clock cycle
}
