/**
 * LDA - Load Accumulator
 */
template <AddressingMode Mode>
inline unsigned Cpu::lda() = delete;

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
inline unsigned Cpu::ldx() = delete;

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
inline unsigned Cpu::ldy() = delete;

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
inline unsigned Cpu::sta() = delete;

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
inline unsigned Cpu::stx() = delete;

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
inline unsigned Cpu::sty() = delete;

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
inline unsigned Cpu::tax() = delete;

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
inline unsigned Cpu::tay() = delete;

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
inline unsigned Cpu::tsx() = delete;

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
inline unsigned Cpu::txa() = delete;

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
inline unsigned Cpu::txs() = delete;

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
inline unsigned Cpu::tya() = delete;

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
inline unsigned Cpu::dec() = delete;

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
inline unsigned Cpu::dex() = delete;

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
inline unsigned Cpu::dey() = delete;

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
inline unsigned Cpu::inc() = delete;

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
inline unsigned Cpu::inx() = delete;

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
inline unsigned Cpu::iny() = delete;

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
inline unsigned Cpu::clc() = delete;

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
inline unsigned Cpu::cld() = delete;

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
inline unsigned Cpu::cli() = delete;

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
inline unsigned Cpu::clv() = delete;

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
inline unsigned Cpu::sec() = delete;

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
inline unsigned Cpu::sed() = delete;

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
inline unsigned Cpu::sei() = delete;

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
inline unsigned Cpu::pha() = delete;

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
inline unsigned Cpu::php() = delete;

template <>
inline unsigned Cpu::php<AddressingMode::Implied>()
{
    auto flags = registers.getP();
    auto flagsToPush = flags.raw |= 0x3 << 4;
    pushIntoStack8(flagsToPush);
    return 3;
}

/**
 * PLA - Pull accumulator from stack 
 */
template <AddressingMode Mode>
inline unsigned Cpu::pla() = delete;

template <>
inline unsigned Cpu::pla<AddressingMode::Implied>()
{
    auto& accumulator = registers.getA();
    accumulator = popFromStack8();
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return 4;
}

/**
 * PLP - Pull processor status from stack 
 */
template <AddressingMode Mode>
inline unsigned Cpu::plp() = delete;

template <>
inline unsigned Cpu::plp<AddressingMode::Implied>()
{
    auto& flags = registers.getP();
    flags &= (1 << 5);
    flags |= (popFromStack8() & ~(1 << 4));
    return 4;
}


/**
 * JMP - Jump
 */
template <AddressingMode Mode>
inline unsigned Cpu::jmp() = delete;

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
    u16 page = pointer & 0xFF00;
    u16 address = mmu->readFromMemory(pointer)
        | mmu->readFromMemory(page | ((pointer + 1) & 0xFF)) << 8;
    pc = address;
    return 5;
}

/**
 * JSR - Jump to subroutine 
 */
template <AddressingMode Mode>
inline unsigned Cpu::jsr() = delete;

template <>
inline unsigned Cpu::jsr<AddressingMode::Absolute>()
{
    auto& pc = registers.getPc();
    auto address = fetchImmedate16();
    pushIntoStack16(pc - 1);
    pc = address;
    return 6;
}

/**
 * RTS - Return from subroutine
 */
template <AddressingMode Mode>
inline unsigned Cpu::rts() = delete;

template <>
inline unsigned Cpu::rts<AddressingMode::Implied>()
{
    auto& pc = registers.getPc();
    pc = popFromStack16() + 1;
    return 6;
}

/**
 * ADC - Add memory to Accumulator with Carry 
 */
template <AddressingMode Mode>
inline unsigned Cpu::adc() = delete;

