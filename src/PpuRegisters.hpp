#pragma once

#include "PpuCtrlRegister.hpp"
#include "PpuMaskRegister.hpp"
#include "PpuStatusRegister.hpp"
#include "PpuScrollRegister.hpp"
#include "PpuAddressRegister.hpp"

class PpuRegisters
{
    public:
        PpuRegisters() = default;

        ~PpuRegisters() = default;

        PpuCtrlRegister& getPpuCtrl() { return ppuCtrl; }

        PpuMaskRegister& getPpuMask() { return ppuMask; }

        PpuStatusRegister& getPpuStatus() { return ppuStatus; }

        u8& getOamAddr() { return oamAddr; }

        PpuScrollRegister& getPpuScroll() { return ppuScroll; }

        PpuAddressRegister& getPpuAddr() { return ppuAddr; }

    private:
        PpuCtrlRegister ppuCtrl;
        PpuMaskRegister ppuMask;
        PpuStatusRegister ppuStatus;
        u8 oamAddr;
        PpuScrollRegister ppuScroll;
        PpuAddressRegister ppuAddr;
};