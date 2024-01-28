#pragma once

#include "OamData.hpp"

template <unsigned SpriteCount>
union Oam
{
    u8 raw[SpriteCount * 4];
    OamData sprites[SpriteCount];
};
