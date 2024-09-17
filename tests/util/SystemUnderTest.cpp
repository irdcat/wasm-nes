#include "SystemUnderTest.hpp"

SystemUnderTest::SystemUnderTest()
    : apu(std::make_shared<Apu>([&](){ cpu->interrupt(InterruptType::IRQ); }))
    , controllers(std::make_shared<Controllers>())
    , cartridge(std::make_shared<Cartridge>())
    , ppu(std::make_shared<Ppu>(cartridge, [&](){ cpu->interrupt(InterruptType::NMI); }, [](){}))
    , mmu(std::make_shared<Mmu>(ppu, apu, cartridge, controllers))
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

Controllers* SystemUnderTest::getControllers()
{
    return controllers.get();
}

Apu* SystemUnderTest::getApu()
{
    return apu.get();
}
