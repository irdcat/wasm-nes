#include "CpuInstructionsTest.hpp"
#include "PpuVblankNmiTest.hpp"
#include "PpuOpenBusTest.hpp"
#include "PpuReadBufferTest.hpp"
#include "PpuPaletteRamTest.hpp"
#include "PpuSpriteRamTest.hpp"
#include "PpuDataAccessTest.hpp"
#include "PpuVramAccessTest.hpp"
#include "VBlankClearTimeTest.hpp"
#include "SpriteHitBasicsTest.hpp"
#include "SpriteHitAlignmentTest.hpp"
#include "SpriteHitCornersTest.hpp"
#include "SpriteHitFlipTest.hpp"
#include "SpriteHitLeftClipTest.hpp"
#include "SpriteHitRightEdgeTest.hpp"
#include "SpriteHitScreenBottomTest.hpp"
#include "SpriteHitDoubleHeightTest.hpp"
#include "SpriteHitTimingBasicsTest.hpp"
#include "SpriteHitTimingOrderTest.hpp"
#include "SpriteHitEdgeTimingTest.hpp"

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
    testExecutor.registerIntegrationTest<VBlankClearTimeTest>();
    testExecutor.registerIntegrationTest<SpriteHitBasicsTest>();
    testExecutor.registerIntegrationTest<SpriteHitAlignmentTest>();
    testExecutor.registerIntegrationTest<SpriteHitCornersTest>();
    testExecutor.registerIntegrationTest<SpriteHitFlipTest>();
    testExecutor.registerIntegrationTest<SpriteHitLeftClipTest>();
    testExecutor.registerIntegrationTest<SpriteHitRightEdgeTest>();
    testExecutor.registerIntegrationTest<SpriteHitScreenBottomTest>();
    testExecutor.registerIntegrationTest<SpriteHitDoubleHeightTest>();
    testExecutor.registerIntegrationTest<SpriteHitTimingBasicsTest>();
    testExecutor.registerIntegrationTest<SpriteHitTimingOrderTest>();
    testExecutor.registerIntegrationTest<SpriteHitEdgeTimingTest>();

    return testExecutor.runAll();
}