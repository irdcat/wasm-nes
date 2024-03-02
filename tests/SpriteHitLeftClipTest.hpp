#pragma once

#include "util/IntegrationTest.hpp"

class SpriteHitLeftClipTest : public IntegrationTest
{
    public:
        ~SpriteHitLeftClipTest() = default;

        SpriteHitLeftClipTest(const std::shared_ptr<SystemUnderTest>& sut)
            : IntegrationTest("SpriteHitLeftClipTest", sut)
        {
        }

        bool setUp() override
        {
            auto cartridge = getSystemUnderTest()->getCartridge();
            auto cpu = getSystemUnderTest()->getCpu();
            if(cartridge->loadFromFile(std::ifstream("sprite_hit_left_clip.nes", std::ios::binary))) {
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