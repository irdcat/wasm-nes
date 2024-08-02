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

    EMSCRIPTEN_KEEPALIVE void run()
    {
        if(SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "Failed to initialize SDL Video" << std::endl;
            return;
        }
        u64 currentTime;
        u64 deltaTime;
        u64 lastTime = 0;
        while(emulator.shouldBeRunning())
        {
            currentTime = SDL_GetTicks64();
            deltaTime = currentTime - lastTime;
            emulator.handleEvents();
            emulator.update(deltaTime);
            emulator.render();
            lastTime = currentTime;
            emscripten_sleep(deltaTime > 16 ? 0 : 16 - deltaTime);
        }
        SDL_Quit();
        return;
    }
}