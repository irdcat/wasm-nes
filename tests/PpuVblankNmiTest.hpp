#pragma once

#include "../src/Cpu.hpp"
#include "../src/Ppu.hpp"
#include "../src/Mmu.hpp"
#include "../src/Cartridge.hpp"
#include "../src/PpuFramebuffer.hpp"
#include "Test.hpp"

class PpuVblankNmiTest : public Test
{
    public:
        ~PpuVblankNmiTest() = default;

        PpuVblankNmiTest()
            : ppuFramebuffer(std::make_shared<PpuFramebuffer>())
            , cartridge(std::make_shared<Cartridge>())
            , ppu(std::make_shared<Ppu>(cartridge, ppuFramebuffer, [&](){ cpu->interrupt(InterruptType::NMI); }, [](){}))
            , mmu(std::make_shared<Mmu>(ppu, cartridge))
            , cpu(std::make_shared<Cpu>(mmu))
        {
        }

        bool setUp() override
        {
            if(cartridge->loadFromFile(std::ifstream("ppu_vbl_nmi.nes", std::ios::binary))) {
                cpu->reset();
                return true;
            }
            return false;
        }

        int run() override
        {
            auto resetTriggerTimer = -1;
            auto code = 0x80u;
            while(true) {
                if(resetTriggerTimer >= 0) {
                    if(resetTriggerTimer == 0) {
                        cpu->reset();
                    }
                    resetTriggerTimer--;
                }
                cpu->step();
                code = cartridge->read(0x6000);
                if (code < 0x80) {
                    // Test either failed or all passed
                    break;
                } else if (code == 0x81) {
                    resetTriggerTimer = 10;
                }
            }
            return code;
        }

        std::string name() override
        {
            return std::string("PpuVblankNmiTest");
        }

    private:
        std::shared_ptr<PpuFramebuffer> ppuFramebuffer;
        std::shared_ptr<Cartridge> cartridge;
        std::shared_ptr<Ppu> ppu;
        std::shared_ptr<Mmu> mmu;
        std::shared_ptr<Cpu> cpu;
};