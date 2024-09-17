#pragma once

#include "../Types.hpp"

class AudioChannel
{
    public:
        AudioChannel() = default;

        virtual ~AudioChannel() = default;

        virtual void write(u8 index, u8 value) = 0;

        virtual void quarterFrameTick() = 0;

        virtual void halfFrameTick() = 0;

        virtual u8 tick() = 0;

        bool isEnabled();

        void enable(bool enable);
    
    private:
        bool enabled;
};