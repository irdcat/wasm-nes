#include "SystemUnderTest.hpp"

SystemUnderTest::SystemUnderTest()
    : cartridge(std::make_shared<Cartridge>())
    , ppu(std::make_shared<Ppu>(cartridge, [&](){ cpu->interrupt(InterruptType::NMI); }, [](){}))
    , mmu(std::make_shared<Mmu>(ppu, cartridge))
    , cpu(std::make_unique<Cpu>(mmu))
{
}

Cpu* SystemUnderTest::getCpu() 
{
    return cpu.get();
}

Mmu* SystemUnderTest::getMmu()
{
    return mmu.get();
}

Ppu* SystemUnderTest::getPpu()
{
    return ppu.get();
}

Cartridge* SystemUnderTest::getCartridge()
{
    return cartridge.get();
}