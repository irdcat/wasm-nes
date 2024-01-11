/**
 * LDA - Load Accumulator
 */
template <AddressingMode Mode>
inline unsigned Cpu::lda()
{
    static_assert("Unsupported addressing mode used in LDA instruction");
    return 0;
}

template <>
inline unsigned Cpu::lda<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    accumulator = fetchImmedate8();
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

template <>
inline unsigned Cpu::lda<AddressingMode::Absolute>() 
{
    auto& accumulator = registers.getA();
    auto address = fetchImmedate16();
    accumulator = mmu->readFromMemory(address);
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::lda<AddressingMode::AbsoluteIndexedX>()
{
    auto& accumulator = registers.getA();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    accumulator = mmu->readFromMemory(targetAddress);
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::lda<AddressingMode::AbsoluteIndexedY>()
{
    auto& accumulator = registers.getA();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    accumulator = mmu->readFromMemory(targetAddress);
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::lda<AddressingMode::ZeroPage>()
{
    auto& accumulator = registers.getA();
    auto address = fetchImmedate8();
    accumulator = mmu->readFromMemory(address);
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 3;
}

template <>
inline unsigned Cpu::lda<AddressingMode::ZeroPageIndexedX>()
{
    auto& accumulator = registers.getA();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    u16 targetAddress = baseAddress + index;
    accumulator = mmu->readFromMemory(targetAddress & 0xFF);
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::lda<AddressingMode::IndirectX>()
{
    auto& accumulator = registers.getA();
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    u16 targetPointerAddress = basePointerAddress + index;
    u16 address = mmu->readFromMemory(targetPointerAddress & 0xFF)
        | mmu->readFromMemory((targetPointerAddress + 1) & 0xFF) << 8;
    accumulator = mmu->readFromMemory(address);
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 6;
}

template <>
inline unsigned Cpu::lda<AddressingMode::IndirectY>()
{
    auto& accumulator = registers.getA();
    auto index = registers.getY();
    auto pointerAddress = fetchImmedate8();
    u16 baseAddress = mmu->readFromMemory(pointerAddress & 0xFF)
        | mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
    auto targetAddress = baseAddress + index;
    accumulator = mmu->readFromMemory(targetAddress);
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * LDX - Load register X
 */
template <AddressingMode Mode>
inline unsigned Cpu::ldx()
{
    static_assert("Unsupported addressing mode used in LDX instruction");
    return 0;
}

template <>
inline unsigned Cpu::ldx<AddressingMode::Immediate>()
{
    auto& x = registers.getX();
    x = fetchImmedate8();
    updateZeroFlag(x);
    updateNegativeFlag(x);
    return 2;
}

template <>
inline unsigned Cpu::ldx<AddressingMode::Absolute>()
{
    auto& x = registers.getX();
    auto address = fetchImmedate16();
    x = mmu->readFromMemory(address);
    updateZeroFlag(x);
    updateNegativeFlag(x);
    return 4;
}

template <>
inline unsigned Cpu::ldx<AddressingMode::AbsoluteIndexedY>()
{
    auto& x = registers.getX();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    x = mmu->readFromMemory(targetAddress);
    updateZeroFlag(x);
    updateNegativeFlag(x);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::ldx<AddressingMode::ZeroPage>()
{
    auto& x = registers.getX();
    auto address = fetchImmedate8();
    x = mmu->readFromMemory(address);
    updateZeroFlag(x);
    updateNegativeFlag(x);
    return 3;
}

template <>
inline unsigned Cpu::ldx<AddressingMode::ZeroPageIndexedY>()
{
    auto& x = registers.getX();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    x = mmu->readFromMemory(targetAddress & 0xFF);
    updateZeroFlag(x);
    updateNegativeFlag(x);
    return 4;
}

/**
 * LDY - Load Y register 
 */
template <AddressingMode Mode>
inline unsigned Cpu::ldy()
{
    static_assert("Unsupported addressing mode used in LDY instruction");
    return 0;
}

template <>
inline unsigned Cpu::ldy<AddressingMode::Immediate>()
{
    auto& y = registers.getY();
    y = fetchImmedate8();
    updateZeroFlag(y);
    updateNegativeFlag(y);
    return 2;
}

template <>
inline unsigned Cpu::ldy<AddressingMode::Absolute>()
{
    auto& y = registers.getY();
    auto address = fetchImmedate16();
    y = mmu->readFromMemory(address);
    updateZeroFlag(y);
    updateNegativeFlag(y);
    return 4;
}

template <>
inline unsigned Cpu::ldy<AddressingMode::AbsoluteIndexedX>()
{
    auto& y = registers.getY();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    y = mmu->readFromMemory(targetAddress);
    updateZeroFlag(y);
    updateNegativeFlag(y);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::ldy<AddressingMode::ZeroPage>()
{
    auto& y = registers.getY();
    auto address = fetchImmedate8();
    y = mmu->readFromMemory(address);
    updateZeroFlag(y);
    updateNegativeFlag(y);
    return 3;
}

template <>
inline unsigned Cpu::ldy<AddressingMode::ZeroPageIndexedX>()
{
    auto& y = registers.getY();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    y = mmu->readFromMemory(targetAddress & 0xFF);
    updateZeroFlag(y);
    updateNegativeFlag(y);
    return 4;
}

/**
 * STA - Store Accumulator 
 */
template <AddressingMode Mode>
inline unsigned Cpu::sta()
{
    static_assert("Unsupported addressing mode used in STA instruction");
    return 0;
}

template <>
inline unsigned Cpu::sta<AddressingMode::Absolute>()
{
    auto accumulator = registers.getA();
    auto address = fetchImmedate16();
    mmu->writeIntoMemory(address, accumulator);
    return 4;
}

template <>
inline unsigned Cpu::sta<AddressingMode::AbsoluteIndexedX>()
{
    auto accumulator = registers.getA();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    mmu->writeIntoMemory(targetAddress, accumulator);
    return 5;
}

template <>
inline unsigned Cpu::sta<AddressingMode::AbsoluteIndexedY>()
{
    auto accumulator = registers.getA();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    mmu->writeIntoMemory(targetAddress, accumulator);
    return 5;
}

template <>
inline unsigned Cpu::sta<AddressingMode::ZeroPage>()
{
    auto accumulator = registers.getA();
    auto address = fetchImmedate8();
    mmu->writeIntoMemory(address, accumulator);
    return 3;
}

template <>
inline unsigned Cpu::sta<AddressingMode::ZeroPageIndexedX>()
{
    auto accumulator = registers.getA();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    mmu->writeIntoMemory(targetAddress, accumulator);
    return 4;
}

template <>
inline unsigned Cpu::sta<AddressingMode::IndirectX>()
{
    auto accumulator = registers.getA();
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    u16 targetPointerAddress = basePointerAddress + index;
    u16 address = mmu->readFromMemory(targetPointerAddress & 0xFF)
        | mmu->readFromMemory((targetPointerAddress + 1) & 0xFF) << 8;
    mmu->writeIntoMemory(address, accumulator);
    return 6;
}

template <>
inline unsigned Cpu::sta<AddressingMode::IndirectY>()
{
    auto accumulator = registers.getA();
    auto index = registers.getY();
    auto pointerAddress = fetchImmedate8();
    u16 baseAddress = mmu->readFromMemory(pointerAddress & 0xFF)
        | mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
    auto targetAddress = baseAddress + index;
    mmu->writeIntoMemory(targetAddress, accumulator);
    return 6;
}

/**
 * Store X register
 */
template <AddressingMode Mode>
inline unsigned Cpu::stx()
{
    static_assert("Unsupported addressing mode used in STX instruction");
    return 0;
}

template <>
inline unsigned Cpu::stx<AddressingMode::Absolute>()
{
    auto x = registers.getX();
    auto address = fetchImmedate16();
    mmu->writeIntoMemory(address, x);
    return 4;
}

template <>
inline unsigned Cpu::stx<AddressingMode::ZeroPage>()
{
    auto x = registers.getX();
    auto address = fetchImmedate8();
    mmu->writeIntoMemory(address, x);
    return 3;
}

template <>
inline unsigned Cpu::stx<AddressingMode::ZeroPageIndexedY>()
{
    auto x = registers.getX();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    mmu->writeIntoMemory(targetAddress & 0xFF, x);
    return 4;
}

/**
 * STY - Store Y register
 */
template <AddressingMode Mode>
inline unsigned Cpu::sty()
{
    static_assert("Unsupported addressing mode used in STY instruction");
    return 0;
}

template <>
inline unsigned Cpu::sty<AddressingMode::Absolute>()
{
    auto y = registers.getY();
    auto address = fetchImmedate16();
    mmu->writeIntoMemory(address, y);
    return 4;
}

template <>
inline unsigned Cpu::sty<AddressingMode::ZeroPage>()
{
    auto y = registers.getY();
    auto address = fetchImmedate8();
    mmu->writeIntoMemory(address, y);
    return 3;
}

template <>
inline unsigned Cpu::sty<AddressingMode::ZeroPageIndexedX>()
{
    auto y = registers.getY();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    mmu->writeIntoMemory(targetAddress & 0xFF, y);
    return 4;
}

/**
 * TAX - Transfer Accmulator to X
 */
template <AddressingMode Mode>
inline unsigned Cpu::tax()
{
    static_assert("Unsupported addressing mode used in TAX instruction");
    return 0;
}

template <>
inline unsigned Cpu::tax<AddressingMode::Implied>()
{
    const auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto& x = registers.getX();
    x = accumulator;
    updateZeroFlag(x);
    updateNegativeFlag(x);
    return 2;
}

/**
 * TAY - Transfer Accumulator to Y 
 */
template <AddressingMode Mode>
inline unsigned Cpu::tay()
{
    static_assert("Unsupported addressing mode used in TAY instruction");
    return 0;
}

template <>
inline unsigned Cpu::tay<AddressingMode::Implied>()
{
    const auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto& y = registers.getY();
    y = accumulator;
    updateZeroFlag(y);
    updateNegativeFlag(y);
    return 2;
}

/**
 * TSX - Transfer Stack pointer to X
 */
template <AddressingMode Mode>
inline unsigned Cpu::tsx()
{
    static_assert("Unsupported addressing mode used in TSX instruction");
    return 0;
}

template <>
inline unsigned Cpu::tsx<AddressingMode::Implied>()
{
    const auto& s = registers.getS();
    auto& x = registers.getX();
    x = s;
    updateZeroFlag(x);
    updateNegativeFlag(x);
    return 2;
}

/**
 * TXA - Transfer X to Accumulator
 */
template <AddressingMode Mode>
inline unsigned Cpu::txa()
{
    static_assert("Unsupported addressing mode used in TXA instruction");
    return 0;
}

template <>
inline unsigned Cpu::txa<AddressingMode::Implied>()
{
    const auto& x = registers.getX();
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    accumulator = x;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

/**
 * TXS - Transfer X to Stack pointer
 */
template <AddressingMode Mode>
inline unsigned Cpu::txs()
{
    static_assert("Unsupported addressing mode used in TXS instruction");
    return 0;
}

template <>
inline unsigned Cpu::txs<AddressingMode::Implied>()
{
    const auto& x = registers.getX();
    auto& s = registers.getS();
    s = x;
    return 2;
}

/**
 * TYA - Transfer Y to Accumulator 
 */
template <AddressingMode Mode>
inline unsigned Cpu::tya()
{
    static_assert("Unsupported addressing mode used in TYA instruction");
    return 0;
}

template <>
inline unsigned Cpu::tya<AddressingMode::Implied>()
{
    const auto& y = registers.getY();
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    accumulator = y;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

/**
 * DEC - Decrement memory
 */
template <AddressingMode Mode>
inline unsigned Cpu::dec()
{
    static_assert("Unsupported addressing mode used in DEC instruction");
    return 0;
}

template <>
inline unsigned Cpu::dec<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);
    value--;
    updateZeroFlag(value);
    updateNegativeFlag(value);
    mmu->writeIntoMemory(address, value);
    return 6;
}

template <>
inline unsigned Cpu::dec<AddressingMode::AbsoluteIndexedX>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    value--;
    updateZeroFlag(value);
    updateNegativeFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::dec<AddressingMode::ZeroPage>()
{
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);
    value--;
    updateZeroFlag(value);
    updateNegativeFlag(value);
    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::dec<AddressingMode::ZeroPageIndexedX>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    value--;
    updateZeroFlag(value);
    updateNegativeFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 6;
}


/**
 * DEX - Decrement X 
 */
template <AddressingMode Mode>
inline unsigned Cpu::dex()
{
    static_assert("Unsupported addressing mode used in DEX instruction");
    return 0;
}

template <>
inline unsigned Cpu::dex<AddressingMode::Implied>()
{
    auto& x = registers.getX();
    auto& flags = registers.getP();
    x--;
    updateZeroFlag(x);
    updateNegativeFlag(x);
    return 2;
}

/**
 * DEY - Decrement Y 
 */
template <AddressingMode Mode>
inline unsigned Cpu::dey()
{
    static_assert("Unsupported addressing mode used in DEY instruction");
    return 0;
}

template <>
inline unsigned Cpu::dey<AddressingMode::Implied>()
{
    auto& y = registers.getY();
    auto& flags = registers.getP();
    y--;
    updateZeroFlag(y);
    updateNegativeFlag(y);
    return 2;
}

/**
 * INC - Increment memory 
 */
template <AddressingMode Mode>
inline unsigned Cpu::inc()
{
    static_assert("Unsupported addressing mode used in INC instruction");
    return 0;
}

template <>
inline unsigned Cpu::inc<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);
    value++;
    updateZeroFlag(value);
    updateNegativeFlag(value);
    mmu->writeIntoMemory(address, value);
    return 6;
}

template <>
inline unsigned Cpu::inc<AddressingMode::AbsoluteIndexedX>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    value++;
    updateZeroFlag(value);
    updateNegativeFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::inc<AddressingMode::ZeroPage>()
{
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);
    value++;
    updateZeroFlag(value);
    updateNegativeFlag(value);
    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::inc<AddressingMode::ZeroPageIndexedX>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    value++;
    updateZeroFlag(value);
    updateNegativeFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 6;
}

/**
 * INX - Increment X 
 */
template <AddressingMode Mode>
inline unsigned Cpu::inx()
{
    static_assert("Unsupported addressing mode used in INX instruction");
    return 0;
}

template <>
inline unsigned Cpu::inx<AddressingMode::Implied>()
{
    auto& x = registers.getX();
    auto& flags = registers.getP();
    x++;
    updateZeroFlag(x);
    updateNegativeFlag(x);
    return 2;
}

/**
 * INY - Increment Y
 */
template <AddressingMode Mode>
inline unsigned Cpu::iny()
{
    static_assert("Unsupoorted addressing mode used in INY instruction");
    return 0;
}

template <>
inline unsigned Cpu::iny<AddressingMode::Implied>()
{
    auto& y = registers.getY();
    auto& flags = registers.getP();
    y++;
    updateZeroFlag(y);
    updateNegativeFlag(y);
    return 2;
}

/**
 * CLC - Clear Carry flag 
 */
template <AddressingMode Mode>
inline unsigned Cpu::clc()
{
    static_assert("Unsupported addressing mode used in CLC instruction");
    return 0;
}

template <>
inline unsigned Cpu::clc<AddressingMode::Implied>()
{
    registers.getP().carry = 0;
    return 2;
}

/**
 * CLD - Clear Decimal flag 
 */
template <AddressingMode Mode>
inline unsigned Cpu::cld()
{
    static_assert("Unsupported addressing mode used in CLD instruction");
    return 0;
}

template <>
inline unsigned Cpu::cld<AddressingMode::Implied>()
{
    registers.getP().decimal = 0;
    return 2;
}

/**
 * CLI - Clear Interrupt disable flag 
 */
template <AddressingMode Mode>
inline unsigned Cpu::cli()
{
    static_assert("Unsupoorted addressing mode used in CLI instruction");
    return 0;
}

template <>
inline unsigned Cpu::cli<AddressingMode::Implied>()
{
    registers.getP().interruptDisable = 0;
    return 2;
}

/**
 * CLV - Clear Overflow flag 
 */
template <AddressingMode Mode>
inline unsigned Cpu::clv()
{
    static_assert("Unsupported addressing mode used in CLV instruction");
    return 0;
}

template <>
inline unsigned Cpu::clv<AddressingMode::Implied>()
{
    registers.getP().overflow = 0;
    return 2;
}

/**
 * SEC - Set Carry flag 
 */
template <AddressingMode Mode>
inline unsigned Cpu::sec()
{
    static_assert("Unsupported addressing mode used in SEC instruction");
    return 0;
}

template <>
inline unsigned Cpu::sec<AddressingMode::Implied>()
{
    registers.getP().carry = 1;
    return 2;
}

/**
 * SED - Set Decimal flag 
 */
template <AddressingMode Mode>
inline unsigned Cpu::sed()
{
    static_assert("Unsupported addressing mode used in SED instruction");
    return 0;
}

template <>
inline unsigned Cpu::sed<AddressingMode::Implied>()
{
    registers.getP().decimal = 1;
    return 2;
}

/**
 * SEI - Set Interrupt disable flag 
 */
template <AddressingMode Mode>
inline unsigned Cpu::sei()
{
    static_assert("Unsupported addressing mode used in SEI instruction");
    return 0;
}

template <>
inline unsigned Cpu::sei<AddressingMode::Implied>()
{
    registers.getP().interruptDisable = 1;
    return 2;
}

/**
 * PHA - Push accumulator into stack 
 */
template <AddressingMode Mode>
inline unsigned Cpu::pha()
{
    static_assert("Unsupported addressing mode used in PHA instruction");
    return 0;
}

template <>
inline unsigned Cpu::pha<AddressingMode::Implied>()
{
    const auto& accumulator = registers.getA();
    pushIntoStack8(accumulator);
    return 3;
}

/**
 * PHP - Push Processor status into stack 
 */
template <AddressingMode Mode>
inline unsigned Cpu::php()
{
    static_assert("Unsupported addressing mode used in PHP instruction");
    return 0;
}

template <>
inline unsigned Cpu::php<AddressingMode::Implied>()
{
    auto& flags = registers.getP();
    flags.raw |= 0x3 << 4;
    pushIntoStack8(flags);
    return 3;
}

/**
 * PLA - Pull accumulator from stack 
 */
template <AddressingMode Mode>
inline unsigned Cpu::pla()
{
    static_assert("Unsupported addressing mode used in PLA instruction");
    return 0;
}

template <>
inline unsigned Cpu::pla<AddressingMode::Implied>()
{
    auto& accumulator = registers.getA();
    accumulator = popFromStack8();
    return 4;
}

/**
 * PLP - Pull processor status from stack 
 */
template <AddressingMode Mode>
inline unsigned Cpu::plp()
{
    static_assert("Unsupported addressing mode used in PLP instruction");
    return 0;
}

template <>
inline unsigned Cpu::plp<AddressingMode::Implied>()
{
    auto& flags = registers.getP();
    flags = popFromStack8();
    flags &= ~(0x3 << 4);
    return 4;
}


/**
 * JMP - Jump
 */
template <AddressingMode Mode>
inline unsigned Cpu::jmp()
{
    static_assert("Unsupported addressing mode used in JMP instruction");
    return 0;
}

template <>
inline unsigned Cpu::jmp<AddressingMode::Absolute>()
{
    auto& pc = registers.getPc();
    auto address = fetchImmedate16();
    pc = address;
    return 3;
}

template <>
inline unsigned Cpu::jmp<AddressingMode::Indirect>()
{
    auto& pc = registers.getPc();
    auto pointer = fetchImmedate16();
    u16 address = mmu->readFromMemory(pointer)
        | mmu->readFromMemory(pointer + 1) << 8;
    pc = address;
    return 5;
}

/**
 * JSR - Jump to subroutine 
 */
template <AddressingMode Mode>
inline unsigned Cpu::jsr()
{
    static_assert("Unsupported addressing mode used in JSR instruction");
    return 0;
}

template <>
inline unsigned Cpu::jsr<AddressingMode::Absolute>()
{
    auto& pc = registers.getPc();
    pushIntoStack16(pc);
    auto address = fetchImmedate16();
    pc = address;
    return 6;
}

/**
 * RTS - Return from subroutine
 */
template <AddressingMode Mode>
inline unsigned Cpu::rts()
{
    static_assert("Unsupported addressing mode used in RTS instruction");
    return 0;
}

template <>
inline unsigned Cpu::rts<AddressingMode::Implied>()
{
    auto& pc = registers.getPc();
    pc = popFromStack16() + 2;
    return 6;
}

/**
 * ADC - Add memory to Accumulator with Carry 
 */
template <AddressingMode Mode>
inline unsigned Cpu::adc()
{
    static_assert("Unsupported addressing mode used in ADC instruction");
    return 0;
}

template <>
inline unsigned Cpu::adc<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto operand = fetchImmedate8();
    u16 result = accumulator + operand + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

template <>
inline unsigned Cpu::adc<AddressingMode::Absolute>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto address = fetchImmedate16();
    auto operand = mmu->readFromMemory(address);
    u16 result = accumulator + operand + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::adc<AddressingMode::AbsoluteIndexedX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    u16 result = accumulator + operand + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::adc<AddressingMode::AbsoluteIndexedY>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    u16 result = accumulator + operand + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::adc<AddressingMode::ZeroPage>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto address = fetchImmedate8();
    auto operand = mmu->readFromMemory(address);
    u16 result = accumulator + operand + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 3;
}

template <>
inline unsigned Cpu::adc<AddressingMode::ZeroPageIndexedX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress & 0xFF);
    u16 result = accumulator + operand + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::adc<AddressingMode::IndirectX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    u16 targetPointerAddress = basePointerAddress + index;
    u16 address = mmu->readFromMemory(targetPointerAddress & 0xFF)
        | mmu->readFromMemory((targetPointerAddress + 1) & 0xFF) << 8;
    auto operand = mmu->readFromMemory(address & 0xFF);
    u16 result = accumulator + operand + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 6;
}

