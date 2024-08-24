#include "util/BlarggRomTest.hpp"

class CpuInstructionsTestV5 : public BlarggRomTest
{
    protected:
        CpuInstructionsTestV5() = default;

        ~CpuInstructionsTestV5() = default;

        void SetUp() override
        {
            BlarggRomTest::SetUp();
        }

        void TearDown() override
        {
            BlarggRomTest::TearDown();
        }
};

TEST_F(CpuInstructionsTestV5, AllInstructions)
{
    GTEST_SKIP() << "Skipped due to unsupported mapper 1";
    auto result = run("resources/instr_test_v5/all_instrs.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuInstructionsTestV5, OfficialInstructions)
{
    GTEST_SKIP() << "Skipped due to unsupported mapper 1";
    auto result = run("resources/instr_test_v5/official_only.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}