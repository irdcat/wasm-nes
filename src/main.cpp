#include <emscripten.h>
#include <iostream>
#include <string>
#include "Emulator.hpp"

EMSCRIPTEN_KEEPALIVE Emulator emulator;

extern "C"
{
    EMSCRIPTEN_KEEPALIVE void loadRom(const char * filename)
    {
        auto filenameString = std::string(filename);
        emulator.loadRom(filenameString);
    }
}

void mainLoop()
{
    static const constexpr u64 FRAME_PERIOD_MILLIS = 1000 / 60;

    u64 currentTime;
    u64 deltaTime;
    u64 lastTime = 0;
    if(emulator.shouldBeRunning()) {
        currentTime = SDL_GetTicks64();
        deltaTime = currentTime - lastTime;
        emulator.handleInput();
        emulator.update(deltaTime);
        lastTime = currentTime;
    }
}

int main()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL Video" << std::endl;
        return 0;
    }
    emscripten_set_main_loop(mainLoop, 0, 1);
    SDL_Quit();
    return 0;
}