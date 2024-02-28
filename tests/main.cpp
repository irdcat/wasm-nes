#include "CpuInstructionsTest.hpp"
#include "PpuVblankNmiTest.hpp"
#include "PpuOpenBusTest.hpp"
#include "PpuReadBufferTest.hpp"
#include "PpuPaletteRamTest.hpp"
#include "PpuSpriteRamTest.hpp"
#include "PpuDataAccessTest.hpp"
#include "PpuVramAccessTest.hpp"

#include "util/TestExecutor.hpp"

int main()
{
    TestExecutor testExecutor;

    testExecutor.registerIntegrationTest<CpuInstructionsTest>();
    testExecutor.registerIntegrationTest<PpuVblankNmiTest>();
    testExecutor.registerIntegrationTest<PpuOpenBusTest>();
    testExecutor.registerIntegrationTest<PpuReadBufferTest>();
    testExecutor.registerIntegrationTest<PpuPaletteRamTest>();
    testExecutor.registerIntegrationTest<PpuSpriteRamTest>();
    testExecutor.registerIntegrationTest<PpuDataAccessTest>();
    testExecutor.registerIntegrationTest<PpuVramAccessTest>();

    return testExecutor.runAll();
}