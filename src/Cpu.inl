template <AddressingMode Mode>
inline u8 Cpu::resolveReadOperand()
{
    using enum AddressingMode;

    constexpr auto isSupportedMode = Mode == Immediate 
        || isAbsolute(Mode) || isZeroPage(Mode) 
        || isIndexedIndirect(Mode) || isIndirectIndexed(Mode);
    static_assert(isSupportedMode, "Unsupported addressing mode used for resolving Read Instruction operand");

    if constexpr (Mode == Absolute) {
        auto address = fetchImmedate16();
        return mmu->readFromMemory(address);
    }

    if constexpr (Mode == AbsoluteIndexedX) {
        auto address = fetchImmedate16();
        auto index = registers.getX();
        auto effectiveAddress = address + index;
        auto result = mmu->readFromMemory((address & 0xFF00) | (effectiveAddress & 0xFF));
        if((address & 0xFF00) != (effectiveAddress & 0xFF00)) {
            result = mmu->readFromMemory(effectiveAddress);
        }
        return result;
    }

    if constexpr (Mode == AbsoluteIndexedY) {
        auto address = fetchImmedate16();
        auto index = registers.getY();
        auto effectiveAddress = address + index;
        auto result = mmu->readFromMemory((address & 0xFF00) | (effectiveAddress & 0xFF));
        if((address & 0xFF00) != (effectiveAddress & 0xFF00)) {
            result = mmu->readFromMemory(effectiveAddress);
        }
        return result;
    }

    if constexpr (Mode == ZeroPage) {
        auto address = fetchImmedate8();
        return mmu->readFromMemory(address);
    }

    if constexpr (Mode == ZeroPageIndexedX) {
        auto address = fetchImmedate8();
        auto index = registers.getX();
        auto effectiveAddress = (address + index) & 0xFF;
        mmu->readFromMemory(address);
        return mmu->readFromMemory(effectiveAddress);
    }

    if constexpr (Mode == ZeroPageIndexedY) {
        auto address = fetchImmedate8();
        auto index = registers.getY();
        auto effectiveAddress = (address + index) & 0xFF;
        mmu->readFromMemory(address);
        return mmu->readFromMemory(effectiveAddress);
    }

    if constexpr (Mode == IndirectX) {
        auto pointerAddress = fetchImmedate8();
        mmu->readFromMemory(pointerAddress);
        auto index = registers.getX();
        auto effectivePointerAddress = pointerAddress + index;
        u16 address = mmu->readFromMemory(effectivePointerAddress & 0xFF) |
            mmu->readFromMemory((effectivePointerAddress + 1) & 0xFF) << 8;
        return mmu->readFromMemory(address);
    }

    if constexpr (Mode == IndirectY) {
        auto pointerAddress = fetchImmedate8();
        u16 address = mmu->readFromMemory(pointerAddress) |
            mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
        auto index = registers.getY();
        auto effectiveAddress = address + index;
        auto result = mmu->readFromMemory((address & 0xFF00) | (effectiveAddress & 0xFF));
        if ((address & 0xFF00) != (effectiveAddress & 0xFF00)) {
            result = mmu->readFromMemory(effectiveAddress);
        }
        return result;
    }

    // Immediate
    return fetchImmedate8();
}

template <AddressingMode Mode>
inline u16 Cpu::resolveWriteAddress()
{
    using enum AddressingMode;
    constexpr auto isSupportedMode = isAbsolute(Mode) 
        || isZeroPage(Mode) || isIndexedIndirect(Mode) || isIndirectIndexed(Mode);
    static_assert(isSupportedMode, "Unsupported addressing mode used for resolving Write Instruction address");

    if constexpr (Mode == AbsoluteIndexedX) {
        auto address = fetchImmedate16();
        auto index = registers.getX();
        auto effectiveAddress = address + index;
        mmu->readFromMemory((address & 0xFF00) | (effectiveAddress & 0xFF));
        return effectiveAddress;
    }

    if constexpr (Mode == AbsoluteIndexedY) {
        auto address = fetchImmedate16();
        auto index = registers.getY();
        auto effectiveAddress = address + index;
        mmu->readFromMemory((address & 0xFF00) | (effectiveAddress & 0xFF));
        return effectiveAddress;
    }

    if constexpr (Mode == ZeroPage) {
        return fetchImmedate8();
    }

    if constexpr (Mode == ZeroPageIndexedX) {
        auto address = fetchImmedate8();
        auto index = registers.getX();
        mmu->readFromMemory(address);
        return (address + index) & 0xFF;
    }

    if constexpr (Mode == ZeroPageIndexedY) {
        auto address = fetchImmedate8();
        auto index = registers.getY();
        mmu->readFromMemory(address);
        return (address + index) & 0xFF;
    }

    if constexpr (Mode == IndirectX) {
        auto pointerAddress = fetchImmedate8();
        mmu->readFromMemory(pointerAddress);
        auto index = registers.getX();
        auto effectivePointerAddress = pointerAddress + index;
        u16 address = mmu->readFromMemory(effectivePointerAddress & 0xFF) |
            mmu->readFromMemory((effectivePointerAddress + 1) & 0xFF) << 8;
        return address;
    }

    if constexpr (Mode == IndirectY) {
        auto pointerAddress = fetchImmedate8();
        u16 address = mmu->readFromMemory(pointerAddress) |
            mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
        auto index = registers.getY();
        auto effectiveAddress = address + index;
        mmu->readFromMemory((address & 0xFF00) | (effectiveAddress & 0xFF));
        return effectiveAddress;
    }

    // Absolute (non-indexed)
    return fetchImmedate16();
}

