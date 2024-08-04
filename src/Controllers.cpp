#include "Controllers.hpp"

Controllers::Controllers()
    : keysLatched()
    , keysInternal()
    , shift()
    , strobeFlag(false)
{
}

void Controllers::updateKeyPressStatus(u8 port, u8 key, bool pressed)
{
    port &= 1;
    key &= 7;

    if(pressed) {
        keysInternal[port] |= 1 << key;
    } else {
        keysInternal[port] &= ~(1 << key);
    }
}

void Controllers::strobe(u8 value)
{
    strobeFlag = value & 1;
    if(strobeFlag) {
        keysLatched[0] = keysInternal[0];
        keysLatched[1] = keysInternal[1];
    }
}

u8 Controllers::read(u8 port)
{
    port &= 1;
    u8 result = keysLatched[port] & 1;
    if (!strobeFlag) {
        keysLatched[port] >>= 1;
        // Unspecified values in 4021 shift register are pulled up
        keysLatched[port] |= 0x80;
    }
    return result;
}
