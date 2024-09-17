#include "PulseChannel.hpp"

PulseChannel::PulseChannel(bool secondChannel)
    : AudioChannel()
    , registers()
    , lengthCounter(0)
    , waveCounter(0)
    , envelope(0)
    , envelopeDecay(0)
    , phase(0)
    , level(0)
    , sweepDelay(0)
    , secondChannel(secondChannel)
{
}

void PulseChannel::write(u8 index, u8 value)
{
    switch (index) {
        case 0:
            registers.reg0 = value;
            break;
        case 1:
            registers.reg1 = value;
            break;
        case 2:
            registers.reg2 = value;
            break;
        case 3:
            registers.reg3 = value;
            break;
    }
}

void PulseChannel::quarterFrameTick()
{
    if (envelopeDecay == 0) {
        envelopeDecay = registers.decayRate;
        if (envelope > 0 || registers.decayLoopEnable) {
            envelope = (envelope - 1) & 0xF;
        }
    } else {
        envelopeDecay--;
    }
}

void PulseChannel::halfFrameTick()
{
    if (!registers.lengthCounterDisable && lengthCounter > 0) {
        lengthCounter--;
    }
    if (sweepDelay == 0) {
        sweepDelay = registers.sweepPeriod;
        auto wl = registers.waveLength;
        if (wl >= 8 && registers.sweepEnabled && registers.shiftCount > 0) {
            int sweepedWaveLength = wl >> registers.shiftCount;
            if (registers.negative) {
                wl = wl + (secondChannel ? -sweepedWaveLength : ~sweepedWaveLength);
            } else {
                wl = wl + sweepedWaveLength;
            }
            if (wl < 0x800) {
                registers.waveLength = wl;
            }
        }
    } else {
        sweepDelay--;
    }
}

u8 PulseChannel::tick()
{   
    if (!isEnabled()) {
        return 8;
    }
    
    auto wl = (registers.waveLength) + 1 * 2;
    if (waveCounter == 0) {
        waveCounter = wl;
        return level;
    } else {
        waveCounter--;
    }

    // Waveform generator has 4 different 8-step binary waveforms thus it is composed of 32 bits of data
    // 1. 12.5% duty cycle
    // 2. 25% duty cycle
    // 3. 50% duty cycle
    // 4. 25% duty cycle (negated)
    static const u32 WAVEFORMS = 0xF33C0C04;
    
    // Envelope generator can produce fixed volume or decreasing saw envelope.
    u8 volume = 0;
    if (lengthCounter > 0) {
        volume = registers.decayDisable ? registers.fixedVolume : envelope; 
    }

    // Return previously chosen volume or zero depending on the phase of chosen waveform
    phase = (phase + 1) % 8;
    level = (WAVEFORMS & (1 << (phase + registers.dutyCycle * 8))) ? volume : 0;
    return level;
}