template <AddressingMode Mode>
inline void Cpu::executeImplied(const std::function<void()> &op)
{
    static_assert(Mode == AddressingMode::Implied, "Addressing mode other than Implied used in implied instruction");
    mmu->readFromMemory(registers.getPc());
    op();
}

template <AddressingMode Mode>
inline void Cpu::executeBranchInstruction(bool condition)
{
    static_assert(Mode == AddressingMode::Relative, "Branch instructions only support Relative addressing");
    auto& pc = registers.getPc();
    s8 offset = fetchImmedate8();
    auto oldPc = pc;
    if(condition) {
        mmu->readFromMemory(pc);
        pc += offset;
    }
    if((oldPc & 0xFF00) != (pc & 0xFF00)) {
        mmu->readFromMemory(pc);
    }
}

template <AddressingMode Mode>
inline void Cpu::executeReadModifyWrite(const std::function<void(u8&)> &op)
{
    using enum AddressingMode;
    constexpr bool isSupportedMode = Mode == Accumulator
        || isAbsolute(Mode) || isZeroPage(Mode) 
        || isIndexedIndirect(Mode) || isIndirectIndexed(Mode);
    static_assert(isSupportedMode, "Unsupported addressing mode used for performing Read-Modify-Write instruction");
    
    if constexpr (Mode == Accumulator) {
        mmu->readFromMemory(registers.getPc());
        auto& accumulator = registers.getA();
        op(accumulator);
        return;
    }
    
    u16 address = 0;
    u8 value = 0;
    if constexpr (Mode == Absolute) {
        address = fetchImmedate16();
        value = mmu->readFromMemory(address);
    }
    if constexpr (Mode == ZeroPage) {
        address = fetchImmedate8();
        value = mmu->readFromMemory(address);
    }
    if constexpr (Mode == AbsoluteIndexedX) {
        auto baseAddress = fetchImmedate16();
        auto index = registers.getX();
        address = baseAddress + index;
        value = mmu->readFromMemory((baseAddress & 0xFF00) | (address & 0xFF));
        value = mmu->readFromMemory(address);
    }
    if constexpr (Mode == AbsoluteIndexedY) {
        auto baseAddress = fetchImmedate16();
        auto index = registers.getY();
        address = baseAddress + index;
        value = mmu->readFromMemory((baseAddress & 0xFF00) | (address & 0xFF));
        value = mmu->readFromMemory(address);
    }
    if constexpr (Mode == ZeroPageIndexedX) {
        auto baseAddress = fetchImmedate8();
        auto index = registers.getX();
        address = (baseAddress + index) & 0xFF;
        value = mmu->readFromMemory(baseAddress);
        value = mmu->readFromMemory(address);
    }
    if constexpr (Mode == ZeroPageIndexedY) {
        auto baseAddress = fetchImmedate8();
        auto index = registers.getY();
        address = (baseAddress + index) & 0xFF;
        value = mmu->readFromMemory(baseAddress);
        value = mmu->readFromMemory(address);
    }
    if constexpr (Mode == IndirectX) {
        auto pointerAddress = fetchImmedate8();
        auto index = registers.getX();
        address = mmu->readFromMemory(pointerAddress);
        auto effectivePointerAddress = (pointerAddress + index) & 0xFF;
        address = mmu->readFromMemory(effectivePointerAddress) |
            mmu->readFromMemory((effectivePointerAddress + 1) & 0xFF) << 8;
        value = mmu->readFromMemory(address);
    }
    if constexpr (Mode == IndirectY) {
        auto pointerAddress = fetchImmedate8();
        auto baseAddress = mmu->readFromMemory(pointerAddress) |
            mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
        auto index = registers.getY();
        address = baseAddress + index;
        value = mmu->readFromMemory(baseAddress);
        value = mmu->readFromMemory(address);
    }
    mmu->writeIntoMemory(address, value);
    op(value);
    mmu->writeIntoMemory(address, value);
}

