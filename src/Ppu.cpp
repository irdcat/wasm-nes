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
        static bool latch = false;
        u16 scrollData = latch ? data : data << 8;
        auto& ppuScroll = ppuRegisters.getPpuScroll();
        u16 oldScrollData = latch ? ppuScroll & 0xFF00 : ppuScroll & 0xFF;
        ppuScroll = scrollData | oldScrollData;
    } else if(index == 6) {
        static bool latch = false;
        u16 scrollData = latch ? data : data << 8;
        auto& ppuAddr = ppuRegisters.getPpuAddr();
        u16 oldScrollData = latch ? ppuAddr & 0xFF00 : ppuAddr & 0xFF;
        ppuAddr = scrollData | oldScrollData;
    } else if(index == 7) {
        auto& ppuData = ppuRegisters.getPpuData();
        ppuData = data;
    }
}
