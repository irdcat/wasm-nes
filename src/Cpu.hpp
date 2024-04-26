#pragma once

#include <memory>
#include <functional>

#include "Mmu.hpp"
#include "CpuRegisters.hpp"
#include "AddressingMode.hpp"
#include "InterruptType.hpp"

/**
 * CPU - Central Processing Unit
 * This class represents CPU part Ricoh RP2A03 CPU (Used by NES in NTSC region).
 */
class Cpu
{
    public:
        explicit Cpu(const std::shared_ptr<Mmu>& mmu);

        ~Cpu() = default;

        unsigned step();

        void reset();

        u8 fetchOpcode();

        void executeInstruction(u8 opcode);

        void interrupt(InterruptType type);

        CpuRegisters& getRegisters();

    private:
        std::shared_ptr<Mmu> mmu;
        CpuRegisters registers;
        bool halted;
        bool irqPending;
        bool nmiPending;

        void handleInterrupt(InterruptType type);

        u8 fetchImmedate8();
        u16 fetchImmedate16();

        u8 popFromStack8();
        void pushIntoStack8(u8 value);

        u16 popFromStack16();
        void pushIntoStack16(u16 value);

        void updateZeroFlag(auto value);
        void updateNegativeFlag(auto value);

        template <AddressingMode Mode> u8 resolveReadOperand();
        template <AddressingMode Mode> u16 resolveWriteAddress();
        template <AddressingMode Mode> void executeImplied(const std::function<void()>& op);
        template <AddressingMode Mode> void executeBranchInstruction(bool condition);
        template <AddressingMode Mode> void executeReadModifyWrite(const std::function<void(u8&)>& op);

        // Load/Transfer instructions
        template <AddressingMode Mode> void lda();          // LDA - Load A
        template <AddressingMode Mode> void ldx();          // LDX - Load X
        template <AddressingMode Mode> void ldy();          // LDY - Load Y
        template <AddressingMode Mode> void sta();          // STA - Store A
        template <AddressingMode Mode> void stx();          // STX - Store X
        template <AddressingMode Mode> void sty();          // STY - Store Y
        template <AddressingMode Mode> void tax();          // TAX - Transfer A to X
        template <AddressingMode Mode> void tay();          // TAY - Transfer A to X
        template <AddressingMode Mode> void tsx();          // TSX - Transfer S to X
        template <AddressingMode Mode> void txa();          // TXA - Transfer X to A
        template <AddressingMode Mode> void txs();          // TXS - Transfer X to S
        template <AddressingMode Mode> void tya();          // TYA - Transfer T to A

        // Increment/Decrement instructions
        template <AddressingMode Mode> void dec();          // DEC - Decrement (mem)
        template <AddressingMode Mode> void dex();          // DEX - Decrement X
        template <AddressingMode Mode> void dey();          // DEY - Decrement Y
        template <AddressingMode Mode> void inc();          // INC - Increment (mem)
        template <AddressingMode Mode> void inx();          // INX - Increment X
        template <AddressingMode Mode> void iny();          // INY - Increment Y

        // Flag instructions
        template <AddressingMode Mode> void clc();          // CLC - Clear carry
        template <AddressingMode Mode> void cld();          // CLD - Clear decimal
        template <AddressingMode Mode> void cli();          // CLI - Clear interrupt disable
        template <AddressingMode Mode> void clv();          // CLV - Clear overflow
        template <AddressingMode Mode> void sec();          // SEC - Set carry
        template <AddressingMode Mode> void sed();          // SED - Set decimal
        template <AddressingMode Mode> void sei();          // SEI - Set interrupt disable

        // Stack instructions
        template <AddressingMode Mode> void pha();          // PHA - Push Accumulator
        template <AddressingMode Mode> void php();          // PHP - Push Processor status
        template <AddressingMode Mode> void pla();          // PLA - Pull Accumulator
        template <AddressingMode Mode> void plp();          // PLP - Pull Processor status

