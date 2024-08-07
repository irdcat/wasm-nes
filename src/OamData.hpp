#pragma once

#include "RegisterBit.hpp"

struct OamData
{
    union
    {
        u8 raw[4];
        struct
        {
            u8 positionY;
            union
            {
                u8 raw;
                RegisterBit<0> bank;
                RegisterBit<1, 7> tileNumber;
            } tileIndexNumber;
            union 
            {
                u8 raw;
                RegisterBit<0, 2> pallete;
                RegisterBit<5> priority;
                RegisterBit<6> horizontalFlip;
                RegisterBit<7> verticalFlip;
            } attributes;
            u8 positionX;
        };
    };
    u8 spriteIndex;
    u16 pattern;
};
