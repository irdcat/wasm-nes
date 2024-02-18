#include <emscripten.h>
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
    u64 currentTime;
    u64 deltaTime;
    u64 lastTime = 0;
    if(emulator.shouldBeRunning()) {
        currentTime = SDL_GetTicks64();
        deltaTime = currentTime - lastTime;

        emulator.handleInput();
        emulator.update(deltaTime);
        emulator.render();
        
        lastTime = currentTime;
    }
}

int main()
{
    emscripten_set_main_loop(mainLoop, 0, 1);
    return 0;
}