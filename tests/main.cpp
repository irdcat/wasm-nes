#include "CpuInstructionsTest.hpp"

#include "util/TestExecutor.hpp"

int main()
{
    TestExecutor testExecutor;

    testExecutor.registerIntegrationTest<CpuInstructionsTest>();

    return testExecutor.runAll();
}