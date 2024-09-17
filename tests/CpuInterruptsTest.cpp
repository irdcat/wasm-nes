#include "util/BlarggRomTest.hpp"

class CpuInterruptsTest : public BlarggRomTest
{
    protected:
        CpuInterruptsTest() = default;

        ~CpuInterruptsTest() = default;

        void SetUp() override
        {
            BlarggRomTest::SetUp();
        }

        void TearDown() override
        {
            BlarggRomTest::TearDown();
        }
};

TEST_F(CpuInterruptsTest, BranchDelaysIrq)
{
    GTEST_SKIP() << "Needs attention (Hangs)";
    auto result = run("resources/cpu_interrupts_v2/branch_delays_irq.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuInterruptsTest, CliLatency)
{
    GTEST_SKIP() << "Needs attention (APU should generate IRQ when $4017 = $00)";
    auto result = run("resources/cpu_interrupts_v2/cli_latency.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuInterruptsTest, IrqAndDma)
{
    GTEST_SKIP() << "Needs attention";
    auto result = run("resources/cpu_interrupts_v2/irq_and_dma.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuInterruptsTest, NmiAndBrk)
{
    GTEST_SKIP() << "Needs attention";
    auto result = run("resources/cpu_interrupts_v2/nmi_and_brk.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}

TEST_F(CpuInterruptsTest, NmiAndIrq)
{
    GTEST_SKIP() << "Needs attention";
    auto result = run("resources/cpu_interrupts_v2/nmi_and_irq.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}