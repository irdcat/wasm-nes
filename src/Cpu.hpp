#pragma once

#include <memory>

#include "Mmu.hpp"
#include "CpuRegisters.hpp"
#include "AddressingMode.hpp"

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
        u16 fetchImmedate16();

        template <AddressingMode Mode> u8 resolveOperand8();
        template <AddressingMode Mode> u16 resolveOperand16();

        // Load/Transfer instructions
        template <AddressingMode Mode> unsigned lda();          // LDA - Load A
        template <AddressingMode Mode> unsigned ldx();          // LDX - Load X
        template <AddressingMode Mode> unsigned ldy();          // LDY - Load Y
        template <AddressingMode Mode> unsigned sta();          // STA - Store A
        template <AddressingMode Mode> unsigned stx();          // STX - Store X
        template <AddressingMode Mode> unsigned sty();          // STY - Store Y
        template <AddressingMode Mode> unsigned tax();          // TAX - Transfer A to X
        template <AddressingMode Mode> unsigned tay();          // TAY - Transfer A to X
        template <AddressingMode Mode> unsigned tsx();          // TSX - Transfer S to X
        template <AddressingMode Mode> unsigned txa();          // TXA - Transfer X to A
        template <AddressingMode Mode> unsigned txs();          // TXS - Transfer X to S
        template <AddressingMode Mode> unsigned tya();          // TYA - Transfer T to A
};

#include "Cpu.inl"