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

u8 Cpu::fetchOpcode()
{
    return fetchImmedate8();
}

unsigned Cpu::executeInstruction(u8 opcode)
{
    if(halted) {
        return 0;
    }

    switch(opcode) {
        case 0x00: return brk<AddressingMode::Implied>();           // BRK implied
        case 0x01: return ora<AddressingMode::IndirectX>();         // ORA indirect X
        case 0x02: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0x03: return slo<AddressingMode::IndirectX>();         // SLO indirect X [Unofficial]
        case 0x04: return nop<AddressingMode::ZeroPage>();          // NOP zero-page [Unofficial]
        case 0x05: return ora<AddressingMode::ZeroPage>();          // ORA zero-page
        case 0x06: return asl<AddressingMode::ZeroPage>();          // ASL zero-page
        case 0x07: return slo<AddressingMode::ZeroPage>();          // SLO zero-page [Unofficial]
        case 0x08: return php<AddressingMode::Implied>();           // PHP implied
        case 0x09: return ora<AddressingMode::Immediate>();         // ORA immediate
        case 0x0A: return asl<AddressingMode::Accumulator>();       // ASL accumulator
        case 0x0B: return anc<AddressingMode::Immediate>();         // ANC immediate [Unofficial]
        case 0x0C: return nop<AddressingMode::Absolute>();          // NOP absolute [Unofficial]
        case 0x0D: return ora<AddressingMode::Absolute>();          // ORA absolute
        case 0x0E: return asl<AddressingMode::Absolute>();          // ASL absolute
        case 0x0F: return slo<AddressingMode::Absolute>();          // SLO absolute [Unofficial]
        case 0x10: return bpl<AddressingMode::Relative>();          // BPL relative
        case 0x11: return ora<AddressingMode::IndirectY>();         // ORA indirect Y
        case 0x12: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0x13: return slo<AddressingMode::IndirectY>();         // SLO indirect Y [Unofficial]
        case 0x14: return nop<AddressingMode::ZeroPageIndexedX>();  // NOP zero-page indexed X [Unofficial]
        case 0x15: return ora<AddressingMode::ZeroPageIndexedX>();  // ORA zero-page indexed X
        case 0x16: return asl<AddressingMode::ZeroPageIndexedX>();  // ASL zero-page indexed X
        case 0x17: return slo<AddressingMode::ZeroPageIndexedX>();  // SLO zero-page indexed X [Unofficial]
        case 0x18: return clc<AddressingMode::Implied>();           // CLC implied
        case 0x19: return ora<AddressingMode::AbsoluteIndexedY>();  // ORA absolute indexed Y
        case 0x1A: return nop<AddressingMode::Implied>();           // NOP implied [Unofficial]
        case 0x1B: return slo<AddressingMode::AbsoluteIndexedY>();  // SLO absolute indexed Y [Unofficial]
        case 0x1C: return nop<AddressingMode::AbsoluteIndexedX>();  // NOP absolute indexed X [Unofficial]
        case 0x1D: return ora<AddressingMode::AbsoluteIndexedX>();  // ORA absolute indexed X
        case 0x1E: return asl<AddressingMode::AbsoluteIndexedX>();  // ASL absolute indexed X
        case 0x1F: return slo<AddressingMode::AbsoluteIndexedX>();  // SLO absolute indexed X [Unofficial]
        case 0x20: return jsr<AddressingMode::Absolute>();          // JSR absolute
        case 0x21: return _and<AddressingMode::IndirectX>();        // AND indirect X
        case 0x22: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0x23: return rla<AddressingMode::IndirectX>();         // RLA indirect X [Unofficial]
        case 0x24: return bit<AddressingMode::ZeroPage>();          // BIT zero-page
        case 0x25: return _and<AddressingMode::ZeroPage>();         // AND zero-page
        case 0x26: return rol<AddressingMode::ZeroPage>();          // ROL zero-page
        case 0x27: return rla<AddressingMode::ZeroPage>();          // RLA zero-page [Unofficial]
        case 0x28: return plp<AddressingMode::Implied>();           // PLP implied
        case 0x29: return _and<AddressingMode::Immediate>();        // AND immediate
        case 0x2A: return rol<AddressingMode::Accumulator>();       // ROL accumulator
        case 0x2B: return anc<AddressingMode::Immediate>();         // ANC immediate [Unofficial]
        case 0x2C: return bit<AddressingMode::Absolute>();          // BIT absolute
        case 0x2D: return _and<AddressingMode::Absolute>();         // AND absolute
        case 0x2E: return rol<AddressingMode::Absolute>();          // ROL absolute
        case 0x2F: return rla<AddressingMode::Absolute>();          // RLA absolute [Unofficial]
        case 0x30: return bmi<AddressingMode::Relative>();          // BMI relative
        case 0x31: return _and<AddressingMode::IndirectY>();        // AND indirect Y
        case 0x32: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0x33: return rla<AddressingMode::IndirectY>();         // RLA indirect Y [Unofficial]
        case 0x34: return nop<AddressingMode::ZeroPageIndexedX>();  // NOP zero-page indexed X [Unofficial]
        case 0x35: return _and<AddressingMode::ZeroPageIndexedX>(); // AND zero-page indexed X
        case 0x36: return rol<AddressingMode::ZeroPageIndexedX>();  // ROL zero-page indexed X
        case 0x37: return rla<AddressingMode::ZeroPageIndexedX>();  // RLA zero-page indexed X [Unofficial]
        case 0x38: return sec<AddressingMode::Implied>();           // SEC implied
        case 0x39: return _and<AddressingMode::AbsoluteIndexedY>(); // AND absolute indexed Y
        case 0x3A: return nop<AddressingMode::Implied>();           // NOP implied [Unofficial]
        case 0x3B: return rla<AddressingMode::AbsoluteIndexedY>();  // RLA absolute indexed Y [Unofficial]
        case 0x3C: return nop<AddressingMode::AbsoluteIndexedX>();  // NOP absolute indexed X [Unofficial]
        case 0x3D: return _and<AddressingMode::AbsoluteIndexedX>(); // AND absolute indexed X
        case 0x3E: return rol<AddressingMode::AbsoluteIndexedX>();  // ROL absolute indexed X
        case 0x3F: return rla<AddressingMode::AbsoluteIndexedX>();  // RLA absolute indexed X [Unofficial]
        case 0x40: return rti<AddressingMode::Implied>();           // RTI implied
        case 0x41: return eor<AddressingMode::IndirectX>();         // EOR indirect X
        case 0x42: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0x43: return sre<AddressingMode::IndirectX>();         // SRE indirect X [Unofficial]
        case 0x44: return nop<AddressingMode::ZeroPage>();          // NOP zero-page [Unofficial]
        case 0x45: return eor<AddressingMode::ZeroPage>();          // EOR zero-page
        case 0x46: return lsr<AddressingMode::ZeroPage>();          // LSR zero-page
        case 0x47: return sre<AddressingMode::ZeroPage>();          // SRE zero-page [Unofficial]
        case 0x48: return pha<AddressingMode::Implied>();           // PHA implied
        case 0x49: return eor<AddressingMode::Immediate>();         // EOR immediate
        case 0x4A: return lsr<AddressingMode::Accumulator>();       // LSR accumulator
        case 0x4B: return alr<AddressingMode::Immediate>();         // ALR immediate [Unofficial]
        case 0x4C: return jmp<AddressingMode::Absolute>();          // JMP absolute
        case 0x4D: return eor<AddressingMode::Absolute>();          // EOR absolute
        case 0x4E: return lsr<AddressingMode::Absolute>();          // LSR absolute
        case 0x4F: return sre<AddressingMode::Absolute>();          // SRE absolute [Unofficial]
        case 0x50: return bvc<AddressingMode::Relative>();          // BVC relative
        case 0x51: return eor<AddressingMode::IndirectY>();         // EOR indirect Y
        case 0x52: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0x53: return sre<AddressingMode::IndirectY>();         // SRE indirect Y [Unofficial]
        case 0x54: return nop<AddressingMode::ZeroPageIndexedX>();  // NOP zero-page indexed X [Unofficial]
        case 0x55: return eor<AddressingMode::ZeroPageIndexedX>();  // EOR zero-page indexed X
        case 0x56: return lsr<AddressingMode::ZeroPageIndexedX>();  // LSR zero-page indexed X
        case 0x57: return sre<AddressingMode::ZeroPageIndexedX>();  // SRE zero-page indexed X [Unofficial]
        case 0x58: return cli<AddressingMode::Implied>();           // CLI implied
        case 0x59: return eor<AddressingMode::AbsoluteIndexedY>();  // EOR absolute indexed Y
        case 0x5A: return nop<AddressingMode::Implied>();           // NOP implied [Unofficial]
        case 0x5B: return sre<AddressingMode::AbsoluteIndexedY>();  // SRE absolute indexed Y [Unofficial]
        case 0x5C: return nop<AddressingMode::AbsoluteIndexedX>();  // NOP absolute indexed X [Unofficial]
        case 0x5D: return eor<AddressingMode::AbsoluteIndexedX>();  // EOR absolute indexed X
        case 0x5E: return lsr<AddressingMode::AbsoluteIndexedX>();  // LSR absolute indexed X
        case 0x5F: return sre<AddressingMode::AbsoluteIndexedX>();  // SRE absolute indexed X [Unofficial]
        case 0x60: return rts<AddressingMode::Implied>();           // RTS implied
        case 0x61: return adc<AddressingMode::IndirectX>();         // ADC indirect X
        case 0x62: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0x63: return rra<AddressingMode::IndirectX>();         // RRA indirect X [Unofficial]
        case 0x64: return nop<AddressingMode::ZeroPage>();          // NOP zero-page [Unofficial]
        case 0x65: return adc<AddressingMode::ZeroPage>();          // ADC zero-page
        case 0x66: return ror<AddressingMode::ZeroPage>();          // ROR zero-page
        case 0x67: return rra<AddressingMode::ZeroPage>();          // RRA zero-page [Unofficial]
        case 0x68: return pla<AddressingMode::Implied>();           // PLA implied
        case 0x69: return adc<AddressingMode::Immediate>();         // ADC immediate
        case 0x6A: return ror<AddressingMode::Accumulator>();       // ROR accumulator
        case 0x6B: return arr<AddressingMode::Immediate>();         // ARR immediate [Unofficial]
        case 0x6C: return jmp<AddressingMode::Indirect>();          // JMP indirect
        case 0x6D: return adc<AddressingMode::Absolute>();          // ADC absolute
        case 0x6E: return ror<AddressingMode::Absolute>();          // ROR absolute
        case 0x6F: return rra<AddressingMode::Absolute>();          // RRA absolute [Unofficial]
        case 0x70: return bvs<AddressingMode::Relative>();          // BVS relative
        case 0x71: return adc<AddressingMode::IndirectY>();         // ADC indirect Y
        case 0x72: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0x73: return rra<AddressingMode::IndirectY>();         // RRA indirect Y [Unofficial]
        case 0x74: return nop<AddressingMode::ZeroPageIndexedX>();  // NOP zero-page indexed X [Unofficial]
        case 0x75: return adc<AddressingMode::ZeroPageIndexedX>();  // ADC zero-page indexed X
        case 0x76: return ror<AddressingMode::ZeroPageIndexedX>();  // ROR zero-page indexed X
        case 0x77: return rra<AddressingMode::ZeroPageIndexedX>();  // RRA zero-page indexed X [Unofficial]
        case 0x78: return sei<AddressingMode::Implied>();           // SEI implied
        case 0x79: return adc<AddressingMode::AbsoluteIndexedY>();  // ADC absolute indexed Y
        case 0x7A: return nop<AddressingMode::Implied>();           // NOP implied [Unofficial]
        case 0x7B: return rra<AddressingMode::AbsoluteIndexedY>();  // RRA absolute indexed Y [Unofficial]
        case 0x7C: return nop<AddressingMode::AbsoluteIndexedX>();  // NOP absolute indexed X [Unofficial]
        case 0x7D: return adc<AddressingMode::AbsoluteIndexedX>();  // ADC absolute indexed X
        case 0x7E: return ror<AddressingMode::AbsoluteIndexedX>();  // ROR absolute indexed X
        case 0x7F: return rra<AddressingMode::AbsoluteIndexedX>();  // RRA absolute indexed X [Unofficial]
        case 0x80: return nop<AddressingMode::Immediate>();         // NOP immediate [Unofficial]
        case 0x81: return sta<AddressingMode::IndirectX>();         // STA indirect X
        case 0x82: return nop<AddressingMode::Immediate>();         // NOP immediate [Unofficial]
        case 0x83: return sax<AddressingMode::IndirectX>();         // SAX indirect X [Unofficial]
        case 0x84: return sty<AddressingMode::ZeroPage>();          // STY zero-page
        case 0x85: return sta<AddressingMode::ZeroPage>();          // STA zero-page
        case 0x86: return stx<AddressingMode::ZeroPage>();          // STX zero-page
        case 0x87: return sax<AddressingMode::ZeroPage>();          // SAX zero-page [Unofficial]
        case 0x88: return dey<AddressingMode::Implied>();           // DEY implied
        case 0x89: return nop<AddressingMode::Immediate>();         // NOP immediate [Unofficial]
        case 0x8A: return txa<AddressingMode::Implied>();           // TXA implied
        case 0x8B: return xaa<AddressingMode::Immediate>();         // XAA immediate [Unofficial]
        case 0x8C: return sty<AddressingMode::Absolute>();          // STY absolute
        case 0x8D: return sta<AddressingMode::Absolute>();          // STA absolute
        case 0x8E: return stx<AddressingMode::Absolute>();          // STX absolute
        case 0x8F: return sax<AddressingMode::Absolute>();          // SAX absolute [Unofficial]
        case 0x90: return bcc<AddressingMode::Relative>();          // BCC relative
        case 0x91: return sta<AddressingMode::IndirectY>();         // STA indirect Y
        case 0x92: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0x93: return ahx<AddressingMode::IndirectY>();         // AHX indirect Y [Unofficial]
        case 0x94: return sty<AddressingMode::ZeroPageIndexedX>();  // STY zero-page indexed X
        case 0x95: return sta<AddressingMode::ZeroPageIndexedX>();  // STA zero-page indexed X
        case 0x96: return stx<AddressingMode::ZeroPageIndexedY>();  // STX zero-page indexed Y
        case 0x97: return sax<AddressingMode::ZeroPageIndexedY>();  // SAX zero-page indexed Y [Unofficial]
        case 0x98: return tya<AddressingMode::Implied>();           // TYA implied
        case 0x99: return sta<AddressingMode::AbsoluteIndexedY>();  // STA absolute indexed Y
        case 0x9A: return txs<AddressingMode::Implied>();           // TXS implied
        case 0x9B: return tas<AddressingMode::AbsoluteIndexedY>();  // TAS absolute indexed Y [Unofficial]
        case 0x9C: return shy<AddressingMode::AbsoluteIndexedX>();  // SHY absolute indexed X [Unofficial]
        case 0x9D: return sta<AddressingMode::AbsoluteIndexedX>();  // STA absolute indexed X
        case 0x9E: return shx<AddressingMode::AbsoluteIndexedY>();  // SHX absolute indexed Y [Unofficial]
        case 0x9F: return ahx<AddressingMode::AbsoluteIndexedY>();  // AHX absolute indexed Y [Unofficial]
        case 0xA0: return ldy<AddressingMode::Immediate>();         // LDY immediate
        case 0xA1: return lda<AddressingMode::IndirectX>();         // LDA indirect X
        case 0xA2: return ldx<AddressingMode::Immediate>();         // LDX immediate
        case 0xA3: return lax<AddressingMode::IndirectX>();         // LAX indirect X [Unofficial]
        case 0xA4: return ldy<AddressingMode::ZeroPage>();          // LDY zero-page
        case 0xA5: return lda<AddressingMode::ZeroPage>();          // LDA zero-page
        case 0xA6: return ldx<AddressingMode::ZeroPage>();          // LDX zero-page
        case 0xA7: return lax<AddressingMode::ZeroPage>();          // LAX zero-page [Unofficial]
        case 0xA8: return tay<AddressingMode::Implied>();           // TAY implied
        case 0xA9: return lda<AddressingMode::Immediate>();         // LDA immediate
        case 0xAA: return tax<AddressingMode::Implied>();           // TAX implied
        case 0xAB: return lxa<AddressingMode::Immediate>();         // LXA immediate [Unofficial]
        case 0xAC: return ldy<AddressingMode::Absolute>();          // LDY absolute
        case 0xAD: return lda<AddressingMode::Absolute>();          // LDA absolute
        case 0xAE: return ldx<AddressingMode::Absolute>();          // LDX absolute
        case 0xAF: return lax<AddressingMode::Absolute>();          // LAX absolute [Unofficial]
        case 0xB0: return bcs<AddressingMode::Relative>();          // BCS relative
        case 0xB1: return lda<AddressingMode::IndirectY>();         // LDA indirect Y
        case 0xB2: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0xB3: return lax<AddressingMode::IndirectY>();         // LAX indirect Y [Unofficial]
        case 0xB4: return ldy<AddressingMode::ZeroPageIndexedX>();  // LDY zero-page indexed X
        case 0xB5: return lda<AddressingMode::ZeroPageIndexedX>();  // LDA zero-page indexed X
        case 0xB6: return ldx<AddressingMode::ZeroPageIndexedY>();  // LDX zero-page indexed Y
        case 0xB7: return lax<AddressingMode::ZeroPageIndexedY>();  // LAX zero-page indexed Y [Unofficial]
        case 0xB8: return clv<AddressingMode::Implied>();           // CLV implied
        case 0xB9: return lda<AddressingMode::ZeroPageIndexedY>();  // LDA absolute indexed Y
        case 0xBA: return tsx<AddressingMode::Implied>();           // TSX implied
        case 0xBB: return las<AddressingMode::AbsoluteIndexedY>();  // LAS absolute indexed Y [Unofficial]
        case 0xBC: return ldy<AddressingMode::AbsoluteIndexedX>();  // LDY absolute indexed X
        case 0xBD: return lda<AddressingMode::AbsoluteIndexedX>();  // LDA absolute indexed X
        case 0xBE: return ldx<AddressingMode::AbsoluteIndexedY>();  // LDX absolute indexed Y
        case 0xBF: return lax<AddressingMode::AbsoluteIndexedY>();  // LAX absolute indexed Y [Unofficial]
        case 0xC0: return cpy<AddressingMode::Immediate>();         // CPY immediate
        case 0xC1: return cmp<AddressingMode::IndirectX>();         // CMP indirect X
        case 0xC2: return nop<AddressingMode::Immediate>();         // NOP immediate [Unofficial]
        case 0xC3: return dcp<AddressingMode::IndirectX>();         // DCP indirect X [Unofficial]
        case 0xC4: return cpy<AddressingMode::ZeroPage>();          // CPY zero-page
        case 0xC5: return cmp<AddressingMode::ZeroPage>();          // CMP zero-page
        case 0xC6: return dec<AddressingMode::ZeroPage>();          // DEC zero-page
        case 0xC7: return dcp<AddressingMode::ZeroPage>();          // DCP zero-page [Unofficial]
        case 0xC8: return iny<AddressingMode::Implied>();           // INY implied
        case 0xC9: return cmp<AddressingMode::Immediate>();         // CMP immediate
        case 0xCA: return dex<AddressingMode::Implied>();           // DEX implied
        case 0xCB: return axs<AddressingMode::Immediate>();         // AXS immediate [Unofficial]
        case 0xCC: return cpy<AddressingMode::Absolute>();          // CPY absolute
        case 0xCD: return cmp<AddressingMode::Absolute>();          // CMP absolute
        case 0xCE: return dec<AddressingMode::Absolute>();          // DEC absolute
        case 0xCF: return dcp<AddressingMode::Absolute>();          // DCP absolute [Unofficial]
        case 0xD0: return bne<AddressingMode::Relative>();          // BNE relative
        case 0xD1: return cmp<AddressingMode::IndirectY>();         // CMP indirect Y
        case 0xD2: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0xD3: return dcp<AddressingMode::IndirectY>();         // DCP indirect Y [Unofficial]
        case 0xD4: return nop<AddressingMode::ZeroPageIndexedX>();  // NOP zero-page indexed X [Unofficial]
        case 0xD5: return cmp<AddressingMode::ZeroPageIndexedX>();  // CMP zero-page indexed X
        case 0xD6: return dec<AddressingMode::ZeroPageIndexedX>();  // DEC zero-page indexed X
        case 0xD7: return dcp<AddressingMode::ZeroPageIndexedX>();  // DCP zero-page indexed X [Unofficial]
        case 0xD8: return cld<AddressingMode::Implied>();           // CLD implied
        case 0xD9: return cmp<AddressingMode::AbsoluteIndexedY>();  // CMP absolute indexed Y
        case 0xDA: return nop<AddressingMode::Implied>();           // NOP implied [Unofficial]
        case 0xDB: return dcp<AddressingMode::AbsoluteIndexedY>();  // DCP absolute indexed Y [Unofficial]
        case 0xDC: return nop<AddressingMode::AbsoluteIndexedX>();  // NOP absolute indexed X [Unofficial]
        case 0xDD: return cmp<AddressingMode::AbsoluteIndexedX>();  // CMP absolute indexed X
        case 0xDE: return dec<AddressingMode::AbsoluteIndexedX>();  // DEC absolute indexed X
        case 0xDF: return dcp<AddressingMode::AbsoluteIndexedX>();  // DCP absolute indexed X [Unofficial]
        case 0xE0: return cpx<AddressingMode::Immediate>();         // CPX immediate
        case 0xE1: return sbc<AddressingMode::IndirectX>();         // SBC indirect X
        case 0xE2: return nop<AddressingMode::Immediate>();         // NOP immediate [Unofficial]
        case 0xE3: return isc<AddressingMode::IndirectX>();         // ISC indirect X [Unofficial]
        case 0xE4: return cpx<AddressingMode::ZeroPage>();          // CPX zero-page
        case 0xE5: return sbc<AddressingMode::ZeroPage>();          // SBC zero-page
        case 0xE6: return inc<AddressingMode::ZeroPage>();          // INC zero-page
        case 0xE7: return isc<AddressingMode::ZeroPage>();          // ISC zero-page [Unofficial]
        case 0xE8: return inx<AddressingMode::Implied>();           // INX implied
        case 0xE9: return sbc<AddressingMode::Immediate>();         // SBC immediate
        case 0xEA: return nop<AddressingMode::Implied>();           // NOP
        case 0xEB: return usbc<AddressingMode::Immediate>();        // USBC immediate [Unofficial]
        case 0xEC: return cpx<AddressingMode::Absolute>();          // CPX absolute
        case 0xED: return sbc<AddressingMode::Absolute>();          // SBC absolute
        case 0xEE: return inc<AddressingMode::Absolute>();          // INC absolute
        case 0xEF: return isc<AddressingMode::Absolute>();          // ISC absolute [Unofficial]
        case 0xF0: return beq<AddressingMode::Relative>();          // BEQ relative
        case 0xF1: return sbc<AddressingMode::IndirectY>();         // SBC indirect Y
        case 0xF2: return stp<AddressingMode::Implied>();           // STP [Unofficial]
        case 0xF3: return isc<AddressingMode::IndirectY>();         // ISC indirect Y [Unofficial]
        case 0xF4: return nop<AddressingMode::ZeroPageIndexedX>();  // NOP zero-page indexed X [Unofficial]
        case 0xF5: return sbc<AddressingMode::ZeroPageIndexedX>();  // SBC zero-page indexed X
        case 0xF6: return inc<AddressingMode::ZeroPageIndexedX>();  // INC zero-page indexed X
        case 0xF7: return isc<AddressingMode::ZeroPageIndexedX>();  // ISC zero-page indexed X [Unofficial]
        case 0xF8: return sed<AddressingMode::Implied>();           // SED implied
        case 0xF9: return sbc<AddressingMode::AbsoluteIndexedY>();  // SBC absolute indexed Y
        case 0xFA: return nop<AddressingMode::Implied>();           // NOP implied [Unofficial]
        case 0xFB: return isc<AddressingMode::AbsoluteIndexedY>();  // ISC absolute indexed Y [Unofficial]
        case 0xFC: return nop<AddressingMode::AbsoluteIndexedX>();  // NOP absolute indexed X [Unofficial]
        case 0xFD: return sbc<AddressingMode::AbsoluteIndexedX>();  // SBC absolute indexed X
        case 0xFE: return inc<AddressingMode::AbsoluteIndexedX>();  // INC absolute indexed X
        case 0xFF: return isc<AddressingMode::AbsoluteIndexedX>();  // ISC absolute indexed X [Unofficial]
    }

    return 0;
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

CpuRegisters &Cpu::getRegisters()
{
    return registers;
}

unsigned Cpu::handleInterrupt(InterruptType type)
{
    auto& flags = registers.getP();
    auto& pc = registers.getPc();
    
    if(flags.interruptDisable && type == InterruptType::IRQ) {
        return 0;
    }

    if(type == InterruptType::BRK) {
        pc++;
        flags.breakFlag = type == InterruptType::BRK;
    }

    pushIntoStack16(pc);
    pushIntoStack8(flags.raw);

    flags.interruptDisable = true;

    switch(type) {
        case InterruptType::BRK:
        case InterruptType::IRQ:
            pc = 0xFFFE;
            break;
        case InterruptType::NMI:
            pc = 0xFFFA;
            break;
    }

    return 7;
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