template <>
inline unsigned Cpu::adc<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto operand = fetchImmedate8();
    u16 result = accumulator + operand + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlagAdc(accumulator, operand, result);
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
    updateOverflowFlagAdc(accumulator, operand, result);
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
    updateOverflowFlagAdc(accumulator, operand, result);
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
    updateOverflowFlagAdc(accumulator, operand, result);
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
    updateOverflowFlagAdc(accumulator, operand, result);
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
    updateOverflowFlagAdc(accumulator, operand, result);
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
    auto operand = mmu->readFromMemory(address);
    u16 result = accumulator + operand + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlagAdc(accumulator, operand, result);
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
    auto operand = mmu->readFromMemory(targetAddress);
    u16 result = accumulator + operand + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlagAdc(accumulator, operand, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * SBC - Subtract memory from accumulator with borrow
 */
template <AddressingMode Mode>
inline unsigned Cpu::sbc() = delete;

template <>
inline unsigned Cpu::sbc<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto flags = registers.getP();
    auto operand = fetchImmedate8();
    u16 result = accumulator - operand - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, operand, result);
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
    u16 result = accumulator - operand - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, operand, result);
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
    u16 result = accumulator - operand - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, operand, result);
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
    u16 result = accumulator - operand - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, operand, result);
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
    u16 result = accumulator - operand - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, operand, result);
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
    u16 result = accumulator - operand - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, operand, result);
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
    auto operand = mmu->readFromMemory(address);
    u16 result = accumulator - operand - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, operand, result);
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
    auto operand = mmu->readFromMemory(targetAddress);
    u16 result = accumulator - operand - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, operand, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * AND - AND memory with Accumulator 
 */
template <AddressingMode Mode>
inline unsigned Cpu::_and() = delete;

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
    auto operand = mmu->readFromMemory(address);
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
    auto operand = mmu->readFromMemory(targetAddress);
    accumulator &= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * EOR - Exclusive OR memory with Accumulator 
 */
template <AddressingMode Mode>
inline unsigned Cpu::eor() = delete;

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
    auto operand = mmu->readFromMemory(address);
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
    auto operand = mmu->readFromMemory(targetAddress);
    accumulator ^= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * ORA - OR memory with Accumulator 
 */
template <AddressingMode Mode>
inline unsigned Cpu::ora() = delete;

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
    auto operand = mmu->readFromMemory(address);
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
    auto operand = mmu->readFromMemory(targetAddress);
    accumulator |= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * BCC - Branch on Carry Clear 
 */
template <AddressingMode Mode>
inline unsigned Cpu::bcc() = delete;

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
inline unsigned Cpu::bcs() = delete;

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
inline unsigned Cpu::beq() = delete;

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
inline unsigned Cpu::bmi() = delete;

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
inline unsigned Cpu::bne() = delete;

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
inline unsigned Cpu::bpl() = delete;

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
inline unsigned Cpu::bvc() = delete;

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
inline unsigned Cpu::bvs() = delete;

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
inline unsigned Cpu::asl() = delete;

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
inline unsigned Cpu::lsr() = delete;

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
inline unsigned Cpu::rol() = delete;

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
    return 6;
}

/**
 * ROR - Rotate Right
 */
template <AddressingMode Mode>
inline unsigned Cpu::ror() = delete;

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
inline unsigned Cpu::cmp() = delete;

template <>
inline unsigned Cpu::cmp<AddressingMode::Immediate>()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto operand = fetchImmedate8();
    flags.carry = operand <= accumulator;
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
    flags.carry = operand <= accumulator;
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
    flags.carry = operand <= accumulator;
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
    flags.carry = operand <= accumulator;
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
    flags.carry = operand <= accumulator;
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
    flags.carry = operand <= accumulator;
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
    auto operand = mmu->readFromMemory(address);
    flags.carry = operand <= accumulator;
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
    auto operand = mmu->readFromMemory(targetAddress);
    flags.carry = operand <= accumulator;
    auto result = accumulator - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return checkForPageCross(baseAddress, targetAddress) ? 6 : 5;
}

/**
 * CPX - Compare memory with register X 
 */
template <AddressingMode Mode>
inline unsigned Cpu::cpx() = delete;

template <>
inline unsigned Cpu::cpx<AddressingMode::Immediate>()
{
    auto x = registers.getX();
    auto& flags = registers.getP();
    auto operand = fetchImmedate8();
    flags.carry = operand <= x;
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
    flags.carry = operand <= x;
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
    flags.carry = operand <= x;
    auto result = x - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 3;
}

/**
 * CPY - Compare memory with register Y 
 */
template <AddressingMode Mode>
inline unsigned Cpu::cpy() = delete;

template <>
inline unsigned Cpu::cpy<AddressingMode::Immediate>()
{
    auto y = registers.getY();
    auto& flags = registers.getP();
    auto operand = fetchImmedate8();
    flags.carry = operand <= y;
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
    flags.carry = operand <= y;
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
    flags.carry = operand <= y;
    auto result = y - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    return 3;
}

/**
 * BRK - Break
 */
template <AddressingMode Mode>
inline unsigned Cpu::brk() = delete;

