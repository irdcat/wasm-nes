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