template <>
inline unsigned Cpu::adc<AddressingMode::IndirectY>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto index = registers.getY();
    auto pointerAddress = fetchImmedate8();
    u16 baseAddress = mmu->readFromMemory(pointerAddress & 0xFF)
        | mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress & 0xFF);
    u16 result = accumulator + operand + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * SBC - Subtract memory from accumulator with borrow
 */
template <AddressingMode Mode>
inline unsigned Cpu::sbc()
{
    static_assert("Unsupported addressing mode used in SBC instruction");
    return 0;
}

template <>
inline unsigned Cpu::sbc<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto operand = fetchImmedate8();
    u16 result = accumulator - operand - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

template <>
inline unsigned Cpu::sbc<AddressingMode::Absolute>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto address = fetchImmedate16();
    auto operand = mmu->readFromMemory(address);
    u16 result = accumulator - operand - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::sbc<AddressingMode::AbsoluteIndexedX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    u16 result = accumulator - operand - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::sbc<AddressingMode::AbsoluteIndexedY>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    u16 result = accumulator - operand - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::sbc<AddressingMode::ZeroPage>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto address = fetchImmedate8();
    auto operand = mmu->readFromMemory(address);
    u16 result = accumulator - operand - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 3;
}

template <>
inline unsigned Cpu::sbc<AddressingMode::ZeroPageIndexedX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress & 0xFF);
    u16 result = accumulator - operand - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::sbc<AddressingMode::IndirectX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    u16 targetPointerAddress = basePointerAddress + index;
    u16 address = mmu->readFromMemory(targetPointerAddress & 0xFF)
        | mmu->readFromMemory((targetPointerAddress + 1) & 0xFF) << 8;
    auto operand = mmu->readFromMemory(address & 0xFF);
    u16 result = accumulator - operand - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 6;
}

