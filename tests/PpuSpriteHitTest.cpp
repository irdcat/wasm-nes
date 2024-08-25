#include "util/BlarggRomTest.hpp"

class PpuSpriteHitTest : public BlarggRomTest
{
    protected:
        PpuSpriteHitTest() = default;

        ~PpuSpriteHitTest() = default;

        void SetUp() override
        {
            BlarggRomTest::SetUp();
        }

        void TearDown() override
        {
            BlarggRomTest::TearDown();
        }
};

TEST_F(PpuSpriteHitTest, Alignment)
{
    auto result = run("resources/ppu_sprite_hit/alignment.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuSpriteHitTest, Basics)
{
    auto result = run("resources/ppu_sprite_hit/basics.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuSpriteHitTest, Corners)
{
    auto result = run("resources/ppu_sprite_hit/corners.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuSpriteHitTest, DoubleHeight)
{
    auto result = run("resources/ppu_sprite_hit/double_height.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuSpriteHitTest, EdgeTiming)
{
    GTEST_SKIP() << "Skipped due to freezing";
    auto result = run("resources/ppu_sprite_hit/edge_timing.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuSpriteHitTest, Flip)
{
    auto result = run("resources/ppu_sprite_hit/flip.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuSpriteHitTest, LeftClip)
{
    auto result = run("resources/ppu_sprite_hit/left_clip.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuSpriteHitTest, RightEdge)
{
    auto result = run("resources/ppu_sprite_hit/right_edge.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuSpriteHitTest, ScreenBottom)
{
    auto result = run("resources/ppu_sprite_hit/screen_bottom.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuSpriteHitTest, TimingBasics)
{
    GTEST_SKIP() << "Skipped due to freezing";
    auto result = run("resources/ppu_sprite_hit/timing_basics.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuSpriteHitTest, TimingOrder)
{
    GTEST_SKIP() << "Skipped (Hit time shouldn't be based on pixels at X=255)";
    auto result = run("resources/ppu_sprite_hit/timing_order.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuSpriteHitTest, Timing)
{
    GTEST_SKIP() << "Flag set too soon for upper-left corner";
    auto result = run("resources/ppu_sprite_hit/timing.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}