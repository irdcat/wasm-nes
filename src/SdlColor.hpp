#pragma once

#include "Types.hpp"
#include <SDL2/SDL.h>

struct SdlColor
{
    u8 red;
    u8 green;
    u8 blue;

    static SdlColor fromNesColorIndex(u8 nesColorIndex);

    u32 mapToColor(const SDL_PixelFormat* format);
};