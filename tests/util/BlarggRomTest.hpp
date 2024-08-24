#pragma once

#include <gtest/gtest.h>

#include "SystemUnderTest.hpp"

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