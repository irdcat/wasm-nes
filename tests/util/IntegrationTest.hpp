#pragma once

#include "Test.hpp"
#include "SystemUnderTest.hpp"

class IntegrationTest : public Test
{
    public:
        explicit IntegrationTest(const std::string& name, const std::shared_ptr<SystemUnderTest>& sut);

        virtual ~IntegrationTest() = default;

        virtual bool setUp() override;

        SystemUnderTest* getSystemUnderTest();

    private:
        std::shared_ptr<SystemUnderTest> sut;
};