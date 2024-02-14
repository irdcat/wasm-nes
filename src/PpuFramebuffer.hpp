#pragma once

#include "Types.hpp"

#include <array>

class PpuFramebuffer
{
    public:
        PpuFramebuffer() = default;

        void setColor(unsigned x, unsigned y, u8 colorIndex);

        u8 getColor(unsigned x, unsigned y);

    private:
        static constexpr const unsigned WIDTH = 256;
        static constexpr const unsigned HEIGHT = 240;

        static constexpr const unsigned BITS_PER_BYTE = 8;
        static constexpr const unsigned BITS_PER_COLOR = 6;
        static constexpr const unsigned SHIFT_OFFSET_THRESHOLD = 2;

        static constexpr const unsigned PIXEL_COUNT = WIDTH * HEIGHT;
        static constexpr const unsigned BUFFER_SIZE = PIXEL_COUNT * BITS_PER_COLOR / BITS_PER_BYTE;

        static constexpr const unsigned COLOR_MASK = (1 << BITS_PER_COLOR) - 1;

        std::array<u8, BUFFER_SIZE> buffer;
};
