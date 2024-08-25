#include "util/BlarggRomTest.hpp"

class PpuVblankNmiTest : public BlarggRomTest
{
    protected:
        PpuVblankNmiTest() = default;

        ~PpuVblankNmiTest() = default;

        void SetUp() override
        {
            BlarggRomTest::SetUp();
        }

        void TearDown() override
        {
            BlarggRomTest::TearDown();
        }
};

TEST_F(PpuVblankNmiTest, EvenOddFrames)
{
    auto result = run("resources/ppu_vbl_nmi/even_odd_frames.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuVblankNmiTest, EvenOddTiming)
{
    GTEST_SKIP() << "Needs attention (Clock is skipped too soon, relative to enabling BG)";
    auto result = run("resources/ppu_vbl_nmi/even_odd_timing.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuVblankNmiTest, NmiControl)
{
    GTEST_SKIP() << "Needs attention (Immediate occurence should be after NEXT instruction)";
    auto result = run("resources/ppu_vbl_nmi/nmi_control.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuVblankNmiTest, NmiOffTiming)
{
    GTEST_SKIP() << "Needs attention";
    auto result = run("resources/ppu_vbl_nmi/nmi_off_timing.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuVblankNmiTest, NmiOnTiming)
{
    GTEST_SKIP() << "Needs attention";
    auto result = run("resources/ppu_vbl_nmi/nmi_on_timing.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuVblankNmiTest, NmiTiming)
{
    GTEST_SKIP() << "Needs attention";
    auto result = run("resources/ppu_vbl_nmi/nmi_timing.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuVblankNmiTest, Suppression)
{
    GTEST_SKIP() << "Needs attention";
    auto result = run("resources/ppu_vbl_nmi/suppression.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuVblankNmiTest, VblankBasics)
{
    auto result = run("resources/ppu_vbl_nmi/vbl_basics.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuVblankNmiTest, VblankClearTime)
{
    GTEST_SKIP() << "Needs attention";
    auto result = run("resources/ppu_vbl_nmi/vbl_clear_time.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuVblankNmiTest, VblankSetTime)
{
    GTEST_SKIP() << "Needs attention";
    auto result = run("resources/ppu_vbl_nmi/vbl_set_time.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}