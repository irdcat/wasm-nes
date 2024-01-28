#pragma once

#include "MirroringType.hpp"

struct NesHeaderData
{
    unsigned prgRomBanks;
    unsigned chrRomBanks;
    unsigned mapperNo;
    bool persistentMemory;
    bool trainer;
    MirroringType mirroring;
};