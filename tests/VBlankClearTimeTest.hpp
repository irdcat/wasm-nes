#pragma once

#include "util/IntegrationTest.hpp"

class VBlankClearTimeTest : public IntegrationTest
{
    public:
        ~VBlankClearTimeTest() = default;

        VBlankClearTimeTest(const std::shared_ptr<SystemUnderTest>& sut)
            : IntegrationTest("VBlankClearTimeTest", sut)
        {
        }

        bool setUp() override
        {
            auto cartridge = getSystemUnderTest()->getCartridge();
            auto cpu = getSystemUnderTest()->getCpu();
            if(cartridge->loadFromFile(std::ifstream("vbl_clear_time.nes", std::ios::binary))) {
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
                if(cpu->getRegisters().pc == 0xE000) {
                    code = mmu->readFromMemory(0xF0);
                    break;
                }
            }
            return code == 1 ? 0 : code;
        }
};