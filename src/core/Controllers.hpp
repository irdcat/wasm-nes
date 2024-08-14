#pragma once

#include <array>
#include "Types.hpp"

class Controllers
{
    public:
        Controllers();

        ~Controllers() = default;

        void updateKeyPressStatus(u8 port, u8 key, bool pressed);

        void strobe(u8 value);

        u8 read(u8 port);

    private:
        std::array<u8, 2> keysLatched;
        std::array<u8, 2> keysInternal;
        std::array<u8, 2> shift;
        bool strobeFlag;
};