template <>
inline unsigned Cpu::brk<AddressingMode::Implied>()
{
    return handleInterrupt(InterruptType::BRK);
}

/**
 * RTI - Return from Interrupt 
 */
template <AddressingMode Mode>
inline unsigned Cpu::rti() = delete;

template <>
inline unsigned Cpu::rti<AddressingMode::Implied>()
{
    auto& flags = registers.getP();
    auto& pc = registers.getPc();
    auto oldFlags = flags;
    flags.raw = popFromStack8() | oldFlags & 0x20;
    pc = popFromStack16();
    return 6;
}

/**
 * NOP - No operation 
 */
template <AddressingMode Mode>
inline unsigned Cpu::nop() = delete;

template <>
inline unsigned Cpu::nop<AddressingMode::Implied>()
{
    return 2;
}

/**
 * BIT - Test bits in memory with Accumulator 
 */
template <AddressingMode Mode>
inline unsigned Cpu::bit() = delete;

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
inline unsigned Cpu::alr() = delete;

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
inline unsigned Cpu::anc() = delete;

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
inline unsigned Cpu::xaa() = delete;

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
inline unsigned Cpu::arr() = delete;

template <>
inline unsigned Cpu::arr<AddressingMode::Immediate>()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    auto operand = fetchImmedate8();
    accumulator &= operand;
    // Operation involves adder and flag is set according to (A and oper) + oper
    updateOverflowFlagAdc(accumulator, operand, accumulator + operand);
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
inline unsigned Cpu::dcp() = delete;

template <>
inline unsigned Cpu::dcp<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);

    auto& flags = registers.getP();
    value--;
    auto accumulator = registers.getA();
    s8 result = accumulator - value;
    flags.carry = value <= accumulator;
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
    flags.carry = value <= accumulator;
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
    flags.carry = value <= accumulator;
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
    flags.carry = value <= accumulator;
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
    auto value = mmu->readFromMemory(targetAddress & 0xFF);

    auto& flags = registers.getP();
    value--;
    auto accumulator = registers.getA();
    s8 result = accumulator - value;
    flags.carry = value <= accumulator;
    updateZeroFlag(result);
    updateNegativeFlag(result);
    
    mmu->writeIntoMemory(targetAddress & 0xFF, value);
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
    flags.carry = value <= accumulator;
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
    flags.carry = value <= accumulator;
    updateZeroFlag(result);
    updateNegativeFlag(result);

    mmu->writeIntoMemory(targetAddress, value);
    return 8;
}

/**
 * ISC [Unofficial] - INC oper + SBC oper 
 */
template <AddressingMode Mode>
inline unsigned Cpu::isc() = delete;

template <>
inline unsigned Cpu::isc<AddressingMode::Absolute>()
{
    auto address = fetchImmedate16();
    auto value = mmu->readFromMemory(address);

    auto flags = registers.getP();
    auto& accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, value, result);
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
    auto& accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, value, result);
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
    auto& accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, value, result);
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
    auto& accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, value, result);
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
    auto value = mmu->readFromMemory(targetAddress & 0xFF);

    auto flags = registers.getP();
    auto& accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, value, result);
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(targetAddress & 0xFF, value);
    return 6;
}