template <>
inline unsigned Cpu::sbc<AddressingMode::IndirectY>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto index = registers.getY();
    auto pointerAddress = fetchImmedate8();
    u16 baseAddress = mmu->readFromMemory(pointerAddress & 0xFF)
        | mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress & 0xFF);
    u16 result = accumulator - operand - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * AND - AND memory with Accumulator 
 */
template <AddressingMode Mode>
inline unsigned Cpu::_and()
{
    static_assert("Unsupported addressing mode used in AND instruction");
    return 0;
}

template <>
inline unsigned Cpu::_and<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto operand = fetchImmedate8();
    accumulator &= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

template <>
inline unsigned Cpu::_and<AddressingMode::Absolute>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto address = fetchImmedate16();
    auto operand = mmu->readFromMemory(address);
    accumulator &= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::_and<AddressingMode::AbsoluteIndexedX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    accumulator &= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::_and<AddressingMode::AbsoluteIndexedY>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    accumulator &= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::_and<AddressingMode::ZeroPage>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto address = fetchImmedate8();
    auto operand = mmu->readFromMemory(address);
    accumulator &= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 3;
}

template <>
inline unsigned Cpu::_and<AddressingMode::ZeroPageIndexedX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress & 0xFF);
    accumulator &= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::_and<AddressingMode::IndirectX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    u16 targetPointerAddress = basePointerAddress + index;
    u16 address = mmu->readFromMemory(targetPointerAddress & 0xFF)
        | mmu->readFromMemory((targetPointerAddress + 1) & 0xFF) << 8;
    auto operand = mmu->readFromMemory(address & 0xFF);
    accumulator &= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 6;
}

template <>
inline unsigned Cpu::_and<AddressingMode::IndirectY>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto index = registers.getY();
    auto pointerAddress = fetchImmedate8();
    u16 baseAddress = mmu->readFromMemory(pointerAddress & 0xFF)
        | mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress & 0xFF);
    accumulator &= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * EOR - Exclusive OR memory with Accumulator 
 */
