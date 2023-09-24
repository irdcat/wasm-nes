#pragma once

#include <memory>

#include "Mmu.hpp"
#include "CpuRegisters.hpp"

class Cpu
{
    public:
        Cpu(const std::shared_ptr<Mmu>& mmu);

        ~Cpu() = default;

        u8 fetchOpcode();

        unsigned executeInstruction(u8 opcode);

        CpuRegisters& getRegisters();

    private:
        std::shared_ptr<Mmu> mmu;
        CpuRegisters registers;

        u8 fetchImmedate8();
};