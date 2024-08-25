#include "util/BlarggRomTest.hpp"

class CpuMiscTest : public BlarggRomTest
{
    protected:
        CpuMiscTest() = default;

        ~CpuMiscTest() = default;

        void SetUp() override
        {
            BlarggRomTest::SetUp();
        }

        void TearDown() override
        {
            BlarggRomTest::TearDown();
        }
};

TEST_F(CpuMiscTest, AbsXWrap)
{
    auto result = run("resources/instr_misc/abs_x_wrap.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuMiscTest, BranchWrap)
{
    auto result = run("resources/instr_misc/branch_wrap.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuMiscTest, DummyReads)
{
    auto result = run("resources/instr_misc/dummy_reads.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuMiscTest, DummyReadsApu)
{
    GTEST_SKIP() << "Ignored due to lack of $4015 IRQ flag implementation";
    auto result = run("resources/instr_misc/dummy_reads_apu.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuMiscTest, DummyWritesOam)
{
    auto result = run("resources/instr_misc/cpu_dummy_writes_oam.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuMiscTest, DummyWritesPpuMemory)
{
    auto result = run("resources/instr_misc/cpu_dummy_writes_ppumem.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}