template <AddressingMode Mode>
inline unsigned Cpu::eor()
{
    static_assert("Unsupported addressing mode used in EOR instruction");
    return 0;
}

template <>
inline unsigned Cpu::eor<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto operand = fetchImmedate8();
    accumulator ^= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

template <>
inline unsigned Cpu::eor<AddressingMode::Absolute>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto address = fetchImmedate16();
    auto operand = mmu->readFromMemory(address);
    accumulator ^= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::eor<AddressingMode::AbsoluteIndexedX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    accumulator ^= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::eor<AddressingMode::AbsoluteIndexedY>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    accumulator ^= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::eor<AddressingMode::ZeroPage>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto address = fetchImmedate8();
    auto operand = mmu->readFromMemory(address);
    accumulator ^= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 3;
}

template <>
inline unsigned Cpu::eor<AddressingMode::ZeroPageIndexedX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress & 0xFF);
    accumulator ^= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::eor<AddressingMode::IndirectX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    u16 targetPointerAddress = basePointerAddress + index;
    u16 address = mmu->readFromMemory(targetPointerAddress & 0xFF)
        | mmu->readFromMemory((targetPointerAddress + 1) & 0xFF) << 8;
    auto operand = mmu->readFromMemory(address & 0xFF);
    accumulator ^= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 6;
}

template <>
inline unsigned Cpu::eor<AddressingMode::IndirectY>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto index = registers.getY();
    auto pointerAddress = fetchImmedate8();
    u16 baseAddress = mmu->readFromMemory(pointerAddress & 0xFF)
        | mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress & 0xFF);
    accumulator ^= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * ORA - OR memory with Accumulator 
 */
template <AddressingMode Mode>
inline unsigned Cpu::ora()
{
    static_assert("Unsupported addressing mode used in ORA instruction");
    return 0;
}

template <>
inline unsigned Cpu::ora<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto operand = fetchImmedate8();
    accumulator |= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

template <>
inline unsigned Cpu::ora<AddressingMode::Absolute>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto address = fetchImmedate16();
    auto operand = mmu->readFromMemory(address);
    accumulator |= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::ora<AddressingMode::AbsoluteIndexedX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    accumulator |= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::ora<AddressingMode::AbsoluteIndexedY>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    accumulator |= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::ora<AddressingMode::ZeroPage>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto address = fetchImmedate8();
    auto operand = mmu->readFromMemory(address);
    accumulator |= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 3;
}

template <>
inline unsigned Cpu::ora<AddressingMode::ZeroPageIndexedX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress & 0xFF);
    accumulator |= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

template <>
inline unsigned Cpu::ora<AddressingMode::IndirectX>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    u16 targetPointerAddress = basePointerAddress + index;
    u16 address = mmu->readFromMemory(targetPointerAddress & 0xFF)
        | mmu->readFromMemory((targetPointerAddress + 1) & 0xFF) << 8;
    auto operand = mmu->readFromMemory(address & 0xFF);
    accumulator |= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 6;
}

template <>
inline unsigned Cpu::ora<AddressingMode::IndirectY>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto index = registers.getY();
    auto pointerAddress = fetchImmedate8();
    u16 baseAddress = mmu->readFromMemory(pointerAddress & 0xFF)
        | mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress & 0xFF);
    accumulator |= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * BCC - Branch on Carry Clear 
 */
template <AddressingMode Mode>
inline unsigned Cpu::bcc()
{
    static_assert("Unsupported addressing mode used in BCC instruction");
    return 0;
}

template <>
inline unsigned Cpu::bcc<AddressingMode::Relative>()
{
    unsigned cycles = 2;
    auto flags = registers.getP();
    s8 offset = fetchImmedate8();
    auto& pc = registers.getPc();
    auto oldPc = pc;

    if(!flags.carry) {
        cycles++;
        pc += offset;
    }
    
    if(checkForPageCross(oldPc, pc)) {
        cycles++;
    }

    return cycles;
}

/**
 * BCS - Branch on Carry Set 
 */
template <AddressingMode Mode>
inline unsigned Cpu::bcs()
{
    static_assert("Unsupported addressing mode used in BCS instruction");
    return 0;
}

template <>
inline unsigned Cpu::bcs<AddressingMode::Relative>()
{
    unsigned cycles = 2;
    auto flags = registers.getP();
    s8 offset = fetchImmedate8();
    auto& pc = registers.getPc();
    auto oldPc = pc;

    if(flags.carry) {
        cycles++;
        pc += offset;
    }
    
    if(checkForPageCross(oldPc, pc)) {
        cycles++;
    }

    return cycles;
}

/**
 * BEQ - Branch on Result zero
 */
template <AddressingMode Mode>
inline unsigned Cpu::beq()
{
    static_assert("Unsupported addressing mode used in BEQ instruction");
    return 0;
}

template <>
inline unsigned Cpu::beq<AddressingMode::Relative>()
{
    unsigned cycles = 2;
    auto flags = registers.getP();
    s8 offset = fetchImmedate8();
    auto& pc = registers.getPc();
    auto oldPc = pc;

    if(flags.zero) {
        cycles++;
        pc += offset;
    }
    
    if(checkForPageCross(oldPc, pc)) {
        cycles++;
    }

    return cycles;
}

/**
 * BMI - Branch on Result Minus
 */
template <AddressingMode Mode>
inline unsigned Cpu::bmi()
{
    static_assert("Unsupported addressing mode used in BMI instruction");
    return 0;
}

template <>
inline unsigned Cpu::bmi<AddressingMode::Relative>()
{
    unsigned cycles = 2;
    auto flags = registers.getP();
    s8 offset = fetchImmedate8();
    auto& pc = registers.getPc();
    auto oldPc = pc;

    if(flags.negative) {
        cycles++;
        pc += offset;
    }
    
    if(checkForPageCross(oldPc, pc)) {
        cycles++;
    }

    return cycles;
}

/**
 * BNE - Branch on Result Not Zero 
 */
template <AddressingMode Mode>
inline unsigned Cpu::bne()
{
    static_assert("Unsupported addressing mode used in BNE instruction");
    return 0;
}

template <>
inline unsigned Cpu::bne<AddressingMode::Relative>()
{
    unsigned cycles = 2;
    auto flags = registers.getP();
    s8 offset = fetchImmedate8();
    auto& pc = registers.getPc();
    auto oldPc = pc;

    if(!flags.zero) {
        cycles++;
        pc += offset;
    }
    
    if(checkForPageCross(oldPc, pc)) {
        cycles++;
    }

    return cycles;
}

/**
 * BPL - Branch on Result Plus
 */
template <AddressingMode Mode>
inline unsigned Cpu::bpl()
{
    static_assert("Unsupported addressing mode used in BPL instruction");
    return 0;
}

template <>
inline unsigned Cpu::bpl<AddressingMode::Relative>()
{
    unsigned cycles = 2;
    auto flags = registers.getP();
    s8 offset = fetchImmedate8();
    auto& pc = registers.getPc();
    auto oldPc = pc;

    if(!flags.negative) {
        cycles++;
        pc += offset;
    }
    
    if(checkForPageCross(oldPc, pc)) {
        cycles++;
    }

    return cycles;
}

/**
 * BVC - Branch on Overflow Clear 
 */
template <AddressingMode Mode>
inline unsigned Cpu::bvc()
{
    static_assert("Unsupported addressing mode used in BVC instruction");
    return 0;
}

template <>
inline unsigned Cpu::bvc<AddressingMode::Relative>()
{
    unsigned cycles = 2;
    auto flags = registers.getP();
    s8 offset = fetchImmedate8();
    auto& pc = registers.getPc();
    auto oldPc = pc;

    if(!flags.overflow) {
        cycles++;
        pc += offset;
    }
    
    if(checkForPageCross(oldPc, pc)) {
        cycles++;
    }

    return cycles;
}

/**
 * BVS - Branch on Overflow Set 
 */
template <AddressingMode Mode>
inline unsigned Cpu::bvs()
{
    static_assert("Unsupported addressing mode used in BVS instruction");
    return 0;
}

template <>
inline unsigned Cpu::bvs<AddressingMode::Relative>()
{
    unsigned cycles = 2;
    auto flags = registers.getP();
    s8 offset = fetchImmedate8();
    auto& pc = registers.getPc();
    auto oldPc = pc;

    if(flags.overflow) {
        cycles++;
        pc += offset;
    }
    
    if(checkForPageCross(oldPc, pc)) {
        cycles++;
    }

    return cycles;
}

/**
 * ASL - Arithmetic Shift Left 
 */
template <AddressingMode Mode>
inline unsigned Cpu::asl()
{
    static_assert("Unsupported addressing mode used in ASL instruction");
    return 0;
}

