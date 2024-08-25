#include "util/BlarggRomTest.hpp"

class PpuOamTest : public BlarggRomTest
{
    protected:
        PpuOamTest() = default;

        ~PpuOamTest() = default;

        void SetUp() override
        {
            BlarggRomTest::SetUp();
        }

        void TearDown() override
        {
            BlarggRomTest::TearDown();
        }
};

TEST_F(PpuOamTest, OamRead)
{
    auto result = run("resources/oam/oam_read.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(PpuOamTest, OamStress)
{
    auto result = run("resources/oam/oam_stress.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}