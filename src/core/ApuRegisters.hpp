#pragma once

#include "RegisterBit.hpp"

union PulseChannelRegisters
{
    // 4000, 4004
    RegisterBit<0, 8, u32> reg0;
    RegisterBit<0, 4, u32> fixedVolume;
    RegisterBit<0, 4, u32> decayRate;
    RegisterBit<4, 1, u32> decayDisable;
    RegisterBit<5, 1, u32> decayLoopEnable;
    RegisterBit<5, 1, u32> lengthCounterDisable;
    RegisterBit<6, 2, u32> dutyCycle;

    // 4001, 4005
    RegisterBit<8, 8, u32> reg1;
    RegisterBit<8, 3, u32> shiftCount;
    RegisterBit<11, 1, u32> negative;
    RegisterBit<12, 3, u32> sweepPeriod;
    RegisterBit<15, 1, u32> sweepEnabled;

    // 4002, 4006
    RegisterBit<16, 8, u32> reg2;
    RegisterBit<16, 8, u32> timerLow;

    // 4003, 4007
    RegisterBit<24, 8, u32> reg3;
    RegisterBit<24, 3, u32> timerHigh;
    RegisterBit<27, 5, u32> lengthCounterLoad;
};

union TriangleChannelRegisters
{
    // 4008
    RegisterBit<0, 8, u32> reg0;
    RegisterBit<0, 7, u32> linearCounterReloadValue;
    RegisterBit<7, 1, u32> linearCounterControl;

    // 400A
    RegisterBit<8, 8, u32> reg1;
    RegisterBit<8, 8, u32> timerLow;

    // 400B
    RegisterBit<16, 8, u32> reg2;
    RegisterBit<16, 3, u32> timerHigh;
    RegisterBit<19, 5, u32> lengthCounterLoad;
};

union NoiseChannelRegisters
{
    // 400C
    RegisterBit<0, 8, u32> reg0;
    RegisterBit<0, 4, u32> fixedVolume;
    RegisterBit<0, 4, u32> decayRate;
    RegisterBit<4, 1, u32> decayDisable;
    RegisterBit<5, 1, u32> decayLoopEnable;
    RegisterBit<5, 1, u32> lengthCounterDisable;

    // 400E
    RegisterBit<8, 8, u32> reg1;
    RegisterBit<8, 4, u32> noisePeriod;
    RegisterBit<15, 1, u32> noiseLoop;

    // 400F
    RegisterBit<16, 8, u32> reg2;
    RegisterBit<19, 5, u32> lengthCounterLoad;
};

union DmcChannelRegisters
{
    // 4010
    RegisterBit<0, 8, u32> reg0;
    RegisterBit<0, 4, u32> frequencyIndex;
    RegisterBit<6, 1, u32> loopSample;
    RegisterBit<7, 1, u32> irqEnable;

    // 4011
    RegisterBit<8, 8, u32> reg1;
    RegisterBit<8, 7, u32> directLoad;

    // 4012
    RegisterBit<16, 8, u32> reg2;
    RegisterBit<16, 8, u32> sampleAddress;

    // 4013
    RegisterBit<24, 8, u32> reg3;
    RegisterBit<24, 8, u32> sampleLength;
};