template <>
inline unsigned Cpu::asl<AddressingMode::Accumulator>()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    flags.carry = (accumulator >> 7) & 0x1;
    accumulator <<= 1;
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);
    return 2;
}

template <>
inline unsigned Cpu::asl<AddressingMode::Absolute>()
{
    auto& flags = registers.getP();
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(address, value);
    return 6;
}

template <>
inline unsigned Cpu::asl<AddressingMode::AbsoluteIndexedX>()
{
    auto& flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::asl<AddressingMode::ZeroPage>()
{
    auto& flags = registers.getP();
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::asl<AddressingMode::ZeroPageIndexedX>()
{
    auto& flags = registers.getP();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 6;
}

/**
 * LSR - Logical Shift Left 
 */
template <AddressingMode Mode>
inline unsigned Cpu::lsr()
{
    static_assert("Unsupported addressing mode used in LSR instruction");
    return 0;
}

template <>
inline unsigned Cpu::lsr<AddressingMode::Accumulator>()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    flags.carry = accumulator & 0x1;
    accumulator >>= 1;
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);
    return 2;
}

template <>
inline unsigned Cpu::lsr<AddressingMode::Absolute>()
{
    auto& flags = registers.getP();
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);
    flags.carry = value & 0x1;
    value >>= 1;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(address, value);
    return 6;
}

template <>
inline unsigned Cpu::lsr<AddressingMode::AbsoluteIndexedX>()
{
    auto& flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    flags.carry = value & 0x1;
    value >>= 1;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::lsr<AddressingMode::ZeroPage>()
{
    auto& flags = registers.getP();
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);
    flags.carry = value & 0x1;
    value >>= 1;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::lsr<AddressingMode::ZeroPageIndexedX>()
{
    auto& flags = registers.getP();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    flags.carry = value & 0x1;
    value >>= 1;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 6;
}

/**
 * ROL - Rotate Left 
 */
template <AddressingMode Mode>
inline unsigned Cpu::rol()
{
    static_assert("Unsupported addressing mode used in ROL instruction");
    return 0;
}

template <>
inline unsigned Cpu::rol<AddressingMode::Accumulator>()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    flags.carry = (accumulator >> 7) & 0x1;
    accumulator = (accumulator << 1) | oldCarry;
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);
    return 2;
}

template <>
inline unsigned Cpu::rol<AddressingMode::Absolute>()
{
    auto& flags = registers.getP();
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = (value << 1) | oldCarry;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(address, value);
    return 6;
}

template <>
inline unsigned Cpu::rol<AddressingMode::AbsoluteIndexedX>()
{
    auto& flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = (value << 1) | oldCarry;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::rol<AddressingMode::ZeroPage>()
{
    auto& flags = registers.getP();
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = (value << 1) | oldCarry;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::rol<AddressingMode::ZeroPageIndexedX>()
{
    auto& flags = registers.getP();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = (value << 1) | oldCarry;
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

/**
 * ROR - Rotate Right
 */
template <AddressingMode Mode>
inline unsigned Cpu::ror()
{
    static_assert("Unsupported addressing mode used in ROR instruction");
    return 0;
}

template <>
inline unsigned Cpu::ror<AddressingMode::Accumulator>()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    flags.carry = accumulator & 0x1;
    accumulator = (accumulator >> 1) | (oldCarry << 7);
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);
    return 2;
}

template <>
inline unsigned Cpu::ror<AddressingMode::Absolute>()
{
    auto& flags = registers.getP();
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = (value >> 1) | (oldCarry << 7);
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(address, value);
    return 6;
}

template <>
inline unsigned Cpu::ror<AddressingMode::AbsoluteIndexedX>()
{
    auto& flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = (value >> 1) | (oldCarry << 7);
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::ror<AddressingMode::ZeroPage>()
{
    auto& flags = registers.getP();
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = (value >> 1) | (oldCarry << 7);
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::ror<AddressingMode::ZeroPageIndexedX>()
{
    auto& flags = registers.getP();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = (value >> 1) | (oldCarry << 7);
    updateNegativeFlag(value);
    updateZeroFlag(value);
    mmu->writeIntoMemory(targetAddress, value);
    return 6;
}

/**
 * CMP - Compare memory and Accumulator 
 */
template <AddressingMode Mode>
inline unsigned Cpu::cmp()
{
    static_assert("Unsupported addressing mode used in CMP instruction");
    return 0;
}

template <>
inline unsigned Cpu::cmp<AddressingMode::Immediate>()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto operand = fetchImmedate8();
    flags.carry = accumulator > operand;
    auto result = accumulator - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 2;
}

template <>
inline unsigned Cpu::cmp<AddressingMode::Absolute>()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto address = fetchImmedate16();
    auto operand = mmu->readFromMemory(address);
    flags.carry = accumulator > operand;
    auto result = accumulator - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 4;
}

template <>
inline unsigned Cpu::cmp<AddressingMode::AbsoluteIndexedX>()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    flags.carry = accumulator > operand;
    auto result = accumulator - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::cmp<AddressingMode::AbsoluteIndexedY>()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    flags.carry = accumulator > operand;
    auto result = accumulator - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::cmp<AddressingMode::ZeroPage>()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto address = fetchImmedate8();
    auto operand = mmu->readFromMemory(address);
    flags.carry = accumulator > operand;
    auto result = accumulator - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 3;
}

template <>
inline unsigned Cpu::cmp<AddressingMode::ZeroPageIndexedX>()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress);
    flags.carry = accumulator > operand;
    auto result = accumulator - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 4;
}

template <>
inline unsigned Cpu::cmp<AddressingMode::IndirectX>()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    u16 targetPointerAddress = basePointerAddress + index;
    u16 address = mmu->readFromMemory(targetPointerAddress & 0xFF)
        | mmu->readFromMemory((targetPointerAddress + 1) & 0xFF) << 8;
    auto operand = mmu->readFromMemory(address & 0xFF);
    flags.carry = accumulator > operand;
    auto result = accumulator - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 6;
}

template <>
inline unsigned Cpu::cmp<AddressingMode::IndirectY>()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto index = registers.getY();
    auto pointerAddress = fetchImmedate8();
    u16 baseAddress = mmu->readFromMemory(pointerAddress & 0xFF)
        | mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
    auto targetAddress = baseAddress + index;
    auto operand = mmu->readFromMemory(targetAddress & 0xFF);
    flags.carry = accumulator > operand;
    auto result = accumulator - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * CPX - Compare memory with register X 
 */
template <AddressingMode Mode>
inline unsigned Cpu::cpx()
{
    static_assert("Unsupported addressing mode used in CPX instruction");
    return 0;
}

template <>
inline unsigned Cpu::cpx<AddressingMode::Immediate>()
{
    auto x = registers.getX();
    auto& flags = registers.getP();
    auto operand = fetchImmedate8();
    flags.carry = x > operand;
    auto result = x - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 2;
}

template <>
inline unsigned Cpu::cpx<AddressingMode::Absolute>()
{
    auto x = registers.getX();
    auto& flags = registers.getP();
    auto address = fetchImmedate16();
    auto operand = mmu->readFromMemory(address);
    flags.carry = x > operand;
    auto result = x - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 4;
}

template <>
inline unsigned Cpu::cpx<AddressingMode::ZeroPage>()
{
    auto x = registers.getX();
    auto& flags = registers.getP();
    auto address = fetchImmedate8();
    auto operand = mmu->readFromMemory(address);
    flags.carry = x > operand;
    auto result = x - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 3;
}

/**
 * CPY - Compare memory with register Y 
 */
template <AddressingMode Mode>
inline unsigned Cpu::cpy()
{
    static_assert("Unsupported addressing mode used in CPY instruction");
    return 0;
}

template <>
inline unsigned Cpu::cpy<AddressingMode::Immediate>()
{
    auto y = registers.getY();
    auto& flags = registers.getP();
    auto operand = fetchImmedate8();
    flags.carry = y > operand;
    auto result = y - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 2;
}

template <>
inline unsigned Cpu::cpy<AddressingMode::Absolute>()
{
    auto y = registers.getY();
    auto& flags = registers.getP();
    auto address = fetchImmedate16();
    auto operand = mmu->readFromMemory(address);
    flags.carry = y > operand;
    auto result = y - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 4;
}

template <>
inline unsigned Cpu::cpy<AddressingMode::ZeroPage>()
{
    auto y = registers.getY();
    auto& flags = registers.getP();
    auto address = fetchImmedate8();
    auto operand = mmu->readFromMemory(address);
    flags.carry = y > operand;
    auto result = y - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 3;
}

/**
 * BRK - Break
 */
template <AddressingMode Mode>
inline unsigned Cpu::brk()
{
    static_assert("Unsupported addressing mode used in BRK instruction");
    return 0;
}

