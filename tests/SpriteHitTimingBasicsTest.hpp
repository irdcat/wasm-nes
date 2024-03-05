#pragma once

#include "util/IntegrationTest.hpp"

class SpriteHitTimingBasicsTest : public IntegrationTest
{
    public:
        ~SpriteHitTimingBasicsTest() = default;

        SpriteHitTimingBasicsTest(const std::shared_ptr<SystemUnderTest>& sut)
            : IntegrationTest("SpriteHitTimingBasicsTest", sut)
        {
        }

        bool setUp() override
        {
            auto cartridge = getSystemUnderTest()->getCartridge();
            auto cpu = getSystemUnderTest()->getCpu();
            if(cartridge->loadFromFile(std::ifstream("sprite_hit_timing_basics.nes", std::ios::binary))) {
                cpu->reset();
                return true;
            }
            return false;
        }

        int run() override
        {
            auto cpu = getSystemUnderTest()->getCpu();
            auto mmu = getSystemUnderTest()->getMmu();
            auto code = 0xFF;
            while(true) {
                cpu->step();
                if(cpu->getRegisters().pc == 0xE00B) {
                    code = mmu->readFromMemory(0xF8);
                    break;
                }
            }
            return code == 1 ? 0 : code;
        }
};