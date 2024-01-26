#pragma once

#include "PpuCtrlRegister.hpp"
#include "PpuMaskRegister.hpp"
#include "PpuStatusRegister.hpp"

class PpuRegisters
{
    public:
        PpuRegisters() = default;

        ~PpuRegisters() = default;

        PpuCtrlRegister& getPpuCtrl() { return ppuCtrl; }

        PpuMaskRegister& getPpuMask() { return ppuMask; }

        PpuStatusRegister& getPpuStatus() { return ppuStatus; }

        u8& getOamAddr() { return oamAddr; }

        u8& getOamData() { return oamData; }

        u16& getPpuScroll() { return ppuScroll; }

        u16& getPpuAddr() { return ppuAddr; }

        u8& getPpuData() { return ppuData; }

    private:
        // 0x2000 PPUCTRL - Ppu control register
        PpuCtrlRegister ppuCtrl;
        // 0x2001 PPUMASK - Ppu mask register
        PpuMaskRegister ppuMask;
        // 0x2002 PPUSTATUS - Ppu status register
        PpuStatusRegister ppuStatus;
        // 0x2003 OAMADDR - OAM address port
        u8 oamAddr;
        // 0x2004 OAMDATA - OAM data port
        u8 oamData;
        // 0x2005 PPUSCROLL - Ppu scrolling position register (X Scroll on first write, Y Scroll on second write)
        // TODO: Make a union separating X and Y scroll
        u16 ppuScroll;
        // 0x2006 PPUADDR - Ppu address register (MSB on first write, LSB on second write)
        // TODO: Make a union separating Low and High byte
        u16 ppuAddr;
        // 0x2007 PPUDATA - Ppu data port
        u8 ppuData;
};