#pragma once

#include <gtest/gtest.h>

#include "SystemUnderTest.hpp"

/**
 * Generic rom based test.
 * This kind of test requires address of test result and address of infinite loop jump label. 
 */
class GenericRomTest : public ::testing::Test
{
    public:
        std::unique_ptr<SystemUnderTest> systemUnderTest;

        GenericRomTest() = default;

        virtual ~GenericRomTest() = default;

        virtual void SetUp() override;

        virtual void TearDown() override;

        unsigned run(std::string romFileName, u16 resultAddress, u16 infiniteLoopAddress);
};