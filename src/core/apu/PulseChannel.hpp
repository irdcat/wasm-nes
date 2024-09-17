#pragma once

#include "AudioChannel.hpp"
#include "AudioChannelRegisters.hpp"

class PulseChannel : public AudioChannel
{
    public:
        PulseChannel(bool secondChannel);

        ~PulseChannel() = default;

        void write(u8 index, u8 value) override;

        void quarterFrameTick() override;

        void halfFrameTick() override;

        u8 tick() override;

    private:
        PulseChannelRegisters registers;
        u8 lengthCounter;
        u8 waveCounter;
        u8 envelope;
        u8 envelopeDecay;
        u8 phase;
        u8 level;
        u8 sweepDelay;
        bool secondChannel;
};