template <>
inline unsigned Cpu::brk<AddressingMode::Implied>()
{
    return handleInterrupt(InterruptType::BRK);
}

/**
 * RTI - Return from Interrupt 
 */
template <AddressingMode Mode>
inline unsigned Cpu::rti()
{
    static_assert("Unsupported addressing mode used in RTI instruction");
    return 0;
}

template <>
inline unsigned Cpu::rti<AddressingMode::Implied>()
{
    auto& flags = registers.getP();
    auto& pc = registers.getPc();
    auto flagsFromStack = popFromStack8();
    flags.raw = flagsFromStack & ~(3 << 4);
    pc = popFromStack16();
    return 6;
}

/**
 * NOP - No operation 
 */
template <AddressingMode Mode>
inline unsigned Cpu::nop()
{
    static_assert("Unsupported addressing mode used in NOP instruction");
    return 0;
}

template <>
inline unsigned Cpu::nop<AddressingMode::Implied>()
{
    return 2;
}

/**
 * BIT - Test bits in memory with Accumulator 
 */
template <AddressingMode Mode>
inline unsigned Cpu::bit()
{
    static_assert("Unsupported addressing mode used in BIT instruction");
    return 0;
}

template <>
inline unsigned Cpu::bit<AddressingMode::Absolute>()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto address = fetchImmedate16();
    auto operand = mmu->readFromMemory(address);
    updateNegativeFlag(operand);
    flags.overflow = (operand >> 6) & 0x1;
    auto result = accumulator & operand;
    updateZeroFlag(result);
    return 4;
}

template <>
inline unsigned Cpu::bit<AddressingMode::ZeroPage>()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto address = fetchImmedate8();
    auto operand = mmu->readFromMemory(address);
    updateNegativeFlag(operand);
    flags.overflow = (operand >> 6) & 0x1;
    auto result = accumulator & operand;
    updateZeroFlag(result);
    return 3;
}

/**
 * ALR [Unofficial] - AND oper + LSR
 */
template <AddressingMode Mode>
inline unsigned Cpu::alr()
{
    static_assert("Unsupported addressing mode used in ALR instruction");
    return 0;
}

template <>
inline unsigned Cpu::alr<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto operand = fetchImmedate8();
    accumulator &= operand;
    flags.carry = accumulator & 0x1;
    accumulator >>= 1;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

/**
 * ANC [Unofficial] - AND oper + set carry as ASL/ROL 
 */
template <AddressingMode Mode>
inline unsigned Cpu::anc()
{
    static_assert("Unsupported addressing mode used in ANC instruction");
    return 0;
}

template <>
inline unsigned Cpu::anc<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto operand = fetchImmedate8();
    accumulator &= operand;
    flags.carry = (accumulator >> 7) & 0x1;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

/**
 * XAA [Unofficial] - A and X and oper -> A 
 */
template <AddressingMode Mode>
inline unsigned Cpu::xaa()
{
    static_assert("Unsupported addressing mode used in XAA instruction");
    return 0;
}

template <>
inline unsigned Cpu::xaa<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto x = registers.getX();
    auto operand = fetchImmedate8();
    accumulator = (accumulator | 0xFF) & x & operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

/**
 * ARR [Unofficial] - AND oper + ROR
 */
template <AddressingMode Mode>
inline unsigned Cpu::arr()
{
    static_assert("Unsupported addressing mode used in ARR instruction");
    return 0;
}

template <>
inline unsigned Cpu::arr<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    auto operand = fetchImmedate8();
    accumulator &= operand;
    // Operation involves adder and flag is set according to (A and oper) + oper
    updateOverflowFlag(accumulator, accumulator + operand);
    flags.carry = accumulator & 0x1;
    accumulator = (accumulator >> 1) | (oldCarry << 7);
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 2;
}

/**
 * DCP [Unofficial] - DEC oper + CMP oper
 */
template <AddressingMode Mode>
inline unsigned Cpu::dcp()
{
    static_assert("Unsupported addressing mode used in DCP instruction");
    return 0;
}

template <>
inline unsigned Cpu::dcp<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);

    auto& flags = registers.getP();
    value--;
    auto accumulator = registers.getA();
    s8 result = accumulator - value;
    flags.carry = value > accumulator;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    
    mmu->writeIntoMemory(address, value);
    return 6;
}

template <>
inline unsigned Cpu::dcp<AddressingMode::AbsoluteIndexedX>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index; 
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    value--;
    auto accumulator = registers.getA();
    s8 result = accumulator - value;
    flags.carry = value > accumulator;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    
    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::dcp<AddressingMode::AbsoluteIndexedY>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index; 
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    value--;
    auto accumulator = registers.getA();
    s8 result = accumulator - value;
    flags.carry = value > accumulator;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    
    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::dcp<AddressingMode::ZeroPage>()
{
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);

    auto& flags = registers.getP();
    value--;
    auto accumulator = registers.getA();
    s8 result = accumulator - value;
    flags.carry = value > accumulator;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    
    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::dcp<AddressingMode::ZeroPageIndexedX>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index; 
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    value--;
    auto accumulator = registers.getA();
    s8 result = accumulator - value;
    flags.carry = value > accumulator;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    
    mmu->writeIntoMemory(targetAddress, value);
    return 6;
}

template <>
inline unsigned Cpu::dcp<AddressingMode::IndirectX>()
{
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetPointerAddress = basePointerAddress + index;
    auto address = mmu->readFromMemory(targetPointerAddress) |
        mmu->readFromMemory(targetPointerAddress + 1) << 8;
    auto value = mmu->readFromMemory(address);

    auto& flags = registers.getP();
    value--;
    auto accumulator = registers.getA();
    s8 result = accumulator - value;
    flags.carry = value > accumulator;
    updateZeroFlag(result);
    updateNegativeFlag(result);

    mmu->writeIntoMemory(address, value);
    return 8;
}

template <>
inline unsigned Cpu::dcp<AddressingMode::IndirectY>()
{
    auto pointerAddress = fetchImmedate8();
    auto index = registers.getY();
    auto baseAddress = mmu->readFromMemory(pointerAddress) | 
        mmu->readFromMemory(pointerAddress + 1) << 8;
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    value--;
    auto accumulator = registers.getA();
    s8 result = accumulator - value;
    flags.carry = value > accumulator;
    updateZeroFlag(result);
    updateNegativeFlag(result);

    mmu->writeIntoMemory(targetAddress, value);
    return 8;
}

/**
 * ISC [Unofficial] - INC oper + SBC oper 
 */
template <AddressingMode Mode>
inline unsigned Cpu::isc()
{
    static_assert("Unsupported addressing mode used in ISC instruction");
    return 0;
}

template <>
inline unsigned Cpu::isc<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);

    auto flags = registers.getP();
    auto accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(address, value);
    return 6;
}

template <>
inline unsigned Cpu::isc<AddressingMode::AbsoluteIndexedX>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto flags = registers.getP();
    auto accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::isc<AddressingMode::AbsoluteIndexedY>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto flags = registers.getP();
    auto accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::isc<AddressingMode::ZeroPage>()
{
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);

    auto flags = registers.getP();
    auto accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::isc<AddressingMode::ZeroPageIndexedX>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto flags = registers.getP();
    auto accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(targetAddress, value);
    return 6;
}

template <>
inline unsigned Cpu::isc<AddressingMode::IndirectX>()
{
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetPointerAddress = basePointerAddress + index;
    auto address = mmu->readFromMemory(targetPointerAddress) |
        mmu->readFromMemory(targetPointerAddress + 1) << 8;
    auto value = mmu->readFromMemory(address);

    auto flags = registers.getP();
    auto accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(address, value);
    return 8;
}

template <>
inline unsigned Cpu::isc<AddressingMode::IndirectY>()
{
    auto pointerAddress = fetchImmedate8();
    auto index = registers.getY();
    auto baseAddress = mmu->readFromMemory(pointerAddress) | 
        mmu->readFromMemory(pointerAddress + 1) << 8;
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto flags = registers.getP();
    auto accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(targetAddress, value);
    return 8;
}

/**
 * LAS [Unofficial] - LDA/TSX oper (M AND SP -> A, X, SP)
 */
template <AddressingMode Mode>
inline unsigned Cpu::las()
{
    static_assert("Unsupported addressing mode used in LAS instruction");
    return 0;
}

template <>
inline unsigned Cpu::las<AddressingMode::AbsoluteIndexedY>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    
    auto& sp = registers.getS();
    auto& accumulator = registers.getA();
    auto& x = registers.getX();

    sp &= value;
    x = sp;
    accumulator = sp;

    updateZeroFlag(sp);
    updateNegativeFlag(sp);

    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