template <>
inline unsigned Cpu::isc<AddressingMode::IndirectX>()
{
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetPointerAddress = basePointerAddress + index;
    auto address = mmu->readFromMemory(targetPointerAddress & 0xFF) |
        mmu->readFromMemory((targetPointerAddress + 1) & 0xFF) << 8;
    auto value = mmu->readFromMemory(address);

    auto flags = registers.getP();
    auto& accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, value, result);
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
    auto& accumulator = registers.getA();
    value++;
    u16 result = accumulator - value - !flags.carry;
    updateCarryFlag(result, true);
    updateOverflowFlagSbc(accumulator, value, result);
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
inline unsigned Cpu::las() = delete;

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
inline unsigned Cpu::lax() = delete;

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
    auto value = mmu->readFromMemory(targetAddress & 0xFF);

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
inline unsigned Cpu::lxa() = delete;

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
inline unsigned Cpu::rla() = delete;

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
    auto value = mmu->readFromMemory(targetAddress & 0xFF);

    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    flags.carry = (value >> 7) & 0x1;
    value = value << 1 | oldCarry;
    auto& accumulator = registers.getA();
    accumulator &= value;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);

    mmu->writeIntoMemory(targetAddress & 0xFF, value);
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
inline unsigned Cpu::rra() = delete;

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
    updateOverflowFlagAdc(accumulator, value, result);
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
    updateOverflowFlagAdc(accumulator, value, result);
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
    updateOverflowFlagAdc(accumulator, value, result);
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
    updateOverflowFlagAdc(accumulator, value, result);
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
    auto value = mmu->readFromMemory(targetAddress & 0xFF);

    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    auto oldCarry = flags.carry;
    flags.carry = value & 0x1;
    value = value >> 1 | (oldCarry << 7);
    u16 result = accumulator + value + flags.carry;
    updateCarryFlag(result);
    updateOverflowFlagAdc(accumulator, value, result);
    accumulator = result & 0xFF;
    updateNegativeFlag(accumulator);
    updateZeroFlag(accumulator);
    
    mmu->writeIntoMemory(targetAddress & 0xFF, value);
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
    updateOverflowFlagAdc(accumulator, value, result);
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
    updateOverflowFlagAdc(accumulator, value, result);
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
inline unsigned Cpu::sax() = delete;

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
    auto address = fetchImmedate8();
    
    auto accumulator = registers.getA();
    auto x = registers.getX();
    auto result = accumulator & x;

    mmu->writeIntoMemory(address, result);
    return 3;
}

template <>
inline unsigned Cpu::sax<AddressingMode::ZeroPageIndexedY>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getY();
    auto targetAddress = baseAddress + index;

    auto accumulator = registers.getA();
    auto x = registers.getX();
    auto result = accumulator & x;

    mmu->writeIntoMemory(targetAddress & 0xFF, result);
    return 4;
}

template <>
inline unsigned Cpu::sax<AddressingMode::IndirectX>()
{
    auto basePointerAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetPointerAddress = basePointerAddress + index;
    auto address = mmu->readFromMemory(targetPointerAddress & 0xFF) | 
        mmu->readFromMemory((targetPointerAddress + 1) & 0xFF) << 8;

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
inline unsigned Cpu::axs() = delete;

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
inline unsigned Cpu::ahx() = delete;

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
inline unsigned Cpu::shx() = delete;

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
inline unsigned Cpu::shy() = delete;

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
inline unsigned Cpu::slo() = delete;

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
    mmu->writeIntoMemory(address, value);
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
    mmu->writeIntoMemory(targetAddress, value);
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
    mmu->writeIntoMemory(targetAddress, value);
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
    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::slo<AddressingMode::ZeroPageIndexedX>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress & 0xFF);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = (value >> 7) & 0x1;
    value <<= 1;
    accumulator = accumulator | value;
    mmu->writeIntoMemory(targetAddress & 0xFF, value);
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
    mmu->writeIntoMemory(address, value);
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
    mmu->writeIntoMemory(targetAddress, value);
    return 8;
}

/**
 * SRE [Unofficial] - LSR oper + EOR oper
 */
template <AddressingMode Mode>
inline unsigned Cpu::sre() = delete;

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
    mmu->writeIntoMemory(address, value);
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
    mmu->writeIntoMemory(targetAddress, value);
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
    mmu->writeIntoMemory(targetAddress, value);
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
    mmu->writeIntoMemory(address, value);
    return 5;
}

template <>
inline unsigned Cpu::sre<AddressingMode::ZeroPageIndexedX>()
{
    auto baseAddress = fetchImmedate8();
    auto index = registers.getX();
    auto targetAddress = baseAddress + index;
    auto value = mmu->readFromMemory(targetAddress & 0xFF);
    auto& flags = registers.getP();
    auto& accumulator = registers.getA();
    flags.carry = value & 0x1;
    value >>= 1;
    accumulator = accumulator ^ value;
    mmu->writeIntoMemory(targetAddress & 0xFF, value);
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
    mmu->writeIntoMemory(address, value);
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
    mmu->writeIntoMemory(targetAddress, value);
    return 8;
}

/**
 * TAS [Unofficial] - Puts A AND X in SP and stores A AND X AND (high-byte of addr. + 1) at addr.
 */
template <AddressingMode Mode>
inline unsigned Cpu::tas() = delete;

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
inline unsigned Cpu::usbc() = delete;

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
inline unsigned Cpu::stp() = delete;

template <>
inline unsigned Cpu::stp<AddressingMode::Implied>()
{
    halted = true;
    return 0;
}