#include "Apu.hpp"
#include "apu/PulseChannel.hpp"
#include <iostream>

Apu::Apu(const std::function<void()> &irqTriggerCallback)
    : channels({
        std::make_unique<PulseChannel>(0),
        std::make_unique<PulseChannel>(1)
    })
    , irqTriggerCallback(irqTriggerCallback)
    , registers()
    , hz240counter(0)
    , frameSequencerStep(0)
    , periodicIrq(false)
{
}

u8 Apu::read()
{
    u8 result = 0;
    for (auto i = 0; i < channels.size(); i++) {
        result |= channels[i]->isEnabled() << i;
    }
    if (periodicIrq) {
        result |= 0x40;
        periodicIrq = false;
    }
    // TODO: Add DMC interrupt
    return result;
}

void Apu::write(u8 addr, u8 value)
{
    switch (addr) {
        case 0x15:
            for (auto i = 0; i < channels.size(); i++) {
                channels[i]->enable(value & 0x1);
                value >>= 1;
            }
            break;
        case 0x17:
            registers.frameCounterRegister = value;
            break;
        default:
            if(addr > 0x13) {
                break;
            }
            auto channelIndex = addr / 4;
            auto registerIndex = addr % 4;

            // TODO: Remove later
            if (channelIndex >= channels.size()) {
                break;
            }

            channels[channelIndex]->write(registerIndex, value);
            break;
    }
}

void Apu::tick()
{
    const auto& frameSequencerRegister = registers.frameCounterRegister;
    hz240counter += 2;
    if (hz240counter < HZ_240_COUNTER_THRESHOLD) {
        return;
    }

    hz240counter -= HZ_240_COUNTER_THRESHOLD;
    frameSequencerStep++;

    if (frameSequencerStep >= 4 + frameSequencerRegister.fiveStepSequencer) {
        frameSequencerStep = 0;
    }

    if (!frameSequencerRegister.irqDisable && !frameSequencerRegister.fiveStepSequencer && frameSequencerStep == 0) {
        periodicIrq = true;
        irqTriggerCallback();
    }

    bool halfFrame = (frameSequencerStep & 5) == 1;
    bool quarterFrame = frameSequencerStep < 4;

    for (auto i = 0; i < channels.size(); i++) {
        auto channel = channels[i].get();
        
        if (halfFrame) {
            channel->halfFrameTick();
        }
        if (quarterFrame) {
            channel->quarterFrameTick();
        }
    }

    auto pulse1 = channels[0]->tick();
    auto pulse2 = channels[1]->tick();
    // TODO: Replace with actual channel outputs
    auto triangle = 0;
    auto noise = 0;
    auto dmc = 0;

    auto safeDivide = [](float dividend, float divisor, float defaultOnDivByZero) {
        if (divisor == 0.f) {
            return defaultOnDivByZero;
        }
        return dividend / divisor;
    };

    auto pulseSample = safeDivide(95.88f, 100.f + safeDivide(8128.f, pulse1 + pulse2, -100.f), 0.f);
    auto tndSample = safeDivide(159.79f, 100.f + safeDivide(1.0f, triangle / 8227.f + noise / 12241.f + dmc / 22638.f, -100.f), 0.f);

    auto sample = pulseSample + tndSample;
    audioQueue.push(sample);
}

std::queue<float> Apu::getAudioQueue()
{
    return audioQueue;
}
