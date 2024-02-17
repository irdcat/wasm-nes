#include "Cpu.hpp"

Cpu::Cpu(const std::shared_ptr<Mmu> &mmu)
    : mmu(mmu)
{
    registers.a = 0;
    registers.x = 0;
    registers.y = 0;
    registers.p = 0x20;
    registers.s = 0;
    registers.pc = 0;
    halted = false;
    nmiPending = false;
    irqPending = false;
}

void Cpu::step()
{
    if(nmiPending) {
        handleInterrupt(InterruptType::NMI);
        nmiPending = false;
        irqPending = false;
        return;
    }
    if(irqPending) {
        handleInterrupt(InterruptType::IRQ);
        nmiPending = false;
        irqPending = false;
        return;
    }

    auto opcode = fetchOpcode();
    executeInstruction(opcode);
}

void Cpu::reset()
{
    mmu->signalReset(true);
    handleInterrupt(InterruptType::RESET);
    mmu->signalReset(false);
}

u8 Cpu::fetchOpcode()
{
    return fetchImmedate8();
}

void Cpu::executeInstruction(u8 opcode)
{
    if(halted) {
        return;
    }

    switch(opcode) {
        case 0x00: brk<AddressingMode::Implied>(); break;           // BRK implied
        case 0x01: ora<AddressingMode::IndirectX>(); break;         // ORA indirect X
        case 0x02: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0x03: slo<AddressingMode::IndirectX>(); break;         // SLO indirect X [Unofficial]
        case 0x04: nop<AddressingMode::ZeroPage>(); break;          // NOP zero-page [Unofficial]
        case 0x05: ora<AddressingMode::ZeroPage>(); break;          // ORA zero-page
        case 0x06: asl<AddressingMode::ZeroPage>(); break;          // ASL zero-page
        case 0x07: slo<AddressingMode::ZeroPage>(); break;          // SLO zero-page [Unofficial]
        case 0x08: php<AddressingMode::Implied>(); break;           // PHP implied
        case 0x09: ora<AddressingMode::Immediate>(); break;         // ORA immediate
        case 0x0A: asl<AddressingMode::Accumulator>(); break;       // ASL accumulator
        case 0x0B: anc<AddressingMode::Immediate>(); break;         // ANC immediate [Unofficial]
        case 0x0C: nop<AddressingMode::Absolute>(); break;          // NOP absolute [Unofficial]
        case 0x0D: ora<AddressingMode::Absolute>(); break;          // ORA absolute
        case 0x0E: asl<AddressingMode::Absolute>(); break;          // ASL absolute
        case 0x0F: slo<AddressingMode::Absolute>(); break;          // SLO absolute [Unofficial]
        case 0x10: bpl<AddressingMode::Relative>(); break;          // BPL relative
        case 0x11: ora<AddressingMode::IndirectY>(); break;         // ORA indirect Y
        case 0x12: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0x13: slo<AddressingMode::IndirectY>(); break;         // SLO indirect Y [Unofficial]
        case 0x14: nop<AddressingMode::ZeroPageIndexedX>(); break;  // NOP zero-page indexed X [Unofficial]
        case 0x15: ora<AddressingMode::ZeroPageIndexedX>(); break;  // ORA zero-page indexed X
        case 0x16: asl<AddressingMode::ZeroPageIndexedX>(); break;  // ASL zero-page indexed X
        case 0x17: slo<AddressingMode::ZeroPageIndexedX>(); break;  // SLO zero-page indexed X [Unofficial]
        case 0x18: clc<AddressingMode::Implied>(); break;           // CLC implied
        case 0x19: ora<AddressingMode::AbsoluteIndexedY>(); break;  // ORA absolute indexed Y
        case 0x1A: nop<AddressingMode::Implied>(); break;           // NOP implied [Unofficial]
        case 0x1B: slo<AddressingMode::AbsoluteIndexedY>(); break;  // SLO absolute indexed Y [Unofficial]
        case 0x1C: nop<AddressingMode::AbsoluteIndexedX>(); break;  // NOP absolute indexed X [Unofficial]
        case 0x1D: ora<AddressingMode::AbsoluteIndexedX>(); break;  // ORA absolute indexed X
        case 0x1E: asl<AddressingMode::AbsoluteIndexedX>(); break;  // ASL absolute indexed X
        case 0x1F: slo<AddressingMode::AbsoluteIndexedX>(); break;  // SLO absolute indexed X [Unofficial]
        case 0x20: jsr<AddressingMode::Absolute>(); break;          // JSR absolute
        case 0x21: _and<AddressingMode::IndirectX>(); break;        // AND indirect X
        case 0x22: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0x23: rla<AddressingMode::IndirectX>(); break;         // RLA indirect X [Unofficial]
        case 0x24: bit<AddressingMode::ZeroPage>(); break;          // BIT zero-page
        case 0x25: _and<AddressingMode::ZeroPage>(); break;         // AND zero-page
        case 0x26: rol<AddressingMode::ZeroPage>(); break;          // ROL zero-page
        case 0x27: rla<AddressingMode::ZeroPage>(); break;          // RLA zero-page [Unofficial]
        case 0x28: plp<AddressingMode::Implied>(); break;           // PLP implied
        case 0x29: _and<AddressingMode::Immediate>(); break;        // AND immediate
        case 0x2A: rol<AddressingMode::Accumulator>(); break;       // ROL accumulator
        case 0x2B: anc<AddressingMode::Immediate>(); break;         // ANC immediate [Unofficial]
        case 0x2C: bit<AddressingMode::Absolute>(); break;          // BIT absolute
        case 0x2D: _and<AddressingMode::Absolute>(); break;         // AND absolute
        case 0x2E: rol<AddressingMode::Absolute>(); break;          // ROL absolute
        case 0x2F: rla<AddressingMode::Absolute>(); break;          // RLA absolute [Unofficial]
        case 0x30: bmi<AddressingMode::Relative>(); break;          // BMI relative
        case 0x31: _and<AddressingMode::IndirectY>(); break;        // AND indirect Y
        case 0x32: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0x33: rla<AddressingMode::IndirectY>(); break;         // RLA indirect Y [Unofficial]
        case 0x34: nop<AddressingMode::ZeroPageIndexedX>(); break;  // NOP zero-page indexed X [Unofficial]
        case 0x35: _and<AddressingMode::ZeroPageIndexedX>(); break; // AND zero-page indexed X
        case 0x36: rol<AddressingMode::ZeroPageIndexedX>(); break;  // ROL zero-page indexed X
        case 0x37: rla<AddressingMode::ZeroPageIndexedX>(); break;  // RLA zero-page indexed X [Unofficial]
        case 0x38: sec<AddressingMode::Implied>(); break;           // SEC implied
        case 0x39: _and<AddressingMode::AbsoluteIndexedY>(); break; // AND absolute indexed Y
        case 0x3A: nop<AddressingMode::Implied>(); break;           // NOP implied [Unofficial]
        case 0x3B: rla<AddressingMode::AbsoluteIndexedY>(); break;  // RLA absolute indexed Y [Unofficial]
        case 0x3C: nop<AddressingMode::AbsoluteIndexedX>(); break;  // NOP absolute indexed X [Unofficial]
        case 0x3D: _and<AddressingMode::AbsoluteIndexedX>(); break; // AND absolute indexed X
        case 0x3E: rol<AddressingMode::AbsoluteIndexedX>(); break;  // ROL absolute indexed X
        case 0x3F: rla<AddressingMode::AbsoluteIndexedX>(); break;  // RLA absolute indexed X [Unofficial]
        case 0x40: rti<AddressingMode::Implied>(); break;           // RTI implied
        case 0x41: eor<AddressingMode::IndirectX>(); break;         // EOR indirect X
        case 0x42: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0x43: sre<AddressingMode::IndirectX>(); break;         // SRE indirect X [Unofficial]
        case 0x44: nop<AddressingMode::ZeroPage>(); break;          // NOP zero-page [Unofficial]
        case 0x45: eor<AddressingMode::ZeroPage>(); break;          // EOR zero-page
        case 0x46: lsr<AddressingMode::ZeroPage>(); break;          // LSR zero-page
        case 0x47: sre<AddressingMode::ZeroPage>(); break;          // SRE zero-page [Unofficial]
        case 0x48: pha<AddressingMode::Implied>(); break;           // PHA implied
        case 0x49: eor<AddressingMode::Immediate>(); break;         // EOR immediate
        case 0x4A: lsr<AddressingMode::Accumulator>(); break;       // LSR accumulator
        case 0x4B: alr<AddressingMode::Immediate>(); break;         // ALR immediate [Unofficial]
        case 0x4C: jmp<AddressingMode::Absolute>(); break;          // JMP absolute
        case 0x4D: eor<AddressingMode::Absolute>(); break;          // EOR absolute
        case 0x4E: lsr<AddressingMode::Absolute>(); break;          // LSR absolute
        case 0x4F: sre<AddressingMode::Absolute>(); break;          // SRE absolute [Unofficial]
        case 0x50: bvc<AddressingMode::Relative>(); break;          // BVC relative
        case 0x51: eor<AddressingMode::IndirectY>(); break;         // EOR indirect Y
        case 0x52: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0x53: sre<AddressingMode::IndirectY>(); break;         // SRE indirect Y [Unofficial]
        case 0x54: nop<AddressingMode::ZeroPageIndexedX>(); break;  // NOP zero-page indexed X [Unofficial]
        case 0x55: eor<AddressingMode::ZeroPageIndexedX>(); break;  // EOR zero-page indexed X
        case 0x56: lsr<AddressingMode::ZeroPageIndexedX>(); break;  // LSR zero-page indexed X
        case 0x57: sre<AddressingMode::ZeroPageIndexedX>(); break;  // SRE zero-page indexed X [Unofficial]
        case 0x58: cli<AddressingMode::Implied>(); break;           // CLI implied
        case 0x59: eor<AddressingMode::AbsoluteIndexedY>(); break;  // EOR absolute indexed Y
        case 0x5A: nop<AddressingMode::Implied>(); break;           // NOP implied [Unofficial]
        case 0x5B: sre<AddressingMode::AbsoluteIndexedY>(); break;  // SRE absolute indexed Y [Unofficial]
        case 0x5C: nop<AddressingMode::AbsoluteIndexedX>(); break;  // NOP absolute indexed X [Unofficial]
        case 0x5D: eor<AddressingMode::AbsoluteIndexedX>(); break;  // EOR absolute indexed X
        case 0x5E: lsr<AddressingMode::AbsoluteIndexedX>(); break;  // LSR absolute indexed X
        case 0x5F: sre<AddressingMode::AbsoluteIndexedX>(); break;  // SRE absolute indexed X [Unofficial]
        case 0x60: rts<AddressingMode::Implied>(); break;           // RTS implied
        case 0x61: adc<AddressingMode::IndirectX>(); break;         // ADC indirect X
        case 0x62: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0x63: rra<AddressingMode::IndirectX>(); break;         // RRA indirect X [Unofficial]
        case 0x64: nop<AddressingMode::ZeroPage>(); break;          // NOP zero-page [Unofficial]
        case 0x65: adc<AddressingMode::ZeroPage>(); break;          // ADC zero-page
        case 0x66: ror<AddressingMode::ZeroPage>(); break;          // ROR zero-page
        case 0x67: rra<AddressingMode::ZeroPage>(); break;          // RRA zero-page [Unofficial]
        case 0x68: pla<AddressingMode::Implied>(); break;           // PLA implied
        case 0x69: adc<AddressingMode::Immediate>(); break;         // ADC immediate
        case 0x6A: ror<AddressingMode::Accumulator>(); break;       // ROR accumulator
        case 0x6B: arr<AddressingMode::Immediate>(); break;         // ARR immediate [Unofficial]
        case 0x6C: jmp<AddressingMode::Indirect>(); break;          // JMP indirect
        case 0x6D: adc<AddressingMode::Absolute>(); break;          // ADC absolute
        case 0x6E: ror<AddressingMode::Absolute>(); break;          // ROR absolute
        case 0x6F: rra<AddressingMode::Absolute>(); break;          // RRA absolute [Unofficial]
        case 0x70: bvs<AddressingMode::Relative>(); break;          // BVS relative
        case 0x71: adc<AddressingMode::IndirectY>(); break;         // ADC indirect Y
        case 0x72: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0x73: rra<AddressingMode::IndirectY>(); break;         // RRA indirect Y [Unofficial]
        case 0x74: nop<AddressingMode::ZeroPageIndexedX>(); break;  // NOP zero-page indexed X [Unofficial]
        case 0x75: adc<AddressingMode::ZeroPageIndexedX>(); break;  // ADC zero-page indexed X
        case 0x76: ror<AddressingMode::ZeroPageIndexedX>(); break;  // ROR zero-page indexed X
        case 0x77: rra<AddressingMode::ZeroPageIndexedX>(); break;  // RRA zero-page indexed X [Unofficial]
        case 0x78: sei<AddressingMode::Implied>(); break;           // SEI implied
        case 0x79: adc<AddressingMode::AbsoluteIndexedY>(); break;  // ADC absolute indexed Y
        case 0x7A: nop<AddressingMode::Implied>(); break;           // NOP implied [Unofficial]
        case 0x7B: rra<AddressingMode::AbsoluteIndexedY>(); break;  // RRA absolute indexed Y [Unofficial]
        case 0x7C: nop<AddressingMode::AbsoluteIndexedX>(); break;  // NOP absolute indexed X [Unofficial]
        case 0x7D: adc<AddressingMode::AbsoluteIndexedX>(); break;  // ADC absolute indexed X
        case 0x7E: ror<AddressingMode::AbsoluteIndexedX>(); break;  // ROR absolute indexed X
        case 0x7F: rra<AddressingMode::AbsoluteIndexedX>(); break;  // RRA absolute indexed X [Unofficial]
        case 0x80: nop<AddressingMode::Immediate>(); break;         // NOP immediate [Unofficial]
        case 0x81: sta<AddressingMode::IndirectX>(); break;         // STA indirect X
        case 0x82: nop<AddressingMode::Immediate>(); break;         // NOP immediate [Unofficial]
        case 0x83: sax<AddressingMode::IndirectX>(); break;         // SAX indirect X [Unofficial]
        case 0x84: sty<AddressingMode::ZeroPage>(); break;          // STY zero-page
        case 0x85: sta<AddressingMode::ZeroPage>(); break;          // STA zero-page
        case 0x86: stx<AddressingMode::ZeroPage>(); break;          // STX zero-page
        case 0x87: sax<AddressingMode::ZeroPage>(); break;          // SAX zero-page [Unofficial]
        case 0x88: dey<AddressingMode::Implied>(); break;           // DEY implied
        case 0x89: nop<AddressingMode::Immediate>(); break;         // NOP immediate [Unofficial]
        case 0x8A: txa<AddressingMode::Implied>(); break;           // TXA implied
        case 0x8B: xaa<AddressingMode::Immediate>(); break;         // XAA immediate [Unofficial]
        case 0x8C: sty<AddressingMode::Absolute>(); break;          // STY absolute
        case 0x8D: sta<AddressingMode::Absolute>(); break;          // STA absolute
        case 0x8E: stx<AddressingMode::Absolute>(); break;          // STX absolute
        case 0x8F: sax<AddressingMode::Absolute>(); break;          // SAX absolute [Unofficial]
        case 0x90: bcc<AddressingMode::Relative>(); break;          // BCC relative
        case 0x91: sta<AddressingMode::IndirectY>(); break;         // STA indirect Y
        case 0x92: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0x93: ahx<AddressingMode::IndirectY>(); break;         // AHX indirect Y [Unofficial]
        case 0x94: sty<AddressingMode::ZeroPageIndexedX>(); break;  // STY zero-page indexed X
        case 0x95: sta<AddressingMode::ZeroPageIndexedX>(); break;  // STA zero-page indexed X
        case 0x96: stx<AddressingMode::ZeroPageIndexedY>(); break;  // STX zero-page indexed Y
        case 0x97: sax<AddressingMode::ZeroPageIndexedY>(); break;  // SAX zero-page indexed Y [Unofficial]
        case 0x98: tya<AddressingMode::Implied>(); break;           // TYA implied
        case 0x99: sta<AddressingMode::AbsoluteIndexedY>(); break;  // STA absolute indexed Y
        case 0x9A: txs<AddressingMode::Implied>(); break;           // TXS implied
        case 0x9B: tas<AddressingMode::AbsoluteIndexedY>(); break;  // TAS absolute indexed Y [Unofficial]
        case 0x9C: shy<AddressingMode::AbsoluteIndexedX>(); break;  // SHY absolute indexed X [Unofficial]
        case 0x9D: sta<AddressingMode::AbsoluteIndexedX>(); break;  // STA absolute indexed X
        case 0x9E: shx<AddressingMode::AbsoluteIndexedY>(); break;  // SHX absolute indexed Y [Unofficial]
        case 0x9F: ahx<AddressingMode::AbsoluteIndexedY>(); break;  // AHX absolute indexed Y [Unofficial]
        case 0xA0: ldy<AddressingMode::Immediate>(); break;         // LDY immediate
        case 0xA1: lda<AddressingMode::IndirectX>(); break;         // LDA indirect X
        case 0xA2: ldx<AddressingMode::Immediate>(); break;         // LDX immediate
        case 0xA3: lax<AddressingMode::IndirectX>(); break;         // LAX indirect X [Unofficial]
        case 0xA4: ldy<AddressingMode::ZeroPage>(); break;          // LDY zero-page
        case 0xA5: lda<AddressingMode::ZeroPage>(); break;          // LDA zero-page
        case 0xA6: ldx<AddressingMode::ZeroPage>(); break;          // LDX zero-page
        case 0xA7: lax<AddressingMode::ZeroPage>(); break;          // LAX zero-page [Unofficial]
        case 0xA8: tay<AddressingMode::Implied>(); break;           // TAY implied
        case 0xA9: lda<AddressingMode::Immediate>(); break;         // LDA immediate
        case 0xAA: tax<AddressingMode::Implied>(); break;           // TAX implied
        case 0xAB: lxa<AddressingMode::Immediate>(); break;         // LXA immediate [Unofficial]
        case 0xAC: ldy<AddressingMode::Absolute>(); break;          // LDY absolute
        case 0xAD: lda<AddressingMode::Absolute>(); break;          // LDA absolute
        case 0xAE: ldx<AddressingMode::Absolute>(); break;          // LDX absolute
        case 0xAF: lax<AddressingMode::Absolute>(); break;          // LAX absolute [Unofficial]
        case 0xB0: bcs<AddressingMode::Relative>(); break;          // BCS relative
        case 0xB1: lda<AddressingMode::IndirectY>(); break;         // LDA indirect Y
        case 0xB2: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0xB3: lax<AddressingMode::IndirectY>(); break;         // LAX indirect Y [Unofficial]
        case 0xB4: ldy<AddressingMode::ZeroPageIndexedX>(); break;  // LDY zero-page indexed X
        case 0xB5: lda<AddressingMode::ZeroPageIndexedX>(); break;  // LDA zero-page indexed X
        case 0xB6: ldx<AddressingMode::ZeroPageIndexedY>(); break;  // LDX zero-page indexed Y
        case 0xB7: lax<AddressingMode::ZeroPageIndexedY>(); break;  // LAX zero-page indexed Y [Unofficial]
        case 0xB8: clv<AddressingMode::Implied>(); break;           // CLV implied
        case 0xB9: lda<AddressingMode::AbsoluteIndexedY>(); break;  // LDA absolute indexed Y
        case 0xBA: tsx<AddressingMode::Implied>(); break;           // TSX implied
        case 0xBB: las<AddressingMode::AbsoluteIndexedY>(); break;  // LAS absolute indexed Y [Unofficial]
        case 0xBC: ldy<AddressingMode::AbsoluteIndexedX>(); break;  // LDY absolute indexed X
        case 0xBD: lda<AddressingMode::AbsoluteIndexedX>(); break;  // LDA absolute indexed X
        case 0xBE: ldx<AddressingMode::AbsoluteIndexedY>(); break;  // LDX absolute indexed Y
        case 0xBF: lax<AddressingMode::AbsoluteIndexedY>(); break;  // LAX absolute indexed Y [Unofficial]
        case 0xC0: cpy<AddressingMode::Immediate>(); break;         // CPY immediate
        case 0xC1: cmp<AddressingMode::IndirectX>(); break;         // CMP indirect X
        case 0xC2: nop<AddressingMode::Immediate>(); break;         // NOP immediate [Unofficial]
        case 0xC3: dcp<AddressingMode::IndirectX>(); break;         // DCP indirect X [Unofficial]
        case 0xC4: cpy<AddressingMode::ZeroPage>(); break;          // CPY zero-page
        case 0xC5: cmp<AddressingMode::ZeroPage>(); break;          // CMP zero-page
        case 0xC6: dec<AddressingMode::ZeroPage>(); break;          // DEC zero-page
        case 0xC7: dcp<AddressingMode::ZeroPage>(); break;          // DCP zero-page [Unofficial]
        case 0xC8: iny<AddressingMode::Implied>(); break;           // INY implied
        case 0xC9: cmp<AddressingMode::Immediate>(); break;         // CMP immediate
        case 0xCA: dex<AddressingMode::Implied>(); break;           // DEX implied
        case 0xCB: axs<AddressingMode::Immediate>(); break;         // AXS immediate [Unofficial]
        case 0xCC: cpy<AddressingMode::Absolute>(); break;          // CPY absolute
        case 0xCD: cmp<AddressingMode::Absolute>(); break;          // CMP absolute
        case 0xCE: dec<AddressingMode::Absolute>(); break;          // DEC absolute
        case 0xCF: dcp<AddressingMode::Absolute>(); break;          // DCP absolute [Unofficial]
        case 0xD0: bne<AddressingMode::Relative>(); break;          // BNE relative
        case 0xD1: cmp<AddressingMode::IndirectY>(); break;         // CMP indirect Y
        case 0xD2: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0xD3: dcp<AddressingMode::IndirectY>(); break;         // DCP indirect Y [Unofficial]
        case 0xD4: nop<AddressingMode::ZeroPageIndexedX>(); break;  // NOP zero-page indexed X [Unofficial]
        case 0xD5: cmp<AddressingMode::ZeroPageIndexedX>(); break;  // CMP zero-page indexed X
        case 0xD6: dec<AddressingMode::ZeroPageIndexedX>(); break;  // DEC zero-page indexed X
        case 0xD7: dcp<AddressingMode::ZeroPageIndexedX>(); break;  // DCP zero-page indexed X [Unofficial]
        case 0xD8: cld<AddressingMode::Implied>(); break;           // CLD implied
        case 0xD9: cmp<AddressingMode::AbsoluteIndexedY>(); break;  // CMP absolute indexed Y
        case 0xDA: nop<AddressingMode::Implied>(); break;           // NOP implied [Unofficial]
        case 0xDB: dcp<AddressingMode::AbsoluteIndexedY>(); break;  // DCP absolute indexed Y [Unofficial]
        case 0xDC: nop<AddressingMode::AbsoluteIndexedX>(); break;  // NOP absolute indexed X [Unofficial]
        case 0xDD: cmp<AddressingMode::AbsoluteIndexedX>(); break;  // CMP absolute indexed X
        case 0xDE: dec<AddressingMode::AbsoluteIndexedX>(); break;  // DEC absolute indexed X
        case 0xDF: dcp<AddressingMode::AbsoluteIndexedX>(); break;  // DCP absolute indexed X [Unofficial]
        case 0xE0: cpx<AddressingMode::Immediate>(); break;         // CPX immediate
        case 0xE1: sbc<AddressingMode::IndirectX>(); break;         // SBC indirect X
        case 0xE2: nop<AddressingMode::Immediate>(); break;         // NOP immediate [Unofficial]
        case 0xE3: isc<AddressingMode::IndirectX>(); break;         // ISC indirect X [Unofficial]
        case 0xE4: cpx<AddressingMode::ZeroPage>(); break;          // CPX zero-page
        case 0xE5: sbc<AddressingMode::ZeroPage>(); break;          // SBC zero-page
        case 0xE6: inc<AddressingMode::ZeroPage>(); break;          // INC zero-page
        case 0xE7: isc<AddressingMode::ZeroPage>(); break;          // ISC zero-page [Unofficial]
        case 0xE8: inx<AddressingMode::Implied>(); break;           // INX implied
        case 0xE9: sbc<AddressingMode::Immediate>(); break;         // SBC immediate
        case 0xEA: nop<AddressingMode::Implied>(); break;           // NOP
        case 0xEB: sbc<AddressingMode::Immediate>(); break;         // USBC immediate [Unofficial]
        case 0xEC: cpx<AddressingMode::Absolute>(); break;          // CPX absolute
        case 0xED: sbc<AddressingMode::Absolute>(); break;          // SBC absolute
        case 0xEE: inc<AddressingMode::Absolute>(); break;          // INC absolute
        case 0xEF: isc<AddressingMode::Absolute>(); break;          // ISC absolute [Unofficial]
        case 0xF0: beq<AddressingMode::Relative>(); break;          // BEQ relative
        case 0xF1: sbc<AddressingMode::IndirectY>(); break;         // SBC indirect Y
        case 0xF2: stp<AddressingMode::Implied>(); break;           // STP [Unofficial]
        case 0xF3: isc<AddressingMode::IndirectY>(); break;         // ISC indirect Y [Unofficial]
        case 0xF4: nop<AddressingMode::ZeroPageIndexedX>(); break;  // NOP zero-page indexed X [Unofficial]
        case 0xF5: sbc<AddressingMode::ZeroPageIndexedX>(); break;  // SBC zero-page indexed X
        case 0xF6: inc<AddressingMode::ZeroPageIndexedX>(); break;  // INC zero-page indexed X
        case 0xF7: isc<AddressingMode::ZeroPageIndexedX>(); break;  // ISC zero-page indexed X [Unofficial]
        case 0xF8: sed<AddressingMode::Implied>(); break;           // SED implied
        case 0xF9: sbc<AddressingMode::AbsoluteIndexedY>(); break;  // SBC absolute indexed Y
        case 0xFA: nop<AddressingMode::Implied>(); break;           // NOP implied [Unofficial]
        case 0xFB: isc<AddressingMode::AbsoluteIndexedY>(); break;  // ISC absolute indexed Y [Unofficial]
        case 0xFC: nop<AddressingMode::AbsoluteIndexedX>(); break;  // NOP absolute indexed X [Unofficial]
        case 0xFD: sbc<AddressingMode::AbsoluteIndexedX>(); break;  // SBC absolute indexed X
        case 0xFE: inc<AddressingMode::AbsoluteIndexedX>(); break;  // INC absolute indexed X
        case 0xFF: isc<AddressingMode::AbsoluteIndexedX>(); break;  // ISC absolute indexed X [Unofficial]
    }
}

