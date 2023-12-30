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
        case 0x01: return ora<AddressingMode::IndirectX>(); // ORA indirect X
        case 0x02: break; // STP [Unofficial]
        case 0x03: break; // SLO indirect X [Unofficial]
        case 0x04: break; // NOP zero-page [Unofficial]
        case 0x05: return ora<AddressingMode::ZeroPage>(); // ORA zero-page
        case 0x06: break; // ASL zero-page
        case 0x07: break; // SLO zero-page [Unofficial]
        case 0x08: return php<AddressingMode::Implied>(); // PHP implied
        case 0x09: return ora<AddressingMode::Immediate>(); // ORA immediate
        case 0x0A: break; // ASL accumulator
        case 0x0B: break; // ANC immediate [Unofficial]
        case 0x0C: break; // NOP absolute [Unofficial]
        case 0x0D: return ora<AddressingMode::Absolute>(); // ORA absolute
        case 0x0E: break; // ASL absolute
        case 0x0F: break; // SLO absolute [Unofficial]
        case 0x10: break; // BPL relative
        case 0x11: return ora<AddressingMode::IndirectY>(); // ORA indirect Y
        case 0x12: break; // STP [Unofficial]
        case 0x13: break; // SLO indirect Y [Unofficial]
        case 0x14: break; // NOP zero-page indexed X [Unofficial]
        case 0x15: return ora<AddressingMode::ZeroPageIndexedX>(); // ORA zero-page indexed X
        case 0x16: break; // ASL zero-page indexed X
        case 0x17: break; // SLO zero-page indexed X [Unofficial]
        case 0x18: return clc<AddressingMode::Implied>(); // CLC implied
        case 0x19: return ora<AddressingMode::AbsoluteIndexedY>(); // ORA absolute indexed Y
        case 0x1A: break; // NOP implied [Unofficial]
        case 0x1B: break; // SLO absolute indexed Y [Unofficial]
        case 0x1C: break; // NOP absolute indexed X [Unofficial]
        case 0x1D: return ora<AddressingMode::AbsoluteIndexedX>(); // ORA absolute indexed X
        case 0x1E: break; // ASL absolute indexed X
        case 0x1F: break; // SLO absolute indexed X [Unofficial]
        case 0x20: return jsr<AddressingMode::Absolute>(); // JSR absolute
        case 0x21: return _and<AddressingMode::IndirectX>(); // AND indirect X
        case 0x22: break; // STP [Unofficial]
        case 0x23: break; // RLA indirect X [Unofficial]
        case 0x24: break; // BIT zero-page
        case 0x25: return _and<AddressingMode::ZeroPage>(); // AND zero-page
        case 0x26: break; // ROL zero-page
        case 0x27: break; // RLA zero-page [Unofficial]
        case 0x28: return plp<AddressingMode::Implied>(); // PLP implied
        case 0x29: return _and<AddressingMode::Immediate>(); // AND immediate
        case 0x2A: break; // ROL accumulator
        case 0x2B: break; // ANC immediate [Unofficial]
        case 0x2C: break; // BIT absolute
        case 0x2D: return _and<AddressingMode::Absolute>(); // AND absolute
        case 0x2E: break; // ROL absolute
        case 0x2F: break; // RLA absolute [Unofficial]
        case 0x30: break; // BMI relative
        case 0x31: return _and<AddressingMode::IndirectY>(); // AND indirect Y
        case 0x32: break; // STP [Unofficial]
        case 0x33: break; // RLA indirect Y [Unofficial]
        case 0x34: break; // NOP zero-page indexed X [Unofficial]
        case 0x35: return _and<AddressingMode::ZeroPageIndexedX>(); // AND zero-page indexed X
        case 0x36: break; // ROL zero-page indexed X
        case 0x37: break; // RLA zero-page indexed X [Unofficial]
        case 0x38: return sec<AddressingMode::Implied>(); // SEC implied
        case 0x39: return _and<AddressingMode::AbsoluteIndexedY>(); // AND absolute indexed Y
        case 0x3A: break; // NOP implied [Unofficial]
        case 0x3B: break; // RLA absolute indexed Y [Unofficial]
        case 0x3C: break; // NOP absolute indexed X [Unofficial]
        case 0x3D: return _and<AddressingMode::AbsoluteIndexedX>(); // AND absolute indexed X
        case 0x3E: break; // ROL absolute indexed X
        case 0x3F: break; // RLA absolute indexed X [Unofficial]
        case 0x40: break; // RTI implied
        case 0x41: return eor<AddressingMode::IndirectX>(); // EOR indirect X
        case 0x42: break; // STP [Unofficial]
        case 0x43: break; // SRE indirect X [Unofficial]
        case 0x44: break; // NOP zero-page [Unofficial]
        case 0x45: return eor<AddressingMode::ZeroPage>(); // EOR zero-page
        case 0x46: break; // LSR zero-page
        case 0x47: break; // SRE zero-page [Unofficial]
        case 0x48: return pha<AddressingMode::Implied>(); // PHA implied
        case 0x49: return eor<AddressingMode::Immediate>(); // EOR immediate
        case 0x4A: break; // LSR accumulator
        case 0x4B: break; // ALR immediate [Unofficial]
        case 0x4C: return jmp<AddressingMode::Absolute>(); // JMP absolute
        case 0x4D: return eor<AddressingMode::Absolute>(); // EOR absolute
        case 0x4E: break; // LSR absolute
        case 0x4F: break; // SRE absolute [Unofficial]
        case 0x50: break; // BVC relative
        case 0x51: return eor<AddressingMode::IndirectY>(); // EOR indirect Y
        case 0x52: break; // STP [Unofficial]
        case 0x53: break; // SRE indirect Y [Unofficial]
        case 0x54: break; // NOP zero-page indexed X [Unofficial]
        case 0x55: return eor<AddressingMode::ZeroPageIndexedX>(); // EOR zero-page indexed X
        case 0x56: break; // LSR zero-page indexed X
        case 0x57: break; // SRE zero-page indexed X [Unofficial]
        case 0x58: return cli<AddressingMode::Implied>(); // CLI implied
        case 0x59: return eor<AddressingMode::AbsoluteIndexedY>(); // EOR absolute indexed Y
        case 0x5A: break; // NOP implied [Unofficial]
        case 0x5B: break; // SRE absolute indexed Y [Unofficial]
        case 0x5C: break; // NOP absolute indexed X [Unofficial]
        case 0x5D: return eor<AddressingMode::AbsoluteIndexedX>(); // EOR absolute indexed X
        case 0x5E: break; // LSR absolute indexed X
        case 0x5F: break; // SRE absolute indexed X [Unofficial]
        case 0x60: return rts<AddressingMode::Implied>(); // RTS implied
        case 0x61: return adc<AddressingMode::IndirectX>(); // ADC indirect X
        case 0x62: break; // STP [Unofficial]
        case 0x63: break; // RRA indirect X [Unofficial]
        case 0x64: break; // NOP zero-page [Unofficial]
        case 0x65: return adc<AddressingMode::ZeroPage>(); // ADC zero-page
        case 0x66: break; // ROR zero-page
        case 0x67: break; // RRA zero-page [Unofficial]
        case 0x68: return pla<AddressingMode::Implied>(); // PLA implied
        case 0x69: return adc<AddressingMode::Immediate>(); // ADC immediate
        case 0x6A: break; // ROR accumulator
        case 0x6B: break; // ARR immediate [Unofficial]
        case 0x6C: return jmp<AddressingMode::Indirect>(); // JMP indirect
        case 0x6D: return adc<AddressingMode::Absolute>(); // ADC absolute
        case 0x6E: break; // ROR absolute
        case 0x6F: break; // RRA absolute [Unofficial]
        case 0x70: break; // BVS relative
        case 0x71: return adc<AddressingMode::IndirectY>(); // ADC indirect Y
        case 0x72: break; // STP [Unofficial]
        case 0x73: break; // RRA indirect Y [Unofficial]
        case 0x74: break; // NOP zero-page indexed X [Unofficial]
        case 0x75: return adc<AddressingMode::ZeroPageIndexedX>(); // ADC zero-page indexed X
        case 0x76: break; // ROR zero-page indexed X
        case 0x77: break; // RRA zero-page indexed X [Unofficial]
        case 0x78: return sei<AddressingMode::Implied>(); // SEI implied
        case 0x79: return adc<AddressingMode::AbsoluteIndexedY>(); // ADC absolute indexed Y
        case 0x7A: break; // NOP implied [Unofficial]
        case 0x7B: break; // RRA absolute indexed Y [Unofficial]
        case 0x7C: break; // NOP absolute indexed X [Unofficial]
        case 0x7D: return adc<AddressingMode::AbsoluteIndexedX>(); // ADC absolute indexed X
        case 0x7E: break; // ROR absolute indexed X
        case 0x7F: break; // RRA absolute indexed X [Unofficial]
        case 0x80: break; // NOP immediate [Unofficial]
        case 0x81: return sta<AddressingMode::IndirectX>(); // STA indirect X
        case 0x82: break; // NOP immediate [Unofficial]
        case 0x83: break; // SAX indirect [Unofficial]
        case 0x84: return sty<AddressingMode::ZeroPage>(); // STY zero-page
        case 0x85: return sta<AddressingMode::ZeroPage>(); // STA zero-page
        case 0x86: return stx<AddressingMode::ZeroPage>(); // STX zero-page
        case 0x87: break; // SAX zero-page [Unofficial]
        case 0x88: return dey<AddressingMode::Implied>(); // DEY implied
        case 0x89: break; // NOP immediate [Unofficial]
        case 0x8A: return txa<AddressingMode::Implied>(); // TXA implied
        case 0x8B: break; // XAA immediate [Unofficial]
        case 0x8C: return sty<AddressingMode::Absolute>(); // STY absolute
        case 0x8D: return sta<AddressingMode::Absolute>(); // STA absolute
        case 0x8E: return stx<AddressingMode::Absolute>(); // STX absolute
        case 0x8F: break; // SAX absolute [Unofficial]
        case 0x90: break; // BCC relative
        case 0x91: return sta<AddressingMode::IndirectY>(); // STA indirect Y
        case 0x92: break; // STP [Unofficial]
        case 0x93: break; // AHX indirect Y
        case 0x94: return sty<AddressingMode::ZeroPageIndexedX>(); // STY zero-page indexed X
        case 0x95: return sta<AddressingMode::ZeroPageIndexedX>(); // STA zero-page indexed X
        case 0x96: return stx<AddressingMode::ZeroPageIndexedY>(); // STX zero-page indexed Y
        case 0x97: break; // SAX zero-page indexed Y [Unofficial]
        case 0x98: return tya<AddressingMode::Implied>(); // TYA implied
        case 0x99: return sta<AddressingMode::AbsoluteIndexedY>(); // STA absolute indexed Y
        case 0x9A: return txs<AddressingMode::Implied>(); // TXS implied
        case 0x9B: break; // TAS absolute indexed Y [Unofficial]
        case 0x9C: break; // SHY absolute indexed X [Unofficial]
        case 0x9D: return sta<AddressingMode::AbsoluteIndexedX>(); // STA absolute indexed X
        case 0x9E: break; // SHX absolute indexed Y [Unofficial]
        case 0x9F: break; // AHX absolute indexed Y
        case 0xA0: return ldy<AddressingMode::Immediate>(); // LDY immediate
        case 0xA1: return lda<AddressingMode::IndirectX>(); // LDA indirect X
        case 0xA2: return ldx<AddressingMode::Immediate>(); // LDX immediate
        case 0xA3: break; // LAX indirect X [Unofficial]
        case 0xA4: return ldy<AddressingMode::ZeroPage>(); // LDY zero-page
        case 0xA5: return lda<AddressingMode::ZeroPage>(); // LDA zero-page
        case 0xA6: return ldx<AddressingMode::ZeroPage>(); // LDX zero-page
        case 0xA7: break; // LAX zero-page [Unofficial]
        case 0xA8: return tay<AddressingMode::Implied>(); // TAY implied
        case 0xA9: return lda<AddressingMode::Immediate>(); // LDA immediate
        case 0xAA: return tax<AddressingMode::Implied>(); // TAX implied
        case 0xAB: break; // LAX immediate [Unofficial]
        case 0xAC: return ldy<AddressingMode::Absolute>(); // LDY absolute
        case 0xAD: return lda<AddressingMode::Absolute>(); // LDA absolute
        case 0xAE: return ldx<AddressingMode::Absolute>(); // LDX absolute
        case 0xAF: break; // LAX absolute [Unofficial]
        case 0xB0: break; // BCS relative
        case 0xB1: return lda<AddressingMode::IndirectY>(); // LDA indirect Y
        case 0xB2: break; // STP [Unofficial]
        case 0xB3: break; // LAX indirect Y [Unofficial]
        case 0xB4: return ldy<AddressingMode::ZeroPageIndexedX>(); // LDY zero-page indexed X
        case 0xB5: return lda<AddressingMode::ZeroPageIndexedX>(); // LDA zero-page indexed X
        case 0xB6: return ldx<AddressingMode::ZeroPageIndexedY>(); // LDX zero-page indexed Y
        case 0xB7: break; // LAX zero-page indexed Y [Unofficial]
        case 0xB8: return clv<AddressingMode::Implied>(); // CLV implied
        case 0xB9: return lda<AddressingMode::ZeroPageIndexedY>(); // LDA absolute indexed Y
        case 0xBA: return tsx<AddressingMode::Implied>(); // TSX implied
        case 0xBB: break; // LAS absolute indexed Y [Unofficial]
        case 0xBC: return ldy<AddressingMode::AbsoluteIndexedX>(); // LDY absolute indexed X
        case 0xBD: return lda<AddressingMode::AbsoluteIndexedX>(); // LDA absolute indexed X
        case 0xBE: return ldx<AddressingMode::AbsoluteIndexedY>(); // LDX absolute indexed Y
        case 0xBF: break; // LAX absolute indexed Y [Unofficial]
        case 0xC0: break; // CPY immediate
        case 0xC1: break; // CMP indirect X
        case 0xC2: break; // NOP immediate [Unofficial]
        case 0xC3: break; // DCP indirect X [Unofficial]
        case 0xC4: break; // CPY zero-page
        case 0xC5: break; // CMP zero-page
        case 0xC6: return dec<AddressingMode::ZeroPage>(); // DEC zero-page
        case 0xC7: break; // DCP zero-page [Unofficial]
        case 0xC8: return iny<AddressingMode::Implied>(); // INY implied
        case 0xC9: break; // CMP immediate
        case 0xCA: return dex<AddressingMode::Implied>(); // DEX implied
        case 0xCB: break; // AXS immediate
        case 0xCC: break; // CPY absolute
        case 0xCD: break; // CMP absolute
        case 0xCE: return dec<AddressingMode::Absolute>(); // DEC absolute
        case 0xCF: break; // DCP absolute [Unofficial]
        case 0xD0: break; // BNE relative
        case 0xD1: break; // CMP indirect Y
        case 0xD2: break; // STP [Unofficial]
        case 0xD3: break; // DCP indirect Y [Unofficial]
        case 0xD4: break; // NOP zero-page indexed X [Unofficial]
        case 0xD5: break; // CMP zero-page indexed X
        case 0xD6: return dec<AddressingMode::ZeroPageIndexedX>(); // DEC zero-page indexed X
        case 0xD7: break; // DCP zero-page indexed X [Unofficial]
        case 0xD8: return cld<AddressingMode::Implied>(); // CLD implied
        case 0xD9: break; // CMP absolute indexed Y
        case 0xDA: break; // NOP implied [Unofficial]
        case 0xDB: break; // DCP absolute indexed Y [Unofficial]
        case 0xDC: break; // NOP absolute indexed X [Unofficial]
        case 0xDD: break; // CMP absolute indexed X
        case 0xDE: return dec<AddressingMode::AbsoluteIndexedX>(); // DEC absolute indexed X
        case 0xDF: break; // DCP absolute indexed X [Unofficial]
        case 0xE0: break; // CPX immediate
        case 0xE1: return sbc<AddressingMode::IndirectX>(); // SBC indirect X
        case 0xE2: break; // NOP immediate [Unofficial]
        case 0xE3: break; // ISC indirect X [Unofficial]
        case 0xE4: break; // CPX zero-page
        case 0xE5: return sbc<AddressingMode::ZeroPage>(); // SBC zero-page
        case 0xE6: return inc<AddressingMode::ZeroPage>(); // INC zero-page
        case 0xE7: break; // ISC zero-page [Unofficial]
        case 0xE8: return inx<AddressingMode::Implied>(); // INX implied
        case 0xE9: return sbc<AddressingMode::Immediate>(); // SBC immediate
        case 0xEA: break; // NOP
        case 0xEB: break; // USBC immediate [Unofficial]
        case 0xEC: break; // CPX absolute
        case 0xED: return sbc<AddressingMode::Absolute>(); // SBC absolute
        case 0xEE: return inc<AddressingMode::Absolute>(); // INC absolute
        case 0xEF: break; // ISC absolute [Unofficial]
        case 0xF0: break; // BEQ relative
        case 0xF1: return sbc<AddressingMode::IndirectY>(); // SBC indirect Y
        case 0xF2: break; // STP [Unofficial]
        case 0xF3: break; // ISC indirect Y [Unofficial]
        case 0xF4: break; // NOP zero-page indexed X [Unofficial]
        case 0xF5: return sbc<AddressingMode::ZeroPageIndexedX>(); // SBC zero-page indexed X
        case 0xF6: return inc<AddressingMode::ZeroPageIndexedX>(); // INC zero-page indexed X
        case 0xF7: break; // ISC zero-page indexed X [Unofficial]
        case 0xF8: return sed<AddressingMode::Implied>(); // SED implied
        case 0xF9: return sbc<AddressingMode::AbsoluteIndexedY>(); // SBC absolute indexed Y
        case 0xFA: break; // NOP implied [Unofficial]
        case 0xFB: break; // ISC absolute indexed Y [Unofficial]
        case 0xFC: break; // NOP absolute indexed X [Unofficial]
        case 0xFD: return sbc<AddressingMode::AbsoluteIndexedX>(); // SBC absolute indexed X
        case 0xFE: return inc<AddressingMode::AbsoluteIndexedX>(); // INC absolute indexed X
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

void Cpu::updateZeroFlag(auto value)
{
    auto& flags = registers.getP();
    flags.zero = value == 0;
}

void Cpu::updateNegativeFlag(auto value)
{
    auto& flags = registers.getP();
    flags.negative = (value >> 7) & 0x1;
}

void Cpu::updateOverflowFlag(auto valueBefore, auto valueAfter)
{
    auto& flags = registers.getP();
    flags.overflow = valueBefore <= 0x7F && valueAfter > 0x7F;
}

void Cpu::updateCarryFlag(auto value)
{
    auto& flags = registers.getP();
    flags.carry = (value >> 8) & 0x1;
}

bool Cpu::checkForPageCross(u16 oldAddress, u16 newAddress)
{
    return (oldAddress & 0xFF00) != (newAddress & 0xFF00);
}
