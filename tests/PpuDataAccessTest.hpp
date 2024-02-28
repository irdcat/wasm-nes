#pragma once

#include "util/IntegrationTest.hpp"

class PpuDataAccessTest : public IntegrationTest
{
    public:
        ~PpuDataAccessTest() = default;

        PpuDataAccessTest(const std::shared_ptr<SystemUnderTest>& sut)
            : IntegrationTest("PpuDataAccessTest", sut)
        {
        }

        int run() override
        {
            auto ppu = getSystemUnderTest()->getPpu();

            // Set PPUADDR register to 0x2101 and write 0x76 to PPUDATA
            ppu->write(6, 0x21);
            ppu->write(6, 0x01);
            ppu->write(7, 0x76);
            ppu->write(7, 0x00);

            // Writing to PPUDATA should result in PPUADDR incrementing so we shouldn't have get 0x76
            ppu->read(7);
            auto value = ppu->read(7);
            if(value == 0x76) {
                // Address was not incremented or garbage in memory was same as saved value
                return 1;
            }

            // Set PPUADDR register to 0x2101 and read from PPUDATA
            ppu->write(6, 0x21);
            ppu->write(6, 0x01);
            ppu->read(7);
            value = ppu->read(7);
            if(value != 0x76) {
                // We're not getting the same value from 0x2101 as we saved
                return 2;
            }

            return 0;
        }
};