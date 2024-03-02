#pragma once

#include <memory>
#include <vector>

#include "Test.hpp"
#include "SystemUnderTest.hpp"

class TestExecutor
{
    public:
        TestExecutor();

        ~TestExecutor() = default;

        template <typename T>
        void registerIntegrationTest();

        template <typename T>
        void registerUnitTest();

        int runAll();

    private:
        std::vector<std::unique_ptr<Test>> tests;
        std::shared_ptr<SystemUnderTest> sut;

        static const constexpr unsigned TEST_NAME_COL_WIDTH = 48;
        static const constexpr unsigned SET_UP_RESULT_COL_WIDTH = 24;
        static const constexpr unsigned RUN_RESULT_COL_WIDTH = 24;
        static const constexpr unsigned COLS = 3;
        static const constexpr unsigned ALL_WIDTH 
            = (COLS + 1) 
            + TEST_NAME_COL_WIDTH 
            + SET_UP_RESULT_COL_WIDTH 
            + RUN_RESULT_COL_WIDTH;
};

template <typename T>
inline void TestExecutor::registerIntegrationTest()
{
    tests.push_back(std::make_unique<T>(sut));
}

template <typename T>
inline void TestExecutor::registerUnitTest()
{
    tests.push_back(std::make_unique<T>());
}
