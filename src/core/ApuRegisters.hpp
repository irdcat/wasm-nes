#pragma once

#include "RegisterBit.hpp"

union FrameCounterRegister
{
    RegisterBit<6> irqDisable;
    RegisterBit<7> fiveStepSequencer;
    u8 raw;

    operator u8&() { return raw; }

    FrameCounterRegister& operator=(u8 value) { raw = value; return *this; }
};

struct ApuRegisters
{
    FrameCounterRegister frameCounterRegister;
};