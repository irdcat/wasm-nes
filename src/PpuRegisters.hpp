#pragma once

#include "PpuCtrlRegister.hpp"
#include "PpuMaskRegister.hpp"
#include "PpuStatusRegister.hpp"
#include "PpuScrollRegister.hpp"
#include "PpuAddressRegister.hpp"

struct PpuRegisters
{
    PpuCtrlRegister ppuCtrl;
    PpuMaskRegister ppuMask;
    PpuStatusRegister ppuStatus;
    u8 oamAddr;
    PpuScrollRegister ppuScroll;
    PpuAddressRegister ppuAddr;
};