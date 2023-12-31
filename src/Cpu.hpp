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

        u8 popFromStack8();
        void pushIntoStack8(u8 value);

        u16 popFromStack16();
        void pushIntoStack16(u16 value);

        void updateZeroFlag(auto value);
        void updateNegativeFlag(auto value);
        void updateOverflowFlag(auto valueBefore, auto valueAfter);
        void updateCarryFlag(auto value);

        bool checkForPageCross(u16 oldAddress, u16 newAddress);

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

        // Increment/Decrement instructions
        template <AddressingMode Mode> unsigned dec();          // DEC - Decrement (mem)
        template <AddressingMode Mode> unsigned dex();          // DEX - Decrement X
        template <AddressingMode Mode> unsigned dey();          // DEY - Decrement Y
        template <AddressingMode Mode> unsigned inc();          // INC - Increment (mem)
        template <AddressingMode Mode> unsigned inx();          // INX - Increment X
        template <AddressingMode Mode> unsigned iny();          // INY - Increment Y

        // Flag instructions
        template <AddressingMode Mode> unsigned clc();          // CLC - Clear carry
        template <AddressingMode Mode> unsigned cld();          // CLD - Clear decimal
        template <AddressingMode Mode> unsigned cli();          // CLI - Clear interrupt disable
        template <AddressingMode Mode> unsigned clv();          // CLV - Clear overflow
        template <AddressingMode Mode> unsigned sec();          // SEC - Set carry
        template <AddressingMode Mode> unsigned sed();          // SED - Set decimal
        template <AddressingMode Mode> unsigned sei();          // SEI - Set interrupt disable

        // Stack instructions
        template <AddressingMode Mode> unsigned pha();          // PHA - Push Accumulator
        template <AddressingMode Mode> unsigned php();          // PHP - Push Processor status
        template <AddressingMode Mode> unsigned pla();          // PLA - Pull Accumulator
        template <AddressingMode Mode> unsigned plp();          // PLP - Pull Processor status

        // Jump & Subroutines instructions
        template <AddressingMode Mode> unsigned jmp();          // JMP - Jump
        template <AddressingMode Mode> unsigned jsr();          // JSR - Jump to subroutine
        template <AddressingMode Mode> unsigned rts();          // RTS - Return from subroutine

        // Arithmetic instructions
        template <AddressingMode Mode> unsigned adc();          // ADC - Add with Carry
        template <AddressingMode Mode> unsigned sbc();          // SBC - Subtract with Carry

        // Logical instructions
        template <AddressingMode Mode> unsigned _and();         // AND - Logical AND
        template <AddressingMode Mode> unsigned eor();          // EOR - Logical Exclusive OR (XOR)
        template <AddressingMode Mode> unsigned ora();          // ORA - Logical OR

        // Conditional Branch instructions
        template <AddressingMode Mode> unsigned bcc();          // BCC - Branch Carry Clear
        template <AddressingMode Mode> unsigned bcs();          // BCS - Branch Carry Set
        template <AddressingMode Mode> unsigned beq();          // BEQ - Branch Equal
        template <AddressingMode Mode> unsigned bmi();          // BMI - Branch Minus
        template <AddressingMode Mode> unsigned bne();          // BNE - Branch Not Equal
        template <AddressingMode Mode> unsigned bpl();          // BPL - Branch Plus
        template <AddressingMode Mode> unsigned bvc();          // BVC - Branch Overflow Clear
        template <AddressingMode Mode> unsigned bvs();          // BVS - Branch Overflow Set

        // Shift and Rotate instructions
        template <AddressingMode Mode> unsigned asl();          // ASL - Arithmetic Shift Left
        template <AddressingMode Mode> unsigned lsr();          // LSR - Logical Shift Right
        template <AddressingMode Mode> unsigned rol();          // ROL - Rotate Left
        template <AddressingMode Mode> unsigned ror();          // ROR - Rotate Right

        // Comparison instructions
        template <AddressingMode Mode> unsigned cmp();          // CMP - Compare with Accumulator
        template <AddressingMode Mode> unsigned cpx();          // CPX - Compare with X
        template <AddressingMode Mode> unsigned cpy();          // CPY - Compare with Y

        // Other instructions
        template <AddressingMode Mode> unsigned nop();          // NOP - No operation
        template <AddressingMode Mode> unsigned bit();          // BIT - Test bit
};

#include "Cpu.inl"