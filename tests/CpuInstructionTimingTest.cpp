#include "util/BlarggRomTest.hpp"

class CpuInstructionTimingTest : public BlarggRomTest
{
    protected:
        CpuInstructionTimingTest() = default;

        ~CpuInstructionTimingTest() = default;

        void SetUp() override
        {
            BlarggRomTest::SetUp();
        }

        void TearDown() override
        {
            BlarggRomTest::TearDown();
        }
};

TEST_F(CpuInstructionTimingTest, InstructionTiming)
{
    GTEST_SKIP() << "Skipped due to lack of APU implementation";
    auto result = run("resources/instr_timing/instr_timing.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuInstructionTimingTest, BranchTiming)
{
    GTEST_SKIP() << "Needs further attention";
    auto result = run("resources/instr_timing/branch_timing.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}