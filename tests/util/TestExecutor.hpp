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
