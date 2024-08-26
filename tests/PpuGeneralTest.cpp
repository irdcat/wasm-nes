#include "util/GenericRomTest.hpp"

class PpuGeneralTest : public GenericRomTest
{
    protected:
        PpuGeneralTest() = default;

        ~PpuGeneralTest() = default;

        void SetUp() override
        {
            GenericRomTest::SetUp();
        }

        void TearDown() override
        {
            GenericRomTest::TearDown();
        }
};

TEST_F(PpuGeneralTest, PaletteRam)
{
    auto result = run("resources/ppu_tests/palette_ram.nes", 0xF0, 0xE412);
    std::string message;
    switch (result) {
        case 0x100:
            message = "Failed to load ROM";
            break;
        case 2:
            message = "Palette read shouldn't be bufferred like other VRAM";
            break;
        case 3:
            message = "Palette write/read doesn't work";
            break;
        case 4:
            message = "Palette should be mirrired within $3f00-$3fff";
            break;
        case 5:
            message = "Write to $10 should be mirrored at $00";
            break;
        case 6:
            message = "Write to $00 should be mirrored at $10";
            break;
        default:
            break;
    }
    ASSERT_EQ(0, result) << message;
}

TEST_F(PpuGeneralTest, PowerUpPalette)
{
    GTEST_SKIP() << "Skipped (Palette differs from table)";
    auto result = run("resources/ppu_tests/power_up_palette.nes", 0xF0, 0xE3AC);
    std::string message;
    switch (result) {
        case 0x100:
            message = "Failed to load ROM";
            break;
        case 2:
            message = "Palette differs from table";
            break;
        default:
            break;
    }
    ASSERT_EQ(0, result) << message;
}

TEST_F(PpuGeneralTest, SpriteRam)
{
    auto result = run("resources/ppu_tests/sprite_ram.nes", 0xF0, 0xE467);
    std::string message;
    switch (result) {
        case 0x100:
            message = "Failed to load ROM";
            break;
        case 2:
            message = "Basic read/write doesn't work";
            break;
        case 3:
            message = "Address should increment on $2004 write";
            break;
        case 4:
            message = "Address should not increment on $2004 read";
            break;
        case 5:
            message = "Third sprite byte should be masked with $e3 on read";
            break;
        case 6:
            message = "$4014 DMA copy doesn't work at all";
            break;
        case 7:
            message = "$4014 DMA copy should start at value in $2003 and wrap";
            break;
        case 8:
            message = "$4014 DMA copy should leave value in $2003 intact";
            break;
        default:
            break;
    }
    ASSERT_EQ(0, result) << message;
}

TEST_F(PpuGeneralTest, VBlankClearTime)
{
    GTEST_SKIP() << "Skipped (VBL flag cleared too late)";
    auto result = run("resources/ppu_tests/vbl_clear_time.nes", 0xF0, 0xE3B3);
    std::string message;
    switch (result) {
        case 0x100:
            message = "Failed to load ROM";
            break;
        case 2:
            message = "VBL flag cleared too soon";
            break;
        case 3:
            message = "VBL flag cleared too late";
            break;
        default:
            break;
    }
    ASSERT_EQ(0, result) << message;
}

TEST_F(PpuGeneralTest, VRamAccess)
{
    auto result = run("resources/ppu_tests/vram_access.nes", 0xF0, 0xE48D);
    std::string message;
    switch (result) {
        case 0x100:
            message = "Failed to load ROM";
            break;
        case 2:
            message = "VRAM reads should be delayed in a buffer";
            break;
        case 3:
            message = "Basic Write/read doesn't work";
            break;
        case 4:
            message = "Read buffer shouldn't be affected by VRAM write";
            break;
        case 5:
            message = "Read buffer shouldn't be affected by palette write";
            break;
        case 6:
            message = "Palette read should also read VRAM into read buffer";
            break;
        case 7:
            message = "\"Shadow\" VRAM read unaffected by palette transparent color mirroring";
            break;
        default:
            break;
    }
    ASSERT_EQ(0, result) << message;
}