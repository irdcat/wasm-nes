#pragma once

#include "util/IntegrationTest.hpp"

class SpriteHitDoubleHeightTest : public IntegrationTest
{
    public:
        ~SpriteHitDoubleHeightTest() = default;

        SpriteHitDoubleHeightTest(const std::shared_ptr<SystemUnderTest>& sut)
            : IntegrationTest("SpriteHitDoubleHeightTest", sut)
        {
        }

        bool setUp() override
        {
            auto cartridge = getSystemUnderTest()->getCartridge();
            auto cpu = getSystemUnderTest()->getCpu();
            if(cartridge->loadFromFile(std::ifstream("sprite_hit_double_height.nes", std::ios::binary))) {
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