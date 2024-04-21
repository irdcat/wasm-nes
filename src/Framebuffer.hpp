#pragma once

#include "Types.hpp"

#include <array>

class Framebuffer
{
    public:
        Framebuffer() = default;

        void setColor(unsigned x, unsigned y, u8 colorIndex);

        u8 getColor(unsigned x, unsigned y);

    private:
        static constexpr const unsigned WIDTH = 256;
        static constexpr const unsigned HEIGHT = 240;
        static constexpr const unsigned BUFFER_SIZE = WIDTH * HEIGHT;
        std::array<u8, BUFFER_SIZE> buffer;
};