/**
 * LAX [Unofficial] - LDA oper + LDX oper
 */
template <AddressingMode Mode>
inline unsigned Cpu::lax()
{
    static_assert("Unsupported addressing mode used in LAX instruction");
    return 0;
}

template <>
inline unsigned Cpu::lax<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);

    auto& accumulator = registers.getA();
    auto& x = registers.getX();
    accumulator = value;
    x = accumulator;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    return 4;
}

template <>
inline unsigned Cpu::lax<AddressingMode::AbsoluteIndexedY>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& accumulator = registers.getA();
    auto& x = registers.getX();
    accumulator = value;
    x = accumulator;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

template <>
inline unsigned Cpu::lax<AddressingMode::ZeroPage>()
{
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);

    auto& accumulator = registers.getA();
    auto& x = registers.getX();
    accumulator = value;
    x = accumulator;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    return 3;
}

template <>
inline unsigned Cpu::lax<AddressingMode::ZeroPageIndexedY>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& accumulator = registers.getA();
    auto& x = registers.getX();
    accumulator = value;
    x = accumulator;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    return 4;
}

template <>
inline unsigned Cpu::lax<AddressingMode::IndirectX>()
{
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetPointerAddress = basePointerAddress + index;
    auto address = mmu->readFromMemory(targetPointerAddress) |
        mmu->readFromMemory(targetPointerAddress + 1) << 8;
    auto value = mmu->readFromMemory(address);

    auto& accumulator = registers.getA();
    auto& x = registers.getX();
    accumulator = value;
    x = accumulator;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    return 6;
}

template <>
inline unsigned Cpu::lax<AddressingMode::IndirectY>()
{
    auto pointerAddress = fetchImmedate8();
    auto index = registers.getY();
    auto baseAddress = mmu->readFromMemory(pointerAddress) |
        mmu->readFromMemory(pointerAddress + 1) << 8;
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& accumulator = registers.getA();
    auto& x = registers.getX();
    accumulator = value;
    x = accumulator;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * LXA [Unofficial] - Store * AND oper in A and X
 */
template <AddressingMode Mode>
inline unsigned Cpu::lxa()
{
    static_assert("Unsupported addressing mode used in LXA instruction");
    return 0; 
}

template <>
inline unsigned Cpu::lxa<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto& x = registers.getX();
    auto operand = fetchImmedate8();
    
    accumulator = (accumulator | 0xFF) & operand;
    x = accumulator;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    
    return 2;
}

/**
 * RLA [Unofficial] - ROL oper + AND oper
 */
template <AddressingMode Mode>
inline unsigned Cpu::rla()
{
    static_assert("Unsupported addressing mode used in RLA instruction");
    return 0;
}

template <>
inline unsigned Cpu::rla<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);
    
    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = value << 1 | oldCarry;
    auto& accumulator = registers.getA();
    accumulator &= value;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(address, value);
    return 6;
}

template <>
inline unsigned Cpu::rla<AddressingMode::AbsoluteIndexedX>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = value << 1 | oldCarry;
    auto& accumulator = registers.getA();
    accumulator &= value;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::rla<AddressingMode::AbsoluteIndexedY>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = value << 1 | oldCarry;
    auto& accumulator = registers.getA();
    accumulator &= value;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::rla<AddressingMode::ZeroPage>()
{
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);
    
    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = value << 1 | oldCarry;
    auto& accumulator = registers.getA();
    accumulator &= value;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::rla<AddressingMode::ZeroPageIndexedX>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = value << 1 | oldCarry;
    auto& accumulator = registers.getA();
    accumulator &= value;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(targetAddress, value);
    return 6;
}

template <>
inline unsigned Cpu::rla<AddressingMode::IndirectX>()
{
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetPointerAddress = basePointerAddress + index;
    auto address = mmu->readFromMemory(targetPointerAddress) |
        mmu->readFromMemory(targetPointerAddress + 1) << 8;
    auto value = mmu->readFromMemory(address);

    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = value << 1 | oldCarry;
    auto& accumulator = registers.getA();
    accumulator &= value;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(address, value);
    return 8;
}

template <>
inline unsigned Cpu::rla<AddressingMode::IndirectY>()
{
    auto pointerAddress = fetchImmedate8();
    auto index = registers.getY();
    auto baseAddress = mmu->readFromMemory(pointerAddress) |
        mmu->readFromMemory(pointerAddress + 1) << 8;
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = value << 1 | oldCarry;
    auto& accumulator = registers.getA();
    accumulator &= value;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(targetAddress, value);
    return 8;
}

/**
 * RRA [Unofficial] - ROR oper + ADC oper
 */
template <AddressingMode Mode>
inline unsigned Cpu::rra()
{
    static_assert("Unsupported addressing mode used in RRA instruction");
    return 0;
}

template <>
inline unsigned Cpu::rra<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);

    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = value >> 1 | (oldCarry << 7);
    u16 result = accumulator + value + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);

    mmu->writeIntoMemory(address, value);
    return 6;
}

template <>
inline unsigned Cpu::rra<AddressingMode::AbsoluteIndexedX>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = value >> 1 | (oldCarry << 7);
    u16 result = accumulator + value + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);
    
    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::rra<AddressingMode::AbsoluteIndexedY>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = value >> 1 | (oldCarry << 7);
    u16 result = accumulator + value + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);
    
    mmu->writeIntoMemory(targetAddress, value);
    return 7;
}

template <>
inline unsigned Cpu::rra<AddressingMode::ZeroPage>()
{
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);

    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = value >> 1 | (oldCarry << 7);
    u16 result = accumulator + value + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);

    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::rra<AddressingMode::ZeroPageIndexedX>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = value >> 1 | (oldCarry << 7);
    u16 result = accumulator + value + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);
    
    mmu->writeIntoMemory(targetAddress, value);
    return 6;
}

template <>
inline unsigned Cpu::rra<AddressingMode::IndirectX>()
{
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetPointerAddress = basePointerAddress + index;
    auto address = mmu->readFromMemory(targetPointerAddress) |
        mmu->readFromMemory(targetPointerAddress + 1) << 8;
    auto value = mmu->readFromMemory(address);

    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = value >> 1 | (oldCarry << 7);
    u16 result = accumulator + value + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);

    mmu->writeIntoMemory(address, value);
    return 8;
}

template <>
inline unsigned Cpu::rra<AddressingMode::IndirectY>()
{
    auto pointerAddress = fetchImmedate8();
    auto index = registers.getY();
    auto baseAddress = mmu->readFromMemory(pointerAddress) |
        mmu->readFromMemory(pointerAddress + 1) << 8;
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);

    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = value >> 1 | (oldCarry << 7);
    u16 result = accumulator + value + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlag(accumulator, result);
    accumulator = result & 0xFF;
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);

    mmu->writeIntoMemory(targetAddress, value);
    return 8;
}

/**
 * SAX [Unofficial] - A and X are put on the bus at the same time (resulting effectively in an AND operation) and stored in M. 
 */
template <AddressingMode Mode>
inline unsigned Cpu::sax()
{
    static_assert("Unsupported addressing mode used in SAX instruction");
    return 0;
}

template <>
inline unsigned Cpu::sax<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    
    auto accumulator = registers.getA();
    auto x = registers.getX();
    auto result = accumulator & x;

    mmu->writeIntoMemory(address, result);
    return 4;
}

template <>
inline unsigned Cpu::sax<AddressingMode::ZeroPage>()
{
    auto address = fetchImmedate16();
    
    auto accumulator = registers.getA();
    auto x = registers.getX();
    auto result = accumulator & x;

    mmu->writeIntoMemory(address, result);
    return 3;
}

template <>
inline unsigned Cpu::sax<AddressingMode::ZeroPageIndexedY>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;

    auto accumulator = registers.getA();
    auto x = registers.getX();
    auto result = accumulator & x;

    mmu->writeIntoMemory(targetAddress, result);
    return 4;
}

template <>
inline unsigned Cpu::sax<AddressingMode::IndirectX>()
{
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetPointerAddress = basePointerAddress + index;
    auto address = mmu->readFromMemory(targetPointerAddress) | 
        mmu->readFromMemory(targetPointerAddress + 1) << 8;

    auto accumulator = registers.getA();
    auto x = registers.getX();
    auto result = accumulator & x;

    mmu->writeIntoMemory(address, result);
    return 6;
}

/**
 * AXS [Unofficial] - CMP and DEX at once, sets flags like CMP
 */
template <AddressingMode Mode>
inline unsigned Cpu::axs()
{
    static_assert("Unsupported addressing mode used in AXS instruction");
    return 0;
}

