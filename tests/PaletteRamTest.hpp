#pragma once

#include "../src/Cpu.hpp"
#include "../src/Ppu.hpp"
#include "../src/Mmu.hpp"
#include "../src/Cartridge.hpp"
#include "../src/PpuFramebuffer.hpp"
#include "Test.hpp"

class PaletteRamTest : public Test
{
    public:
        ~PaletteRamTest() = default;

        PaletteRamTest()
            : ppuFramebuffer(std::make_shared<PpuFramebuffer>())
            , cartridge(std::make_shared<Cartridge>())
            , ppu(std::make_shared<Ppu>(cartridge, ppuFramebuffer, [&](){ cpu->interrupt(InterruptType::NMI); }, [](){}))
            , mmu(std::make_shared<Mmu>(ppu, cartridge))
            , cpu(std::make_shared<Cpu>(mmu))
        {
        }

        bool setUp() override
        {
            if(cartridge->loadFromFile(std::ifstream("palette_ram.nes", std::ios::binary))) {
                cpu->reset();
                return true;
            }
            return false;
        }

        int run() override
        {
            auto code = 0xFF;
            while(true) {
                cpu->step();
                if(cpu->getRegisters().pc == 0xE000) {
                    code = mmu->readFromMemory(0xF0);
                    break;
                }
            }
            return code == 1 ? 0 : code;
        }

        std::string name() override
        {
            return std::string("PaletteRamTest");
        }

    private:
        std::shared_ptr<PpuFramebuffer> ppuFramebuffer;
        std::shared_ptr<Cartridge> cartridge;
        std::shared_ptr<Ppu> ppu;
        std::shared_ptr<Mmu> mmu;
        std::shared_ptr<Cpu> cpu;
};