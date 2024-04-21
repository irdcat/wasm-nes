#include "SystemUnderTest.hpp"

SystemUnderTest::SystemUnderTest()
    : ppuFramebuffer(std::make_shared<Framebuffer>())
    , cartridge(std::make_shared<Cartridge>())
    , ppu(std::make_shared<Ppu>(cartridge, ppuFramebuffer, [&](){ cpu->interrupt(InterruptType::NMI); }, [](){}))
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