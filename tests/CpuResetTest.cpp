#include "util/BlarggRomTest.hpp"

class CpuResetTest : public BlarggRomTest
{
    protected:
        CpuResetTest() = default;

        ~CpuResetTest() = default;

        void SetUp() override
        {
            BlarggRomTest::SetUp();
        }

        void TearDown() override
        {
            BlarggRomTest::TearDown();
        }
};

TEST_F(CpuResetTest, Registers)
{
    GTEST_SKIP() << "Ignored due to lack of $4015 IRQ flag implementation";
    auto result = run("resources/cpu_reset/registers.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuResetTest, RamAfterReset)
{
    GTEST_SKIP() << "Ignored due to lack of $4015 IRQ flag implementation";
    auto result = run("resources/cpu_reset/ram_after_reset.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}