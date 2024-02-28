#pragma once

#include "util/IntegrationTest.hpp"

class CpuInstructionsTest : public IntegrationTest
{
    public:
        ~CpuInstructionsTest() = default;

        CpuInstructionsTest(const std::shared_ptr<SystemUnderTest>& sut)
            : IntegrationTest("CpuInstructionsTest", sut)
        {
        }

        bool setUp() override
        {
            auto cartridge = getSystemUnderTest()->getCartridge();
            auto cpu = getSystemUnderTest()->getCpu();
            if(cartridge->loadFromFile(std::ifstream("all_instrs.nes", std::ios::binary))) {
                cpu->reset();
                return true;
            }
            return false;
        }

        int run() override
        {
            auto cartridge = getSystemUnderTest()->getCartridge();
            auto cpu = getSystemUnderTest()->getCpu();
            auto resetTriggerTimer = -1;
            auto code = 0x80u;
            while(true) {
                if(resetTriggerTimer >= 0) {
                    if(resetTriggerTimer == 0) {
                        cpu->reset();
                    }
                    resetTriggerTimer--;
                }
                cpu->step();
                code = cartridge->read(0x6000);
                if (code < 0x80) {
                    // Test either failed or all passed
                    break;
                } else if (code == 0x81) {
                    resetTriggerTimer = 10;
                }
            }
            return code;
        }
};