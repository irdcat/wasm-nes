#include "BlarggRomTest.hpp"

#include <array>

void BlarggRomTest::SetUp()
{
    systemUnderTest = std::make_unique<SystemUnderTest>();
}

void BlarggRomTest::TearDown()
{
}

unsigned BlarggRomTest::run(std::string romFileName)
{
    auto cartridge = systemUnderTest->getCartridge();
    auto cpu = systemUnderTest->getCpu();
    auto mmu = systemUnderTest->getMmu();
    if(!cartridge->loadFromFile(std::ifstream(romFileName, std::ios::binary))) {
        return 0x100;
    }
    cpu->reset();
    auto resetRequested = false;
    auto resetDelay = 0;
    auto elapsedCycles = 0;
    while(true) {
        cpu->step();
        elapsedCycles = mmu->getAndResetTickCounterValue();
        if(resetDelay <= 0 && resetRequested) {
            cpu->reset();
            resetRequested = false;
        } else if(resetDelay > 0 && resetRequested) {
            resetDelay -= elapsedCycles;
        }
        std::array<u8, 3> testActivityBytes = {
            cartridge->read(0x6001),
            cartridge->read(0x6002),
            cartridge->read(0x6003)
        };
        std::string testActivity(testActivityBytes.begin(), testActivityBytes.end());
        if (testActivity != "\xDE\xB0\x61") {
            continue;
        }
        auto result = cartridge->read(0x6000);
        switch (result) {
            case 0x00:
                // Passed
                return 0;
            case 0x80:
                // Still running
                break;
            case 0x81:
                // Reset required soon
                if (resetDelay == -1) {
                    resetDelay = 410000;
                    resetRequested = true;
                }
                break;
            default:
                // Test failed
                return result;
        }
    }
}

std::string BlarggRomTest::readMessage()
{
    auto cartridge = systemUnderTest->getCartridge();
    const u16 MESSAGE_START_ADDR = 0x6004;
    u16 addr = MESSAGE_START_ADDR;
    std::string message;
    do {
        auto byte = cartridge->read(addr);
        if(byte == 0) {
            break;
        }
        message.push_back(static_cast<char>(byte));
        addr++;
    } while(true);
    return message;
}
