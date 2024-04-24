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
    if(port > 1) {
        port = 1;
    }

    key &= 7;

    if(pressed) {
        keysInternal[port] |= 1 << key;
    } else {
        keysInternal[port] &= ~(1 << key);
    }
}

void Controllers::strobe(u8 value)
{
    strobeFlag = value & 0x1;
    if(strobeFlag) {
        keysLatched[0] = keysInternal[0];
        keysLatched[1] = keysInternal[1];
    }
}

u8 Controllers::read(u8 port)
{
    if(port > 1) {
        port = 1;
    }

    u8 result = 0;
    if(strobeFlag) {
        result = keysLatched[port] & 0x1;
    } else {
        result = keysLatched[port] >> shift[port];
        shift[port] = ++shift[port] & 0x7;
    }
    return result;
}
