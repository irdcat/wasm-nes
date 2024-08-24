#include "CpuInstructionsTest.hpp"
#include "PpuDataAccessTest.hpp"

#include "util/TestExecutor.hpp"

int main()
{
    TestExecutor testExecutor;

    testExecutor.registerIntegrationTest<CpuInstructionsTest>();
    testExecutor.registerIntegrationTest<PpuDataAccessTest>();

    return testExecutor.runAll();
}