void Cpu::interrupt(InterruptType type)
{
    switch(type) {
        case InterruptType::IRQ:
            irqPending = true;
            break;
        case InterruptType::NMI:
            nmiPending = true;
            break;
        default:
            break;
    }
}

// cppcheck-suppress unusedFunction
CpuRegisters &Cpu::getRegisters()
{
    return registers;
}

void Cpu::handleInterrupt(InterruptType type)
{
    if(registers.p.interruptDisable && type == InterruptType::IRQ) {
        return;
    }

    if(type == InterruptType::BRK || type == InterruptType::RESET) {
        fetchImmedate8();
        registers.p.breakFlag = type == InterruptType::BRK;
    }

    pushIntoStack16(registers.pc);
    pushIntoStack8(registers.p);

    registers.p.interruptDisable = true;

    u16 interruptVectorLocation = 0;
    switch(type) {
        case InterruptType::NMI:
            interruptVectorLocation = 0xFFFA;
            break;
        case InterruptType::RESET:
            interruptVectorLocation = 0xFFFC;
            break;
        default: // IRQ and BRK
            interruptVectorLocation = 0xFFFE;
            break;
    }
    
    u16 interruptVector = mmu->readFromMemory(interruptVectorLocation) 
        | (mmu->readFromMemory(interruptVectorLocation + 1) << 8);
    registers.pc = interruptVector;
}

u8 Cpu::fetchImmedate8()
{
    return mmu->readFromMemory(registers.pc++);
}

u16 Cpu::fetchImmedate16()
{
    return static_cast<u16>(fetchImmedate8()) 
        + static_cast<u16>(fetchImmedate8() << 8);
}

u8 Cpu::popFromStack8()
{
    return mmu->readFromMemory(0x100 | ++registers.s);
}

void Cpu::pushIntoStack8(u8 value)
{
    mmu->writeIntoMemory(0x100 | registers.s--, value);
}

u16 Cpu::popFromStack16()
{
    u8 low = popFromStack8();
    u8 high = popFromStack8();
    return high << 8 | low;
}

void Cpu::pushIntoStack16(u16 value)
{
    pushIntoStack8(value >> 8);
    pushIntoStack8(value & 0xFF);
}

void Cpu::updateZeroFlag(auto value)
{
    registers.p.zero = value == 0;
}

void Cpu::updateNegativeFlag(auto value)
{
    registers.p.negative = (value >> 7) & 0x1;
}
