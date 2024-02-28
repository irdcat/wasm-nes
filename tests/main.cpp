#include "CpuInstructionsTest.hpp"
#include "PpuVblankNmiTest.hpp"
#include "PpuOpenBusTest.hpp"
#include "PpuReadBufferTest.hpp"
#include "PaletteRamTest.hpp"
#include "SpriteRamTest.hpp"

#include "util/TestExecutor.hpp"

int main()
{
    TestExecutor testExecutor;

    testExecutor.registerIntegrationTest<CpuInstructionsTest>();
    testExecutor.registerIntegrationTest<PpuVblankNmiTest>();
    testExecutor.registerIntegrationTest<PpuOpenBusTest>();
    testExecutor.registerIntegrationTest<PpuReadBufferTest>();
    testExecutor.registerIntegrationTest<PaletteRamTest>();
    testExecutor.registerIntegrationTest<SpriteRamTest>();

    return testExecutor.runAll();
}