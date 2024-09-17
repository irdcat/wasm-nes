#pragma once

#include <array>
#include <memory>
#include <queue>
#include <functional>

#include "Types.hpp"
#include "ApuRegisters.hpp"
#include "apu/AudioChannel.hpp"

class Apu
{
    public:
        Apu(const std::function<void()>& irqTriggerCallback);

        ~Apu() = default;

        u8 read();

        void write(u8 addr, u8 value);

        void tick();

        std::queue<float> getAudioQueue();

    private:
        std::array<std::unique_ptr<AudioChannel>, 2> channels;
        std::function<void()> irqTriggerCallback;
        std::queue<float> audioQueue;

        ApuRegisters registers;

        u16 hz240counter;
        u8 frameSequencerStep;
        bool periodicIrq;

        static const constexpr u16 HZ_240_COUNTER_THRESHOLD = 14915;
};