        // Jump & Subroutines instructions
        template <AddressingMode Mode> void jmp();          // JMP - Jump
        template <AddressingMode Mode> void jsr();          // JSR - Jump to subroutine
        template <AddressingMode Mode> void rts();          // RTS - Return from subroutine

        // Arithmetic instructions
        template <AddressingMode Mode> void adc();          // ADC - Add with Carry
        template <AddressingMode Mode> void sbc();          // SBC - Subtract with Carry

        // Logical instructions
        template <AddressingMode Mode> void _and();         // AND - Logical AND
        template <AddressingMode Mode> void eor();          // EOR - Logical Exclusive OR (XOR)
        template <AddressingMode Mode> void ora();          // ORA - Logical OR

        // Conditional Branch instructions
        template <AddressingMode Mode> void bcc();          // BCC - Branch Carry Clear
        template <AddressingMode Mode> void bcs();          // BCS - Branch Carry Set
        template <AddressingMode Mode> void beq();          // BEQ - Branch Equal
        template <AddressingMode Mode> void bmi();          // BMI - Branch Minus
        template <AddressingMode Mode> void bne();          // BNE - Branch Not Equal
        template <AddressingMode Mode> void bpl();          // BPL - Branch Plus
        template <AddressingMode Mode> void bvc();          // BVC - Branch Overflow Clear
        template <AddressingMode Mode> void bvs();          // BVS - Branch Overflow Set

        // Shift and Rotate instructions
        template <AddressingMode Mode> void asl();          // ASL - Arithmetic Shift Left
        template <AddressingMode Mode> void lsr();          // LSR - Logical Shift Right
        template <AddressingMode Mode> void rol();          // ROL - Rotate Left
        template <AddressingMode Mode> void ror();          // ROR - Rotate Right

        // Comparison instructions
        template <AddressingMode Mode> void cmp();          // CMP - Compare with Accumulator
        template <AddressingMode Mode> void cpx();          // CPX - Compare with X
        template <AddressingMode Mode> void cpy();          // CPY - Compare with Y

        // Interrupt instructions
        template <AddressingMode Mode> void brk();          // BRK - Break
        template <AddressingMode Mode> void rti();          // RTI - Return from Interrupt

        // Other instructions
        template <AddressingMode Mode> void nop();          // NOP - No operation
        template <AddressingMode Mode> void bit();          // BIT - Test bit

        // Unofficial instructions
        template <AddressingMode Mode> void alr();          // ALR - AND + LSR
        template <AddressingMode Mode> void anc();          // ANC - AND + Set carry as Shift/Rotate instruction
        template <AddressingMode Mode> void xaa();          // XAA - AND X + AND
        template <AddressingMode Mode> void arr();          // ARR - AND + ROR
        template <AddressingMode Mode> void dcp();          // DCP - DEC + CMP
        template <AddressingMode Mode> void isc();          // ISC - INC + SBC
        template <AddressingMode Mode> void las();          // LAS - LDA/TSX
        template <AddressingMode Mode> void lax();          // LAX - LDA + LDX
        template <AddressingMode Mode> void lxa();          // LXA - A and M -> A -> X
        template <AddressingMode Mode> void rla();          // RLA - ROL + AND
        template <AddressingMode Mode> void rra();          // RRA - ROR + ADC
        template <AddressingMode Mode> void sax();          // SAX - A and X -> M
        template <AddressingMode Mode> void axs();          // AXS - (A and X) - M -> X
        template <AddressingMode Mode> void ahx();          // AHX - A and X and (high-byte of addr + 1) -> M
        template <AddressingMode Mode> void shx();          // SHX - X and (high-byte of addr + 1) -> M
        template <AddressingMode Mode> void shy();          // SHY - Y and (high-byte of addr + 1) -> M
        template <AddressingMode Mode> void slo();          // SLO - ASL + ORA
        template <AddressingMode Mode> void sre();          // SRE - LSR + EOR
        template <AddressingMode Mode> void tas();          // TAS - A and X -> SP, A and X and (high-byte of addr + 1) -> M
        template <AddressingMode Mode> void stp();          // STP - Freeze the CPU
};

#include "Cpu.inl"