/**
 * LDA - Load Accumulator
 */
template <AddressingMode Mode>
inline void Cpu::lda()
{
    auto& accumulator = registers.getA();
    accumulator = resolveReadOperand<Mode>();
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * LDX - Load register X
 */
template <AddressingMode Mode>
inline void Cpu::ldx()
{
    auto& x = registers.getX();
    x = resolveReadOperand<Mode>();
    updateZeroFlag(x);
    updateNegativeFlag(x);
}

/**
 * LDY - Load Y register 
 */
template <AddressingMode Mode>
inline void Cpu::ldy()
{
    auto& y = registers.getY();
    y = resolveReadOperand<Mode>();
    updateZeroFlag(y);
    updateNegativeFlag(y);
}

/**
 * STA - Store Accumulator 
 */
template <AddressingMode Mode>
inline void Cpu::sta()
{
    auto accumulator = registers.getA();
    auto address = resolveWriteAddress<Mode>();
    mmu->writeIntoMemory(address, accumulator);
}

/**
 * Store X register
 */
template <AddressingMode Mode>
inline void Cpu::stx()
{
    auto x = registers.getX();
    auto address = resolveWriteAddress<Mode>();
    mmu->writeIntoMemory(address, x);
}

/**
 * STY - Store Y register
 */
template <AddressingMode Mode>
inline void Cpu::sty()
{
    auto y = registers.getY();
    auto address = resolveWriteAddress<Mode>();
    mmu->writeIntoMemory(address, y);
}

/**
 * TAX - Transfer Accmulator to X
 */
template <AddressingMode Mode>
inline void Cpu::tax()
{
    auto taxOp = [this](){
        const auto& accumulator = registers.getA();
        auto& x = registers.getX();
        x = accumulator;
        updateZeroFlag(x);
        updateNegativeFlag(x);
    };
    executeImplied<Mode>(taxOp);
}

/**
 * TAY - Transfer Accumulator to Y 
 */
template <AddressingMode Mode>
inline void Cpu::tay()
{
    auto tayOp = [this](){
        const auto& accumulator = registers.getA();
        auto& y = registers.getY();
        y = accumulator;
        updateZeroFlag(y);
        updateNegativeFlag(y);
    };
    executeImplied<Mode>(tayOp);
}

/**
 * TSX - Transfer Stack pointer to X
 */
template <AddressingMode Mode>
inline void Cpu::tsx()
{
    auto tsxOp = [this](){
        const auto& s = registers.getS();
        auto& x = registers.getX();
        x = s;
        updateZeroFlag(x);
        updateNegativeFlag(x);
    };
    executeImplied<Mode>(tsxOp);
}

/**
 * TXA - Transfer X to Accumulator
 */
template <AddressingMode Mode>
inline void Cpu::txa()
{
    auto txaOp = [this](){
        const auto& x = registers.getX();
        auto& flags = registers.getP();
        auto& accumulator = registers.getA();
        accumulator = x;
        updateZeroFlag(accumulator);
        updateNegativeFlag(accumulator);
    };
    executeImplied<Mode>(txaOp);
}

/**
 * TXS - Transfer X to Stack pointer
 */
template <AddressingMode Mode>
inline void Cpu::txs()
{
    auto txsOp = [this](){
        const auto& x = registers.getX();
        auto& s = registers.getS();
        s = x;
    };
    executeImplied<Mode>(txsOp);
}

/**
 * TYA - Transfer Y to Accumulator 
 */
template <AddressingMode Mode>
inline void Cpu::tya()
{
    auto tyaOp = [this](){
        const auto& y = registers.getY();
        auto& flags = registers.getP();
        auto& accumulator = registers.getA();
        accumulator = y;
        updateZeroFlag(accumulator);
        updateNegativeFlag(accumulator);
    };
    executeImplied<Mode>(tyaOp);
}

/**
 * DEC - Decrement memory
 */
template <AddressingMode Mode>
inline void Cpu::dec()
{
    auto decOp = [this](u8& value){
        value--;
        updateZeroFlag(value);
        updateNegativeFlag(value);
    };
    executeReadModifyWrite<Mode>(decOp);
}

/**
 * DEX - Decrement X 
 */
template <AddressingMode Mode>
inline void Cpu::dex()
{
    auto dexOp = [this](){
        auto& x = registers.getX();
        auto& flags = registers.getP();
        x--;
        updateZeroFlag(x);
        updateNegativeFlag(x);
    };
    executeImplied<Mode>(dexOp);
}

/**
 * DEY - Decrement Y 
 */
template <AddressingMode Mode>
inline void Cpu::dey()
{
    auto deyOp = [this](){
        auto& y = registers.getY();
        auto& flags = registers.getP();
        y--;
        updateZeroFlag(y);
        updateNegativeFlag(y);
    };
    executeImplied<Mode>(deyOp);
}

/**
 * INC - Increment memory 
 */
template <AddressingMode Mode>
inline void Cpu::inc()
{
    auto incOp = [this](u8& value){
        value++;
        updateZeroFlag(value);
        updateNegativeFlag(value);
    };
    executeReadModifyWrite<Mode>(incOp);
}

/**
 * INX - Increment X 
 */
template <AddressingMode Mode>
inline void Cpu::inx()
{
    auto inxOp = [this](){
        auto& x = registers.getX();
        auto& flags = registers.getP();
        x++;
        updateZeroFlag(x);
        updateNegativeFlag(x);
    };
    executeImplied<Mode>(inxOp);
}

/**
 * INY - Increment Y
 */
template <AddressingMode Mode>
inline void Cpu::iny()
{
    auto inyOp = [this](){
        auto& y = registers.getY();
        auto& flags = registers.getP();
        y++;
        updateZeroFlag(y);
        updateNegativeFlag(y);
    };
    executeImplied<Mode>(inyOp);
}

/**
 * CLC - Clear Carry flag 
 */
template <AddressingMode Mode>
inline void Cpu::clc()
{
    auto clcOp = [this](){
        registers.getP().carry = 0;
    };
    executeImplied<Mode>(clcOp);
}

/**
 * CLD - Clear Decimal flag 
 */
template <AddressingMode Mode>
inline void Cpu::cld()
{
    auto cldOp = [this](){
        registers.getP().decimal = 0;
    };
    executeImplied<Mode>(cldOp);
}

/**
 * CLI - Clear Interrupt disable flag 
 */
template <AddressingMode Mode>
inline void Cpu::cli()
{
    auto cliOp = [this](){
        registers.getP().interruptDisable = 0;
    };
    executeImplied<Mode>(cliOp);
}

/**
 * CLV - Clear Overflow flag 
 */
template <AddressingMode Mode>
inline void Cpu::clv()
{
    auto clvOp = [this](){
        registers.getP().overflow = 0;
    };
    executeImplied<Mode>(clvOp);
}

/**
 * SEC - Set Carry flag 
 */
template <AddressingMode Mode>
inline void Cpu::sec()
{
    auto secOp = [this](){
        registers.getP().carry = 1;
    };
    executeImplied<Mode>(secOp);
}

/**
 * SED - Set Decimal flag 
 */
template <AddressingMode Mode>
inline void Cpu::sed()
{
    auto sedOp = [this](){
        registers.getP().decimal = 1;
    };
    executeImplied<Mode>(sedOp);
}

/**
 * SEI - Set Interrupt disable flag 
 */
template <AddressingMode Mode>
inline void Cpu::sei()
{
    auto seiOp = [this](){
        registers.getP().interruptDisable = 1;
    };
    executeImplied<Mode>(seiOp);
}

/**
 * PHA - Push accumulator into stack 
 */
template <AddressingMode Mode>
inline void Cpu::pha()
{
    auto phaOp = [this](){
        const auto& accumulator = registers.getA();
        pushIntoStack8(accumulator);
    };
    executeImplied<Mode>(phaOp);
}

/**
 * PHP - Push Processor status into stack 
 */
template <AddressingMode Mode>
inline void Cpu::php()
{
    auto phpOp = [this](){
        auto flags = registers.getP();
        pushIntoStack8(flags.raw |= 0x3 << 4);
    };
    executeImplied<Mode>(phpOp);
}

/**
 * PLA - Pull accumulator from stack 
 */
template <AddressingMode Mode>
inline void Cpu::pla()
{
    auto plaOp = [this](){
        auto& accumulator = registers.getA();
        mmu->readFromMemory(registers.getS());
        accumulator = popFromStack8();
        updateZeroFlag(accumulator);
        updateNegativeFlag(accumulator);
    };
    executeImplied<Mode>(plaOp);
}

/**
 * PLP - Pull processor status from stack 
 */
template <AddressingMode Mode>
inline void Cpu::plp()
{
    auto plpOp = [this](){
        auto& flags = registers.getP();
        mmu->readFromMemory(registers.getS());
        flags = (flags & (1 << 5)) | (popFromStack8() & ~(1 << 4));
    };
    executeImplied<Mode>(plpOp);
}

/**
 * JMP - Jump
 */
template <AddressingMode Mode>
inline void Cpu::jmp()
{
    using enum AddressingMode;
    auto& pc = registers.getPc();
    static_assert(Mode == Absolute || Mode == Indirect, "Unsupported addressing mode used in JMP instruction");
    if constexpr (Mode == Absolute) {
        pc = fetchImmedate16();
    }
    if constexpr (Mode == Indirect) {
        auto pointer = fetchImmedate16();
        pc = mmu->readFromMemory(pointer) | mmu->readFromMemory((pointer & 0xFF00) | ((pointer + 1) & 0xFF)) << 8;
    }
}

/**
 * JSR - Jump to subroutine 
 */
template <AddressingMode Mode>
inline void Cpu::jsr()
{
    static_assert(Mode == AddressingMode::Absolute, "JSR instruction only supports Absolute addressing");
    auto& pc = registers.getPc();
    auto address = fetchImmedate16();
    mmu->readFromMemory(registers.getS());
    pushIntoStack16(pc - 1);
    pc = address;
}

/**
 * RTS - Return from subroutine
 */
template <AddressingMode Mode>
inline void Cpu::rts()
{
    auto rtsOp = [this](){
        auto& pc = registers.getPc();
        mmu->readFromMemory(registers.getS());
        pc = popFromStack16() + 1;
        mmu->readFromMemory(pc);
    };
    executeImplied<Mode>(rtsOp);
}

/**
 * ADC - Add memory to Accumulator with Carry 
 */
template <AddressingMode Mode>
inline void Cpu::adc()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto operand = resolveReadOperand<Mode>();
    u16 result = accumulator + operand + flags.carry;
    flags.carry = (result >> 8) & 0x1;
    flags.overflow = ((accumulator ^ result) & (operand ^ result) & 0x80) > 0;
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * SBC - Subtract memory from accumulator with borrow
 */
template <AddressingMode Mode>
inline void Cpu::sbc()
{
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto operand = resolveReadOperand<Mode>();
    u16 result = accumulator - operand - !flags.carry;
    flags.carry = !((result >> 8) & 0x1);
    flags.overflow = ((accumulator ^ result) & (~operand ^ result) & 0x80) > 0;
    accumulator = result & 0xFF;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * AND - AND memory with Accumulator 
 */
template <AddressingMode Mode>
inline void Cpu::_and()
{
    auto& accumulator = registers.getA();
    auto operand = resolveReadOperand<Mode>();
    accumulator &= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * EOR - Exclusive OR memory with Accumulator 
 */
template <AddressingMode Mode>
inline void Cpu::eor()
{
    auto& accumulator = registers.getA();
    auto operand = resolveReadOperand<Mode>();
    accumulator ^= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * ORA - OR memory with Accumulator 
 */
template <AddressingMode Mode>
inline void Cpu::ora()
{
    auto& accumulator = registers.getA();
    auto operand = resolveReadOperand<Mode>();
    accumulator |= operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * BCC - Branch on Carry Clear 
 */
template <AddressingMode Mode>
inline void Cpu::bcc()
{
    executeBranchInstruction<Mode>(!registers.getP().carry);
}

/**
 * BCS - Branch on Carry Set 
 */
template <AddressingMode Mode>
inline void Cpu::bcs()
{
    executeBranchInstruction<Mode>(registers.getP().carry);
}

/**
 * BEQ - Branch on Result zero
 */
template <AddressingMode Mode>
inline void Cpu::beq()
{
    executeBranchInstruction<Mode>(registers.getP().zero);
}

/**
 * BMI - Branch on Result Minus
 */
template <AddressingMode Mode>
inline void Cpu::bmi()
{
    executeBranchInstruction<Mode>(registers.getP().negative);
}

/**
 * BNE - Branch on Result Not Zero 
 */
template <AddressingMode Mode>
inline void Cpu::bne()
{
    executeBranchInstruction<Mode>(!registers.getP().zero);
}

/**
 * BPL - Branch on Result Plus
 */
template <AddressingMode Mode>
inline void Cpu::bpl()
{
    executeBranchInstruction<Mode>(!registers.getP().negative);
}

/**
 * BVC - Branch on Overflow Clear 
 */
template <AddressingMode Mode>
inline void Cpu::bvc()
{
    executeBranchInstruction<Mode>(!registers.getP().overflow);
}

/**
 * BVS - Branch on Overflow Set 
 */
template <AddressingMode Mode>
inline void Cpu::bvs()
{
    executeBranchInstruction<Mode>(registers.getP().overflow);
}

/**
 * ASL - Arithmetic Shift Left 
 */
template <AddressingMode Mode>
inline void Cpu::asl()
{
    auto aslOp = [this](u8& value) {
        auto& flags = registers.getP();
        flags.carry = (value >> 7) & 0x1;
        value <<= 1;
        updateNegativeFlag(value);
        updateZeroFlag(value);
    };
    executeReadModifyWrite<Mode>(aslOp);
}

/**
 * LSR - Logical Shift Left 
 */
template <AddressingMode Mode>
inline void Cpu::lsr()
{
    auto lsrOp = [this](u8& value) {
        auto& flags = registers.getP();
        flags.carry = value & 0x1;
        value >>= 1;
        updateNegativeFlag(value);
        updateZeroFlag(value);
    };
    executeReadModifyWrite<Mode>(lsrOp);
}

/**
 * ROL - Rotate Left 
 */
template <AddressingMode Mode>
inline void Cpu::rol()
{
    auto rolOp = [this](u8& value) {
        auto& flags = registers.getP();
        auto oldCarry = flags.carry;
        flags.carry = (value >> 7) & 0x1;
        value = (value << 1) | oldCarry;
        updateNegativeFlag(value);
        updateZeroFlag(value);
    };
    executeReadModifyWrite<Mode>(rolOp);
}

/**
 * ROR - Rotate Right
 */
template <AddressingMode Mode>
inline void Cpu::ror()
{
    auto rorOp = [this](u8& value) {
        auto& flags = registers.getP();
        auto oldCarry = flags.carry;
        flags.carry = value & 0x1;
        value = (value >> 1) | (oldCarry << 7);
        updateNegativeFlag(value);
        updateZeroFlag(value);
    };
    executeReadModifyWrite<Mode>(rorOp);
}

/**
 * CMP - Compare memory and Accumulator 
 */
template <AddressingMode Mode>
inline void Cpu::cmp()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto operand = resolveReadOperand<Mode>();
    flags.carry = operand <= accumulator;
    auto result = accumulator - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
}

/**
 * CPX - Compare memory with register X 
 */
template <AddressingMode Mode>
inline void Cpu::cpx()
{
    auto x = registers.getX();
    auto& flags = registers.getP();
    auto operand = resolveReadOperand<Mode>();
    flags.carry = operand <= x;
    auto result = x - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
}

/**
 * CPY - Compare memory with register Y 
 */
template <AddressingMode Mode>
inline void Cpu::cpy()
{
    auto y = registers.getY();
    auto& flags = registers.getP();
    auto operand = resolveReadOperand<Mode>();
    flags.carry = operand <= y;
    auto result = y - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
}

/**
 * BRK - Break
 */
template <AddressingMode Mode>
inline void Cpu::brk()
{
    static_assert(Mode == AddressingMode::Implied, "BRK instruction only supports Implied addressing");
    return handleInterrupt(InterruptType::BRK);
}

/**
 * RTI - Return from Interrupt 
 */
template <AddressingMode Mode>
inline void Cpu::rti()
{
    auto rtiOp = [this](){
        auto& flags = registers.getP();
        auto& pc = registers.getPc();
        auto oldFlags = flags;
        mmu->readFromMemory(registers.getS());
        flags.raw = popFromStack8() | oldFlags & 0x20;
        pc = popFromStack16();
    };
    executeImplied<Mode>(rtiOp);
}

/**
 * NOP - No operation 
 */
template <AddressingMode Mode>
inline void Cpu::nop()
{
    using enum AddressingMode;
    if constexpr (Mode == Implied) {
        mmu->readFromMemory(registers.getPc());
    } else {
        resolveReadOperand<Mode>();
    }
}

/**
 * BIT - Test bits in memory with Accumulator 
 */
template <AddressingMode Mode>
inline void Cpu::bit()
{
    auto accumulator = registers.getA();
    auto& flags = registers.getP();
    auto operand = resolveReadOperand<Mode>();
    updateNegativeFlag(operand);
    flags.overflow = (operand >> 6) & 0x1;
    auto result = accumulator & operand;
    updateZeroFlag(result);
}

/**
 * ALR [Unofficial] - AND oper + LSR
 */
template <AddressingMode Mode>
inline void Cpu::alr()
{
    static_assert(Mode == AddressingMode::Immediate, "ALR instruction only supports Immediate addressing");
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto operand = resolveReadOperand<Mode>();
    accumulator &= operand;
    flags.carry = accumulator & 0x1;
    accumulator >>= 1;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * ANC [Unofficial] - AND oper + set carry as ASL/ROL 
 */
template <AddressingMode Mode>
inline void Cpu::anc()
{
    static_assert(Mode == AddressingMode::Immediate, "ANC instruction only supports Immediate addressing");
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto operand = resolveReadOperand<Mode>();
    accumulator &= operand;
    flags.carry = (accumulator >> 7) & 0x1;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * XAA [Unofficial] - A and X and oper -> A 
 */
template <AddressingMode Mode>
inline void Cpu::xaa()
{
    static_assert(Mode == AddressingMode::Immediate, "XAA instruction only supports Immediate addressing");
    auto& accumulator = registers.getA();
    auto x = registers.getX();
    auto operand = resolveReadOperand<Mode>();
    accumulator = (accumulator | 0xFF) & x & operand;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * ARR [Unofficial] - AND oper + ROR
 */
template <AddressingMode Mode>
inline void Cpu::arr()
{
    static_assert(Mode == AddressingMode::Immediate, "XAA instruction only supports Immediate addressing");
    auto& accumulator = registers.getA();
    auto& flags = registers.getP();
    auto oldCarry = flags.carry;
    auto operand = resolveReadOperand<Mode>();
    accumulator &= operand;
    // Operation involves adder and flag is set according to (A and oper) + oper
    auto sum = accumulator + operand;
    flags.overflow = ((accumulator ^ sum) & (operand ^ sum) & 0x80) > 0;
    flags.carry = accumulator & 0x1;
    accumulator = (accumulator >> 1) | (oldCarry << 7);
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * DCP [Unofficial] - DEC oper + CMP oper
 */
template <AddressingMode Mode>
inline void Cpu::dcp()
{
    auto dcpOp = [this](u8& value){
        auto& flags = registers.getP();
        auto accumulator = registers.getA();
        value--;
        s8 result = accumulator - value;
        flags.carry = value <= accumulator;
        updateZeroFlag(result);
        updateNegativeFlag(result);
    };
    executeReadModifyWrite<Mode>(dcpOp);
}

/**
 * ISC [Unofficial] - INC oper + SBC oper 
 */
template <AddressingMode Mode>
inline void Cpu::isc()
{
    auto iscOp = [this](u8& value){
        auto& flags = registers.getP();
        auto& accumulator = registers.getA();
        value++;
        u16 result = accumulator - value - !flags.carry;
        flags.carry = !((result >> 8) & 0x1);
        flags.overflow = ((accumulator ^ result) & (~value ^ result) & 0x80) > 0;
        accumulator = result & 0xFF;
        updateZeroFlag(accumulator);
        updateNegativeFlag(accumulator);
    };
    executeReadModifyWrite<Mode>(iscOp);
}

/**
 * LAS [Unofficial] - LDA/TSX oper (M AND SP -> A, X, SP)
 */
template <AddressingMode Mode>
inline void Cpu::las()
{
    static_assert(Mode == AddressingMode::AbsoluteIndexedY, "LAS instruction supports only Absolute Indexed Y addressing");
    auto value = resolveReadOperand<Mode>();
    auto& sp = registers.getS();
    auto& accumulator = registers.getA();
    auto& x = registers.getX();
    sp &= value;
    x = accumulator = sp;
    updateZeroFlag(sp);
    updateNegativeFlag(sp);
}

/**
 * LAX [Unofficial] - LDA oper + LDX oper
 */
template <AddressingMode Mode>
inline void Cpu::lax()
{
    auto operand = resolveReadOperand<Mode>();
    auto& accumulator = registers.getA();
    auto& x = registers.getX();
    accumulator = operand;
    x = accumulator;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * LXA [Unofficial] - Store * AND oper in A and X
 */
template <AddressingMode Mode>
inline void Cpu::lxa()
{
    auto& accumulator = registers.getA();
    auto& x = registers.getX();
    auto operand = resolveReadOperand<Mode>();
    accumulator = (accumulator | 0xFF) & operand;
    x = accumulator;
    updateZeroFlag(accumulator);
    updateNegativeFlag(accumulator);
}

/**
 * RLA [Unofficial] - ROL oper + AND oper
 */
template <AddressingMode Mode>
inline void Cpu::rla()
{
    auto rlaOp = [this](u8& value){
        auto& flags = registers.getP();
        auto oldCarry = flags.carry;
        flags.carry = (value >> 7) & 0x1;
        value = value << 1 | oldCarry;
        auto& accumulator = registers.getA();
        accumulator &= value;
        updateZeroFlag(accumulator);
        updateNegativeFlag(accumulator);
    };
    executeReadModifyWrite<Mode>(rlaOp);
}

/**
 * RRA [Unofficial] - ROR oper + ADC oper
 */
template <AddressingMode Mode>
inline void Cpu::rra()
{
    auto rraOp = [this](u8& value){
        auto& flags = registers.getP();
        auto& accumulator = registers.getA();
        auto oldCarry = flags.carry;
        flags.carry = value & 0x1;
        value = value >> 1 | (oldCarry << 7);
        u16 result = accumulator + value + flags.carry;
        flags.carry = (result >> 8) & 0x1;
        flags.overflow = ((accumulator ^ result) & (value ^ result) & 0x80) > 0;
        accumulator = result & 0xFF;
        updateNegativeFlag(accumulator);
        updateZeroFlag(accumulator);
    };
    executeReadModifyWrite<Mode>(rraOp);
}

/**
 * SAX [Unofficial] - A and X are put on the bus at the same time (resulting effectively in an AND operation) and stored in M. 
 */
template <AddressingMode Mode>
inline void Cpu::sax()
{
    auto address = resolveWriteAddress<Mode>();
    auto accumulator = registers.getA();
    auto x = registers.getX();
    auto result = accumulator & x;
    mmu->writeIntoMemory(address, result);
}

/**
 * AXS [Unofficial] - CMP and DEX at once, sets flags like CMP
 */
template <AddressingMode Mode>
inline void Cpu::axs()
{
    static_assert(Mode == AddressingMode::Immediate, "AXS instruction supports only Immediate addressing");
    auto& x = registers.getX();
    auto& flags = registers.getP();
    auto accumulator = registers.getA();
    auto operand = fetchImmedate8();
    auto intermediate = accumulator & x;
    flags.carry = operand > intermediate;
    x = intermediate - operand;
    updateZeroFlag(x);
    updateNegativeFlag(x);
}

/**
 * AHX [Unofficial] - Stores A AND X AND (high-byte of addr. + 1) at addr.
 */
template <AddressingMode Mode>
inline void Cpu::ahx()
{
    using enum AddressingMode;
    static_assert(Mode == AbsoluteIndexedY || Mode == IndirectY, "AHX instruction only supports Absolute Indexed Y and Indirect Y addressing");
    auto address = resolveWriteAddress<Mode>();
    auto accumulator = registers.getA();
    auto x = registers.getX();
    auto result = accumulator & x & (((address >> 8) + 1) & 0xFF);
    mmu->writeIntoMemory(address, result);
}

/**
 * SHX [Unofficial] - Stores X AND (high-byte of addr. + 1) at addr.
 */
template <AddressingMode Mode>
inline void Cpu::shx()
{
    static_assert(Mode == AddressingMode::AbsoluteIndexedY, "SHX instruction only supports Absolute Indexed Y addressing");
    auto address = resolveWriteAddress<Mode>();
    auto x = registers.getX();
    u8 result = x & (((address >> 8) & 0xFF) + 1);
    mmu->writeIntoMemory(address, result);
}

/**
 * SHY [Unofficial] - Stores Y AND (high-byte of addr. + 1) at addr.
 */
template <AddressingMode Mode>
inline void Cpu::shy()
{
    static_assert(Mode == AddressingMode::AbsoluteIndexedX, "SHY instruction only supports Absolute Indexed X addressing");
    auto address = resolveWriteAddress<Mode>();
    auto y = registers.getY();
    u8 result = y & (((address >> 8) & 0xFF) + 1);
    mmu->writeIntoMemory(address, result);
}

/**
 * SLO [Unofficial] - ASL oper + ORA oper 
 */
template <AddressingMode Mode>
inline void Cpu::slo()
{
    auto sloOp = [this](u8& value){
        auto& flags = registers.getP();
        auto& accumulator = registers.getA();
        flags.carry = (value >> 7) & 0x1;
        value <<= 1;
        accumulator = accumulator | value;
    };
    executeReadModifyWrite<Mode>(sloOp);
}

/**
 * SRE [Unofficial] - LSR oper + EOR oper
 */
template <AddressingMode Mode>
inline void Cpu::sre()
{
    auto sreOp = [this](u8& value){
        auto& flags = registers.getP();
        auto& accumulator = registers.getA();
        flags.carry = value & 0x1;
        value >>= 1;
        accumulator = accumulator ^ value;
    };
    executeReadModifyWrite<Mode>(sreOp);
}

/**
 * TAS [Unofficial] - Puts A AND X in SP and stores A AND X AND (high-byte of addr. + 1) at addr.
 */
template <AddressingMode Mode>
inline void Cpu::tas()
{
    static_assert(Mode == AddressingMode::AbsoluteIndexedY, "TAS instruction only supports Absolute Indexed Y addressing");
    auto address = resolveWriteAddress<Mode>();
    auto& sp = registers.getS();
    auto accumulator = registers.getA();
    auto x = registers.getX();
    sp = accumulator & x;
    auto y = registers.getY();
    auto value = accumulator & x & ((address >> 8) + 1);
    mmu->writeIntoMemory(address, value);
}

/**
 * STP [Unofficial] - Freeze the CPU 
 */
template <AddressingMode Mode>
inline void Cpu::stp()
{
    static_assert(Mode == AddressingMode::Implied, "STP instruction supports only Implied addressing");
    halted = true;
}