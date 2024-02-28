#include "IntegrationTest.hpp"

IntegrationTest::IntegrationTest(const std::string& name, const std::shared_ptr<SystemUnderTest>& sut)
    : Test(name)
    , sut(sut)
{
}

bool IntegrationTest::setUp()
{
    sut->getCpu()->reset();
    return true;
}

SystemUnderTest *IntegrationTest::getSystemUnderTest()
{
    return sut.get();
}
