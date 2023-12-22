#include "Cpu.hpp"

Cpu::Cpu(const std::shared_ptr<Mmu> &mmu)
    : mmu(mmu)
{
    auto& a = registers.getA();
    auto& x = registers.getX();
    auto& y = registers.getY();
    auto& p = registers.getP();
    auto& sp = registers.getS();

    a = 0;
    x = 0;
    y = 0;
    p.raw = 0x24;
    sp = 0xFD;
}

u8 Cpu::fetchOpcode()
{
    return fetchImmedate8();
}

unsigned Cpu::executeInstruction(u8 opcode)
{
    switch(opcode) {
        case 0x00: break; // BRK implied
        case 0x01: ora<AddressingMode::IndirectX>(); break; // ORA indirect X
        case 0x02: break; // STP [Unofficial]
        case 0x03: break; // SLO indirect X [Unofficial]
        case 0x04: break; // NOP zero-page [Unofficial]
        case 0x05: ora<AddressingMode::ZeroPage>(); break; // ORA zero-page
        case 0x06: break; // ASL zero-page
        case 0x07: break; // SLO zero-page [Unofficial]
        case 0x08: php<AddressingMode::Implied>(); break; // PHP implied
        case 0x09: ora<AddressingMode::Immediate>(); break; // ORA immediate
        case 0x0A: break; // ASL accumulator
        case 0x0B: break; // ANC immediate [Unofficial]
        case 0x0C: break; // NOP absolute [Unofficial]
        case 0x0D: ora<AddressingMode::Absolute>(); break; // ORA absolute
        case 0x0E: break; // ASL absolute
        case 0x0F: break; // SLO absolute [Unofficial]
        case 0x10: break; // BPL relative
        case 0x11: ora<AddressingMode::IndirectY>(); break; // ORA indirect Y
        case 0x12: break; // STP [Unofficial]
        case 0x13: break; // SLO indirect Y [Unofficial]
        case 0x14: break; // NOP zero-page indexed X [Unofficial]
        case 0x15: ora<AddressingMode::ZeroPageIndexedX>(); break; // ORA zero-page indexed X
        case 0x16: break; // ASL zero-page indexed X
        case 0x17: break; // SLO zero-page indexed X [Unofficial]
        case 0x18: clc<AddressingMode::Implied>(); break; // CLC implied
        case 0x19: ora<AddressingMode::AbsoluteIndexedY>(); break; // ORA absolute indexed Y
        case 0x1A: break; // NOP implied [Unofficial]
        case 0x1B: break; // SLO absolute indexed Y [Unofficial]
        case 0x1C: break; // NOP absolute indexed X [Unofficial]
        case 0x1D: ora<AddressingMode::AbsoluteIndexedX>(); break; // ORA absolute indexed X
        case 0x1E: break; // ASL absolute indexed X
        case 0x1F: break; // SLO absolute indexed X [Unofficial]
        case 0x20: jsr<AddressingMode::Absolute>(); break; // JSR absolute
        case 0x21: _and<AddressingMode::IndirectX>(); break; // AND indirect X
        case 0x22: break; // STP [Unofficial]
        case 0x23: break; // RLA indirect X [Unofficial]
        case 0x24: break; // BIT zero-page
        case 0x25: _and<AddressingMode::ZeroPage>(); break; // AND zero-page
        case 0x26: break; // ROL zero-page
        case 0x27: break; // RLA zero-page [Unofficial]
        case 0x28: plp<AddressingMode::Implied>(); break; // PLP implied
        case 0x29: _and<AddressingMode::Immediate>(); break; // AND immediate
        case 0x2A: break; // ROL accumulator
        case 0x2B: break; // ANC immediate [Unofficial]
        case 0x2C: break; // BIT absolute
        case 0x2D: _and<AddressingMode::Absolute>(); break; // AND absolute
        case 0x2E: break; // ROL absolute
        case 0x2F: break; // RLA absolute [Unofficial]
        case 0x30: break; // BMI relative
        case 0x31: _and<AddressingMode::IndirectY>(); break; // AND indirect Y
        case 0x32: break; // STP [Unofficial]
        case 0x33: break; // RLA indirect Y [Unofficial]
        case 0x34: break; // NOP zero-page indexed X [Unofficial]
        case 0x35: _and<AddressingMode::ZeroPageIndexedX>(); break; // AND zero-page indexed X
        case 0x36: break; // ROL zero-page indexed X
        case 0x37: break; // RLA zero-page indexed X [Unofficial]
        case 0x38: sec<AddressingMode::Implied>(); break; // SEC implied
        case 0x39: _and<AddressingMode::AbsoluteIndexedY>(); break; // AND absolute indexed Y
        case 0x3A: break; // NOP implied [Unofficial]
        case 0x3B: break; // RLA absolute indexed Y [Unofficial]
        case 0x3C: break; // NOP absolute indexed X [Unofficial]
        case 0x3D: _and<AddressingMode::AbsoluteIndexedX>(); break; // AND absolute indexed X
        case 0x3E: break; // ROL absolute indexed X
        case 0x3F: break; // RLA absolute indexed X [Unofficial]
        case 0x40: break; // RTI implied
        case 0x41: eor<AddressingMode::IndirectX>(); break; // EOR indirect X
        case 0x42: break; // STP [Unofficial]
        case 0x43: break; // SRE indirect X [Unofficial]
        case 0x44: break; // NOP zero-page [Unofficial]
        case 0x45: eor<AddressingMode::ZeroPage>(); break; // EOR zero-page
        case 0x46: break; // LSR zero-page
        case 0x47: break; // SRE zero-page [Unofficial]
        case 0x48: pha<AddressingMode::Implied>(); break; // PHA implied
        case 0x49: eor<AddressingMode::Immediate>(); break; // EOR immediate
        case 0x4A: break; // LSR accumulator
        case 0x4B: break; // ALR immediate [Unofficial]
        case 0x4C: jmp<AddressingMode::Absolute>(); break; // JMP absolute
        case 0x4D: eor<AddressingMode::Absolute>(); break; // EOR absolute
        case 0x4E: break; // LSR absolute
        case 0x4F: break; // SRE absolute [Unofficial]
        case 0x50: break; // BVC relative
        case 0x51: eor<AddressingMode::IndirectY>(); break; // EOR indirect Y
        case 0x52: break; // STP [Unofficial]
        case 0x53: break; // SRE indirect Y [Unofficial]
        case 0x54: break; // NOP zero-page indexed X [Unofficial]
        case 0x55: eor<AddressingMode::ZeroPageIndexedX>(); break; // EOR zero-page indexed X
        case 0x56: break; // LSR zero-page indexed X
        case 0x57: break; // SRE zero-page indexed X [Unofficial]
        case 0x58: cli<AddressingMode::Implied>(); break; // CLI implied
        case 0x59: eor<AddressingMode::AbsoluteIndexedY>(); break; // EOR absolute indexed Y
        case 0x5A: break; // NOP implied [Unofficial]
        case 0x5B: break; // SRE absolute indexed Y [Unofficial]
        case 0x5C: break; // NOP absolute indexed X [Unofficial]
        case 0x5D: eor<AddressingMode::AbsoluteIndexedX>(); break; // EOR absolute indexed X
        case 0x5E: break; // LSR absolute indexed X
        case 0x5F: break; // SRE absolute indexed X [Unofficial]
        case 0x60: rts<AddressingMode::Implied>(); break; // RTS implied
        case 0x61: adc<AddressingMode::IndirectX>(); break; // ADC indirect X
        case 0x62: break; // STP [Unofficial]
        case 0x63: break; // RRA indirect X [Unofficial]
        case 0x64: break; // NOP zero-page [Unofficial]
        case 0x65: adc<AddressingMode::ZeroPage>(); break; // ADC zero-page
        case 0x66: break; // ROR zero-page
        case 0x67: break; // RRA zero-page [Unofficial]
        case 0x68: pla<AddressingMode::Implied>(); break; // PLA implied
        case 0x69: adc<AddressingMode::Immediate>(); break; // ADC immediate
        case 0x6A: break; // ROR accumulator
        case 0x6B: break; // ARR immediate [Unofficial]
        case 0x6C: jmp<AddressingMode::Indirect>(); break; // JMP indirect
        case 0x6D: adc<AddressingMode::Absolute>(); break; // ADC absolute
        case 0x6E: break; // ROR absolute
        case 0x6F: break; // RRA absolute [Unofficial]
        case 0x70: break; // BVS relative
        case 0x71: adc<AddressingMode::IndirectY>(); break; // ADC indirect Y
        case 0x72: break; // STP [Unofficial]
        case 0x73: break; // RRA indirect Y [Unofficial]
        case 0x74: break; // NOP zero-page indexed X [Unofficial]
        case 0x75: adc<AddressingMode::ZeroPageIndexedX>(); break; // ADC zero-page indexed X
        case 0x76: break; // ROR zero-page indexed X
        case 0x77: break; // RRA zero-page indexed X [Unofficial]
        case 0x78: sei<AddressingMode::Implied>(); break; // SEI implied
        case 0x79: adc<AddressingMode::AbsoluteIndexedY>(); break; // ADC absolute indexed Y
        case 0x7A: break; // NOP implied [Unofficial]
        case 0x7B: break; // RRA absolute indexed Y [Unofficial]
        case 0x7C: break; // NOP absolute indexed X [Unofficial]
        case 0x7D: adc<AddressingMode::AbsoluteIndexedX>(); break; // ADC absolute indexed X
        case 0x7E: break; // ROR absolute indexed X
        case 0x7F: break; // RRA absolute indexed X [Unofficial]
        case 0x80: break; // NOP immediate [Unofficial]
        case 0x81: sta<AddressingMode::IndirectX>(); break; // STA indirect X
        case 0x82: break; // NOP immediate [Unofficial]
        case 0x83: break; // SAX indirect [Unofficial]
        case 0x84: sty<AddressingMode::ZeroPage>(); break; // STY zero-page
        case 0x85: sta<AddressingMode::ZeroPage>(); break; // STA zero-page
        case 0x86: stx<AddressingMode::ZeroPage>(); break; // STX zero-page
        case 0x87: break; // SAX zero-page [Unofficial]
        case 0x88: dey<AddressingMode::Implied>(); break; // DEY implied
        case 0x89: break; // NOP immediate [Unofficial]
        case 0x8A: txa<AddressingMode::Implied>(); break; // TXA implied
        case 0x8B: break; // XAA immediate [Unofficial]
        case 0x8C: sty<AddressingMode::Absolute>(); break; // STY absolute
        case 0x8D: sta<AddressingMode::Absolute>(); break; // STA absolute
        case 0x8E: stx<AddressingMode::Absolute>(); break; // STX absolute
        case 0x8F: break; // SAX absolute [Unofficial]
        case 0x90: break; // BCC relative
        case 0x91: sta<AddressingMode::IndirectY>(); break; // STA indirect Y
        case 0x92: break; // STP [Unofficial]
        case 0x93: break; // AHX indirect Y
        case 0x94: sty<AddressingMode::ZeroPageIndexedX>(); break; // STY zero-page indexed X
        case 0x95: sta<AddressingMode::ZeroPageIndexedX>(); break; // STA zero-page indexed X
        case 0x96: stx<AddressingMode::ZeroPageIndexedY>(); break; // STX zero-page indexed Y
        case 0x97: break; // SAX zero-page indexed Y [Unofficial]
        case 0x98: tya<AddressingMode::Implied>(); break; // TYA implied
        case 0x99: sta<AddressingMode::AbsoluteIndexedY>(); break; // STA absolute indexed Y
        case 0x9A: txs<AddressingMode::Implied>(); break; // TXS implied
        case 0x9B: break; // TAS absolute indexed Y [Unofficial]
        case 0x9C: break; // SHY absolute indexed X [Unofficial]
        case 0x9D: sta<AddressingMode::AbsoluteIndexedX>(); break; // STA absolute indexed X
        case 0x9E: break; // SHX absolute indexed Y [Unofficial]
        case 0x9F: break; // AHX absolute indexed Y
        case 0xA0: ldy<AddressingMode::Immediate>(); break; // LDY immediate
        case 0xA1: lda<AddressingMode::IndirectX>(); break; // LDA indirect X
        case 0xA2: ldx<AddressingMode::Immediate>(); break; // LDX immediate
        case 0xA3: break; // LAX indirect X [Unofficial]
        case 0xA4: ldy<AddressingMode::ZeroPage>(); break; // LDY zero-page
        case 0xA5: lda<AddressingMode::ZeroPage>(); break; // LDA zero-page
        case 0xA6: ldx<AddressingMode::ZeroPage>(); break; // LDX zero-page
        case 0xA7: break; // LAX zero-page [Unofficial]
        case 0xA8: tay<AddressingMode::Implied>(); break; // TAY implied
        case 0xA9: lda<AddressingMode::Immediate>(); break; // LDA immediate
        case 0xAA: tax<AddressingMode::Implied>(); break; // TAX implied
        case 0xAB: break; // LAX immediate [Unofficial]
        case 0xAC: ldy<AddressingMode::Absolute>(); break; // LDY absolute
        case 0xAD: lda<AddressingMode::Absolute>(); break; // LDA absolute
        case 0xAE: ldx<AddressingMode::Absolute>(); break; // LDX absolute
        case 0xAF: break; // LAX absolute [Unofficial]
        case 0xB0: break; // BCS relative
        case 0xB1: lda<AddressingMode::IndirectY>(); break; // LDA indirect Y
        case 0xB2: break; // STP [Unofficial]
        case 0xB3: break; // LAX indirect Y [Unofficial]
        case 0xB4: ldy<AddressingMode::ZeroPageIndexedX>(); break; // LDY zero-page indexed X
        case 0xB5: lda<AddressingMode::ZeroPageIndexedX>(); break; // LDA zero-page indexed X
        case 0xB6: ldx<AddressingMode::ZeroPageIndexedY>(); break; // LDX zero-page indexed Y
        case 0xB7: break; // LAX zero-page indexed Y [Unofficial]
        case 0xB8: clv<AddressingMode::Implied>(); break; // CLV implied
        case 0xB9: lda<AddressingMode::ZeroPageIndexedY>(); break; // LDA absolute indexed Y
        case 0xBA: tsx<AddressingMode::Implied>(); break; // TSX implied
        case 0xBB: break; // LAS absolute indexed Y [Unofficial]
        case 0xBC: ldy<AddressingMode::AbsoluteIndexedX>(); break; // LDY absolute indexed X
        case 0xBD: lda<AddressingMode::AbsoluteIndexedX>(); break; // LDA absolute indexed X
        case 0xBE: ldx<AddressingMode::AbsoluteIndexedY>(); break; // LDX absolute indexed Y
        case 0xBF: break; // LAX absolute indexed Y [Unofficial]
        case 0xC0: break; // CPY immediate
        case 0xC1: break; // CMP indirect X
        case 0xC2: break; // NOP immediate [Unofficial]
        case 0xC3: break; // DCP indirect X [Unofficial]
        case 0xC4: break; // CPY zero-page
        case 0xC5: break; // CMP zero-page
        case 0xC6: dec<AddressingMode::ZeroPage>(); break; // DEC zero-page
        case 0xC7: break; // DCP zero-page [Unofficial]
        case 0xC8: iny<AddressingMode::Implied>(); break; // INY implied
        case 0xC9: break; // CMP immediate
        case 0xCA: dex<AddressingMode::Implied>(); break; // DEX implied
        case 0xCB: break; // AXS immediate
        case 0xCC: break; // CPY absolute
        case 0xCD: break; // CMP absolute
        case 0xCE: dec<AddressingMode::Absolute>(); break; // DEC absolute
        case 0xCF: break; // DCP absolute [Unofficial]
        case 0xD0: break; // BNE relative
        case 0xD1: break; // CMP indirect Y
        case 0xD2: break; // STP [Unofficial]
        case 0xD3: break; // DCP indirect Y [Unofficial]
        case 0xD4: break; // NOP zero-page indexed X [Unofficial]
        case 0xD5: break; // CMP zero-page indexed X
        case 0xD6: dec<AddressingMode::ZeroPageIndexedX>(); break; // DEC zero-page indexed X
        case 0xD7: break; // DCP zero-page indexed X [Unofficial]
        case 0xD8: cld<AddressingMode::Implied>(); break; // CLD implied
        case 0xD9: break; // CMP absolute indexed Y
        case 0xDA: break; // NOP implied [Unofficial]
        case 0xDB: break; // DCP absolute indexed Y [Unofficial]
        case 0xDC: break; // NOP absolute indexed X [Unofficial]
        case 0xDD: break; // CMP absolute indexed X
        case 0xDE: dec<AddressingMode::AbsoluteIndexedX>(); break; // DEC absolute indexed X
        case 0xDF: break; // DCP absolute indexed X [Unofficial]
        case 0xE0: break; // CPX immediate
        case 0xE1: sbc<AddressingMode::IndirectX>(); break; // SBC indirect X
        case 0xE2: break; // NOP immediate [Unofficial]
        case 0xE3: break; // ISC indirect X [Unofficial]
        case 0xE4: break; // CPX zero-page
        case 0xE5: sbc<AddressingMode::ZeroPage>(); break; // SBC zero-page
        case 0xE6: inc<AddressingMode::ZeroPage>(); break; // INC zero-page
        case 0xE7: break; // ISC zero-page [Unofficial]
        case 0xE8: inx<AddressingMode::Implied>(); break; // INX implied
        case 0xE9: sbc<AddressingMode::Immediate>(); break; // SBC immediate
        case 0xEA: break; // NOP
        case 0xEB: break; // USBC immediate [Unofficial]
        case 0xEC: break; // CPX absolute
        case 0xED: sbc<AddressingMode::Absolute>(); break; // SBC absolute
        case 0xEE: inc<AddressingMode::Absolute>(); break; // INC absolute
        case 0xEF: break; // ISC absolute [Unofficial]
        case 0xF0: break; // BEQ relative
        case 0xF1: sbc<AddressingMode::IndirectY>(); break; // SBC indirect Y
        case 0xF2: break; // STP [Unofficial]
        case 0xF3: break; // ISC indirect Y [Unofficial]
        case 0xF4: break; // NOP zero-page indexed X [Unofficial]
        case 0xF5: sbc<AddressingMode::ZeroPageIndexedX>(); break; // SBC zero-page indexed X
        case 0xF6: inc<AddressingMode::ZeroPageIndexedX>(); break; // INC zero-page indexed X
        case 0xF7: break; // ISC zero-page indexed X [Unofficial]
        case 0xF8: sed<AddressingMode::Implied>(); break; // SED implied
        case 0xF9: sbc<AddressingMode::AbsoluteIndexedY>(); break; // SBC absolute indexed Y
        case 0xFA: break; // NOP implied [Unofficial]
        case 0xFB: break; // ISC absolute indexed Y [Unofficial]
        case 0xFC: break; // NOP absolute indexed X [Unofficial]
        case 0xFD: sbc<AddressingMode::AbsoluteIndexedX>(); break; // SBC absolute indexed X
        case 0xFE: inc<AddressingMode::AbsoluteIndexedX>(); break; // INC absolute indexed X
        case 0xFF: break; // ISC absolute indexed X [Unofficial]
    }

    return 0;
}

CpuRegisters &Cpu::getRegisters()
{
    return registers;
}

u8 Cpu::fetchImmedate8()
{
    auto& pc = registers.getPc();
    return mmu->readFromMemory(pc++);
}

u16 Cpu::fetchImmedate16()
{
    return static_cast<u16>(fetchImmedate8()) 
        + static_cast<u16>(fetchImmedate8() << 8);
}

u8 Cpu::popFromStack8()
{
    auto& s = registers.getS();
    return mmu->readFromMemory(0x100 | ++s);
}

void Cpu::pushIntoStack8(u8 value)
{
    auto& s = registers.getS();
    mmu->writeIntoMemory(0x100 | s--, value);
}

u16 Cpu::popFromStack16()
{
    u8 high = popFromStack8();
    u8 low = popFromStack8();
    return high << 8 | low;
}

void Cpu::pushIntoStack16(u16 value)
{
    pushIntoStack8(value & 0xFF);
    pushIntoStack8(value >> 8);
}
