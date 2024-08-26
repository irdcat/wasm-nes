#include "GenericRomTest.hpp"

void GenericRomTest::SetUp()
{
    systemUnderTest = std::make_unique<SystemUnderTest>();
}

void GenericRomTest::TearDown()
{
}

unsigned GenericRomTest::run(std::string romFileName, u16 resultAddress, u16 infiniteLoopAddress)
{
    auto cartridge = systemUnderTest->getCartridge();
    auto mmu = systemUnderTest->getMmu();
    auto cpu = systemUnderTest->getCpu();

    if(!cartridge->loadFromFile(std::ifstream(romFileName, std::ios::binary))) {
        return 0x100;
    }

    auto code = 0xFF;
    cpu->reset();
    while(true) {
        cpu->step();
        if(cpu->getRegisters().pc == infiniteLoopAddress) {
            code = mmu->readFromMemory(resultAddress);
            break;
        }
    }
    return code == 1 ? 0 : code;
}
