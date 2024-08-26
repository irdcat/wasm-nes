#pragma once

#include <gtest/gtest.h>

#include "SystemUnderTest.hpp"

/**
 * Test based on some of the test roms developed by blargg (Shay Green).
 * 
 * Tests save the test result at the memory location 0x6000.
 * However to let the emulator know when the value at 0x6000 is a valid test result
 * 0xDE 0xB0 0x61 are written to 0x6001 - 0x6003.
 * At 0x6004 rom store a null-terminated string. As text is being produced, terminator is moved forward.
 */
class BlarggRomTest : public ::testing::Test
{
    public:
        std::unique_ptr<SystemUnderTest> systemUnderTest;

        BlarggRomTest() = default;

        virtual ~BlarggRomTest() = default;

        virtual void SetUp() override;

        virtual void TearDown() override;

        unsigned run(std::string romFileName);

        std::string readMessage();
};