template <>
inline unsigned Cpu::axs<AddressingMode::Immediate>()
{
    auto& x = registers.getX();
    auto& flags = registers.getP();
    auto accumulator = registers.getA();
    auto operand = fetchImmedate8();

    auto intermediate = accumulator & x;
    flags.carry = operand > intermediate;
    x = intermediate - operand;
    updateZeroFlag(x);
    updateNegativeFlag(x);

    return 2;
}

/**
 * AHX [Unofficial] - Stores A AND X AND (high-byte of addr. + 1) at addr.
 */
template <AddressingMode Mode>
inline unsigned Cpu::ahx()
{
    static_assert("Unsupported addressing mode used in AHX instruction");
    return 0;
}

template <>
inline unsigned Cpu::ahx<AddressingMode::AbsoluteIndexedY>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;

    auto accumulator = registers.getA();
    auto x = registers.getX();
    auto result = accumulator & x & (((targetAddress >> 8) + 1) & 0xFF);

    mmu->writeIntoMemory(targetAddress, result);
    return 5;
}

template <>
inline unsigned Cpu::ahx<AddressingMode::IndirectY>()
{
    auto pointerAddress = fetchImmedate8();
    auto index = registers.getY();
    auto baseAddress = mmu->readFromMemory(pointerAddress) |
        mmu->readFromMemory(pointerAddress + 1) << 8;
    auto targetAddress = baseAddress + index;

    auto accumulator = registers.getA();
    auto x = registers.getX();
    auto result = accumulator & x & (((targetAddress >> 8) + 1) & 0xFF);

    mmu->writeIntoMemory(targetAddress, result);
    return 6;
}

/**
 * SHX [Unofficial] - Stores X AND (high-byte of addr. + 1) at addr.
 */
template <AddressingMode Mode>
inline unsigned Cpu::shx()
{
    static_assert("Unsupported addressing mode used in SHX instruction");
    return 0;
}

template <>
inline unsigned Cpu::shx<AddressingMode::AbsoluteIndexedY>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto x = registers.getX();

    u8 result = x & (((targetAddress >> 8) & 0xFF) + 1);
    mmu->writeIntoMemory(targetAddress, result);
    return 5;
}

/**
 * SHY [Unofficial] - Stores Y AND (high-byte of addr. + 1) at addr.
 */
template <AddressingMode Mode>
inline unsigned Cpu::shy()
{
    static_assert("Unsupported addressing mode used in SHY instruction");
    return 0;
}

template <>
inline unsigned Cpu::shy<AddressingMode::AbsoluteIndexedX>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto y = registers.getY();
    
    u8 result = y & (((targetAddress >> 8) & 0xFF) + 1);
    mmu->writeIntoMemory(targetAddress, result);
    return 5;
}

/**
 * SLO [Unofficial] - ASL oper + ORA oper 
 */
template <AddressingMode Mode>
inline unsigned Cpu::slo()
{
    static_assert("Unsupported addressing mode used in SLO instruction");
    return 0;
}

template <>
inline unsigned Cpu::slo<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    accumulator = accumulator | value;
    return 6;
}

template <>
inline unsigned Cpu::slo<AddressingMode::AbsoluteIndexedX>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    accumulator = accumulator | value;
    return 7;
}

template <>
inline unsigned Cpu::slo<AddressingMode::AbsoluteIndexedY>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    accumulator = accumulator | value;
    return 7;
}

template <>
inline unsigned Cpu::slo<AddressingMode::ZeroPage>()
{
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    accumulator = accumulator | value;
    return 5;
}

template <>
inline unsigned Cpu::slo<AddressingMode::ZeroPageIndexedX>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    accumulator = accumulator | value;
    return 6;
}

template <>
inline unsigned Cpu::slo<AddressingMode::IndirectX>()
{
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetPointerAddress = basePointerAddress + index;
    auto address = mmu->readFromMemory(targetPointerAddress) |
        mmu->readFromMemory(targetPointerAddress + 1) << 8;
    auto value = mmu->readFromMemory(address);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    accumulator = accumulator | value;
    return 8;
}

template <>
inline unsigned Cpu::slo<AddressingMode::IndirectY>()
{
    auto pointerAddress = fetchImmedate8();
    auto index = registers.getY();
    auto baseAddress = mmu->readFromMemory(pointerAddress) |
        mmu->readFromMemory(pointerAddress + 1) << 8;
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    accumulator = accumulator | value;
    return 8;
}

/**
 * SRE [Unofficial] - LSR oper + EOR oper
 */
template <AddressingMode Mode>
inline unsigned Cpu::sre()
{
    static_assert("Unsupported addressing mode used in SRE instruction");
    return 0;
}

template <>
inline unsigned Cpu::sre<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = value & 0x1;
    value >>= 1;
    accumulator = accumulator ^ value;
    return 6;
}

template <>
inline unsigned Cpu::sre<AddressingMode::AbsoluteIndexedX>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = value & 0x1;
    value >>= 1;
    accumulator = accumulator ^ value;
    return 7;
}

template <>
inline unsigned Cpu::sre<AddressingMode::AbsoluteIndexedY>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = value & 0x1;
    value >>= 1;
    accumulator = accumulator ^ value;
    return 7;
}

template <>
inline unsigned Cpu::sre<AddressingMode::ZeroPage>()
{
    auto address = fetchImmedate8();
    auto value = mmu->readFromMemory(address);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = value & 0x1;
    value >>= 1;
    accumulator = accumulator ^ value;
    return 5;
}

template <>
inline unsigned Cpu::sre<AddressingMode::ZeroPageIndexedX>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = value & 0x1;
    value >>= 1;
    accumulator = accumulator ^ value;
    return 6;
}

template <>
inline unsigned Cpu::sre<AddressingMode::IndirectX>()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    u16 targetPointerAddress = basePointerAddress + index;
    u16 address = mmu->readFromMemory(targetPointerAddress & 0xFF)
        | mmu->readFromMemory((targetPointerAddress + 1) & 0xFF) << 8;
    auto value = mmu->readFromMemory(address);
    flags.carry = value & 0x1;
    value >>= 1;
    accumulator = accumulator ^ value;
    return 8;
}

template <>
inline unsigned Cpu::sre<AddressingMode::IndirectY>()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto index = registers.getY();
    auto pointerAddress = fetchImmedate8();
    u16 baseAddress = mmu->readFromMemory(pointerAddress & 0xFF)
        | mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress);
    flags.carry = value & 0x1;
    value >>= 1;
    accumulator = accumulator ^ value;
    return 8;
}

/**
 * TAS [Unofficial] - Puts A AND X in SP and stores A AND X AND (high-byte of addr. + 1) at addr.
 */
template <AddressingMode Mode>
inline unsigned Cpu::tas()
{
    static_assert("Unsupported addressing mode used in TAS instruction");
    return 0;
}

template <>
inline unsigned Cpu::tas<AddressingMode::AbsoluteIndexedY>()
{
    auto& sp = registers.getS();
    auto accumulator = registers.getA();
    auto x = registers.getX();
    sp = accumulator & x;

    auto address = fetchImmedate16();
    auto y = registers.getY();
    auto value = accumulator & x & ((address >> 8) + 1);
    mmu->writeIntoMemory(address, value);
    return 5;
}

/**
 * USBC [Unofficial] - Combination of SBC and NOP 
 */
template <AddressingMode Mode>
inline unsigned Cpu::usbc()
{
    static_assert("Unsupported addressing mode used in USBC instruction");
    return 0;
}

template <>
inline unsigned Cpu::usbc<AddressingMode::Immediate>()
{
    return sbc<AddressingMode::Immediate>();
}

/**
 * NOP [Unofficial] - Unofficial variants of NOP
 */
template <>
inline unsigned Cpu::nop<AddressingMode::Immediate>()
{
    fetchImmedate8();
    return 2;
}

template <>
inline unsigned Cpu::nop<AddressingMode::ZeroPage>()
{
    fetchImmedate8();
    return 3;
}

template <>
inline unsigned Cpu::nop<AddressingMode::ZeroPageIndexedX>()
{
    fetchImmedate8();
    return 4;
}

template <>
inline unsigned Cpu::nop<AddressingMode::Absolute>()
{
    fetchImmedate16();
    return 4;
}

template <>
inline unsigned Cpu::nop<AddressingMode::AbsoluteIndexedX>()
{
    auto baseAddress = fetchImmedate16();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    return checkForPageCross(baseAddress, targetAddress) ? 5 : 4;
}

/**
 * STP [Unofficial] - Freeze the CPU 
 */
template <AddressingMode Mode>
inline unsigned Cpu::stp()
{
    static_assert("Unsupported addressing mode used in STP instruction");
    return 0;
}

template <>
inline unsigned Cpu::stp<AddressingMode::Implied>()
{
    halted = true;
    return 0;
}