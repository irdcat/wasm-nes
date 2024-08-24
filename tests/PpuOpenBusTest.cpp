#include "util/BlarggRomTest.hpp"

class PpuOpenBusTest : public BlarggRomTest
{
    protected:
        PpuOpenBusTest() = default;

        ~PpuOpenBusTest() = default;

        void SetUp() override
        {
            BlarggRomTest::SetUp();
        }

        void TearDown() override
        {
            BlarggRomTest::TearDown();
        }
};

TEST_F(PpuOpenBusTest, OpenBusBehaviour)
{
    auto result = run("resources/ppu_open_bus.nes");
    ASSERT_EQ(0, result) << (result == 0x100 ? "Failed to load ROM" : readMessage());
}