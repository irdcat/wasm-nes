template <AddressingMode Mode>
inline u8 Cpu::resolveReadOperand()
{
    using enum AddressingMode;

    constexpr auto isSupportedMode = Mode == Immediate 
        || isAbsolute(Mode) || isZeroPage(Mode) 
        || isIndexedIndirect(Mode) || isIndirectIndexed(Mode);
    static_assert(isSupportedMode, "Unsupported addressing mode used for resolving Read Instruction operand");

    u8 result = 0;
    if constexpr (Mode == Immediate) {
        result = fetchImmedate8();
    } else if constexpr (Mode == Absolute) {
        auto address = fetchImmedate16();
        result = mmu->readFromMemory(address);
    } else if constexpr (isAbsolute(Mode) && isIndexed(Mode)) {
        auto address = fetchImmedate16();
        auto index = Mode == AbsoluteIndexedX ? registers.x : registers.y;
        auto effectiveAddress = address + index;
        result = mmu->readFromMemory((address & 0xFF00) | (effectiveAddress & 0xFF));
        if((address & 0xFF00) != (effectiveAddress & 0xFF00)) {
            result = mmu->readFromMemory(effectiveAddress);
        }
    } else if constexpr (Mode == ZeroPage) {
        auto address = fetchImmedate8();
        result = mmu->readFromMemory(address);
    } else if constexpr (isZeroPage(Mode) && isIndexed(Mode)) {
        auto address = fetchImmedate8();
        auto index = Mode == ZeroPageIndexedX ? registers.x : registers.y;
        auto effectiveAddress = (address + index) & 0xFF;
        mmu->readFromMemory(address);
        result = mmu->readFromMemory(effectiveAddress);
    } else if constexpr (Mode == IndirectX) {
        auto pointerAddress = fetchImmedate8();
        mmu->readFromMemory(pointerAddress);
        auto index = registers.x;
        auto effectivePointerAddress = pointerAddress + index;
        u16 address = mmu->readFromMemory(effectivePointerAddress & 0xFF) |
            mmu->readFromMemory((effectivePointerAddress + 1) & 0xFF) << 8;
        result = mmu->readFromMemory(address);
    } else if constexpr (Mode == IndirectY) {
        auto pointerAddress = fetchImmedate8();
        u16 address = mmu->readFromMemory(pointerAddress) |
            mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
        auto index = registers.y;
        auto effectiveAddress = address + index;
        result = mmu->readFromMemory((address & 0xFF00) | (effectiveAddress & 0xFF));
        if ((address & 0xFF00) != (effectiveAddress & 0xFF00)) {
            result = mmu->readFromMemory(effectiveAddress);
        }
    }
    return result;
}

template <AddressingMode Mode>
inline u16 Cpu::resolveWriteAddress()
{
    using enum AddressingMode;
    constexpr auto isSupportedMode = isAbsolute(Mode) 
        || isZeroPage(Mode) || isIndexedIndirect(Mode) || isIndirectIndexed(Mode);
    static_assert(isSupportedMode, "Unsupported addressing mode used for resolving Write Instruction address");

    u16 result = 0;
    if constexpr (Mode == Absolute) {
        result = fetchImmedate16();
    } else if constexpr (isAbsolute(Mode) && isIndexed(Mode)) {
        auto address = fetchImmedate16();
        auto index = Mode == AbsoluteIndexedX ? registers.x : registers.y;
        auto effectiveAddress = address + index;
        mmu->readFromMemory((address & 0xFF00) | (effectiveAddress & 0xFF));
        result = effectiveAddress;
    } else if constexpr (Mode == ZeroPage) {
        result = fetchImmedate8();
    } else if constexpr (isZeroPage(Mode) && isIndexed(Mode)) {
        auto address = fetchImmedate8();
        auto index = Mode == ZeroPageIndexedX ? registers.x : registers.y;
        mmu->readFromMemory(address);
        result = (address + index) & 0xFF;
    } else if constexpr (Mode == IndirectX) {
        auto pointerAddress = fetchImmedate8();
        mmu->readFromMemory(pointerAddress);
        auto index = registers.x;
        auto effectivePointerAddress = pointerAddress + index;
        result = mmu->readFromMemory(effectivePointerAddress & 0xFF) |
            mmu->readFromMemory((effectivePointerAddress + 1) & 0xFF) << 8;
    } else if constexpr (Mode == IndirectY) {
        auto pointerAddress = fetchImmedate8();
        u16 address = mmu->readFromMemory(pointerAddress) |
            mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
        auto index = registers.y;
        auto effectiveAddress = address + index;
        mmu->readFromMemory((address & 0xFF00) | (effectiveAddress & 0xFF));
        result = effectiveAddress;
    }
    return result;
}

template <AddressingMode Mode>
inline void Cpu::executeImplied(const std::function<void()> &op)
{
    static_assert(Mode == AddressingMode::Implied, "Addressing mode other than Implied used in implied instruction");
    mmu->readFromMemory(registers.pc);
    op();
}

template <AddressingMode Mode>
inline void Cpu::executeBranchInstruction(bool condition)
{
    static_assert(Mode == AddressingMode::Relative, "Branch instructions only support Relative addressing");
    s8 offset = fetchImmedate8();
    auto oldPc = registers.pc;
    if(condition) {
        mmu->readFromMemory(registers.pc);
        registers.pc += offset;
    }
    if((oldPc & 0xFF00) != (registers.pc & 0xFF00)) {
        mmu->readFromMemory(registers.pc);
    }
}

template <AddressingMode Mode>
inline void Cpu::executeReadModifyWrite(const std::function<void(u8&)> &op)
{
    using enum AddressingMode;
    constexpr bool isSupportedMode = isAbsolute(Mode) 
        || isZeroPage(Mode) || isIndexedIndirect(Mode) || isIndirectIndexed(Mode);
    static_assert(isSupportedMode, "Unsupported addressing mode used for performing Read-Modify-Write instruction");
    
    u16 address = 0;
    u8 value = 0;
    if constexpr (Mode == Absolute) {
        address = fetchImmedate16();
        value = mmu->readFromMemory(address);
    } else if constexpr (Mode == ZeroPage) {
        address = fetchImmedate8();
        value = mmu->readFromMemory(address);
    } else if constexpr (isAbsolute(Mode) && isIndexed(Mode)) {
        auto baseAddress = fetchImmedate16();
        auto index = Mode == AbsoluteIndexedX ? registers.x : registers.y;
        address = baseAddress + index;
        value = mmu->readFromMemory((baseAddress & 0xFF00) | (address & 0xFF));
        value = mmu->readFromMemory(address);
    } else if constexpr (isZeroPage(Mode) && isIndexed(Mode)) {
        auto baseAddress = fetchImmedate8();
        auto index = Mode == ZeroPageIndexedX ? registers.x : registers.y;
        address = (baseAddress + index) & 0xFF;
        value = mmu->readFromMemory(baseAddress);
        value = mmu->readFromMemory(address);
    } else if constexpr (Mode == IndirectX) {
        auto pointerAddress = fetchImmedate8();
        auto index = registers.x;
        address = mmu->readFromMemory(pointerAddress);
        auto effectivePointerAddress = (pointerAddress + index) & 0xFF;
        address = mmu->readFromMemory(effectivePointerAddress) |
            mmu->readFromMemory((effectivePointerAddress + 1) & 0xFF) << 8;
        value = mmu->readFromMemory(address);
    } else if constexpr (Mode == IndirectY) {
        auto pointerAddress = fetchImmedate8();
        auto baseAddress = mmu->readFromMemory(pointerAddress) |
            mmu->readFromMemory((pointerAddress + 1) & 0xFF) << 8;
        auto index = registers.y;
        address = baseAddress + index;
        value = mmu->readFromMemory(baseAddress);
        value = mmu->readFromMemory(address);
    }
    mmu->writeIntoMemory(address, value);
    op(value);
    mmu->writeIntoMemory(address, value);
}

template <>
inline void Cpu::executeReadModifyWrite<AddressingMode::Accumulator>(const std::function<void(u8&)>& op) {
    mmu->readFromMemory(registers.pc);
    op(registers.a);
}

/**
 * LDA - Load Accumulator
 */
template <AddressingMode Mode>
inline void Cpu::lda()
{
    registers.a = resolveReadOperand<Mode>();
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * LDX - Load register X
 */
template <AddressingMode Mode>
inline void Cpu::ldx()
{
    registers.x = resolveReadOperand<Mode>();
    updateZeroFlag(registers.x);
    updateNegativeFlag(registers.x);
}

/**
 * LDY - Load Y register 
 */
template <AddressingMode Mode>
inline void Cpu::ldy()
{
    registers.y = resolveReadOperand<Mode>();
    updateZeroFlag(registers.y);
    updateNegativeFlag(registers.y);
}

/**
 * STA - Store Accumulator 
 */
template <AddressingMode Mode>
inline void Cpu::sta()
{
    auto address = resolveWriteAddress<Mode>();
    mmu->writeIntoMemory(address, registers.a);
}

/**
 * Store X register
 */
template <AddressingMode Mode>
inline void Cpu::stx()
{
    auto address = resolveWriteAddress<Mode>();
    mmu->writeIntoMemory(address, registers.x);
}

/**
 * STY - Store Y register
 */
template <AddressingMode Mode>
inline void Cpu::sty()
{
    auto address = resolveWriteAddress<Mode>();
    mmu->writeIntoMemory(address, registers.y);
}

/**
 * TAX - Transfer Accmulator to X
 */
template <AddressingMode Mode>
inline void Cpu::tax()
{
    auto taxOp = [this](){
        registers.x = registers.a;
        updateZeroFlag(registers.x);
        updateNegativeFlag(registers.x);
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
        registers.y = registers.a;
        updateZeroFlag(registers.y);
        updateNegativeFlag(registers.y);
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
        registers.x = registers.s;
        updateZeroFlag(registers.x);
        updateNegativeFlag(registers.x);
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
        registers.a = registers.x;
        updateZeroFlag(registers.a);
        updateNegativeFlag(registers.a);
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
        registers.s = registers.x;
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
        registers.a = registers.y;
        updateZeroFlag(registers.a);
        updateNegativeFlag(registers.a);
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
        registers.x--;
        updateZeroFlag(registers.x);
        updateNegativeFlag(registers.x);
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
        registers.y--;
        updateZeroFlag(registers.y);
        updateNegativeFlag(registers.y);
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
        registers.x++;
        updateZeroFlag(registers.x);
        updateNegativeFlag(registers.x);
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
        registers.y++;
        updateZeroFlag(registers.y);
        updateNegativeFlag(registers.y);
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
        registers.p.carry = 0;
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
        registers.p.decimal = 0;
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
        registers.p.interruptDisable = 0;
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
        registers.p.overflow = 0;
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
        registers.p.carry = 1;
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
        registers.p.decimal = 1;
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
        registers.p.interruptDisable = 1;
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
        pushIntoStack8(registers.a);
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
        pushIntoStack8(registers.p | 0x3 << 4);
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
        mmu->readFromMemory(registers.s);
        registers.a = popFromStack8();
        updateZeroFlag(registers.a);
        updateNegativeFlag(registers.a);
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
        mmu->readFromMemory(registers.s);
        registers.p = (registers.p & (1 << 5)) | (popFromStack8() & ~(1 << 4));
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
    static_assert(Mode == Absolute || Mode == Indirect, "Unsupported addressing mode used in JMP instruction");
    if constexpr (Mode == Absolute) {
        registers.pc = fetchImmedate16();
    }
    if constexpr (Mode == Indirect) {
        auto pointer = fetchImmedate16();
        registers.pc = mmu->readFromMemory(pointer) | mmu->readFromMemory((pointer & 0xFF00) | ((pointer + 1) & 0xFF)) << 8;
    }
}

/**
 * JSR - Jump to subroutine 
 */
template <AddressingMode Mode>
inline void Cpu::jsr()
{
    static_assert(Mode == AddressingMode::Absolute, "JSR instruction only supports Absolute addressing");
    auto address = fetchImmedate16();
    mmu->readFromMemory(registers.s);
    pushIntoStack16(registers.pc - 1);
    registers.pc = address;
}

/**
 * RTS - Return from subroutine
 */
template <AddressingMode Mode>
inline void Cpu::rts()
{
    auto rtsOp = [this](){
        mmu->readFromMemory(registers.s);
        registers.pc = popFromStack16() + 1;
        mmu->readFromMemory(registers.pc);
    };
    executeImplied<Mode>(rtsOp);
}

/**
 * ADC - Add memory to Accumulator with Carry 
 */
template <AddressingMode Mode>
inline void Cpu::adc()
{
    auto operand = resolveReadOperand<Mode>();
    u16 result = registers.a + operand + registers.p.carry;
    registers.p.carry = (result >> 8) & 0x1;
    registers.p.overflow = ((registers.a ^ result) & (operand ^ result) & 0x80) > 0;
    registers.a = result & 0xFF;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * SBC - Subtract memory from accumulator with borrow
 */
template <AddressingMode Mode>
inline void Cpu::sbc()
{
    auto operand = resolveReadOperand<Mode>();
    u16 result = registers.a - operand - !registers.p.carry;
    registers.p.carry = !((result >> 8) & 0x1);
    registers.p.overflow = ((registers.a ^ result) & (~operand ^ result) & 0x80) > 0;
    registers.a = result & 0xFF;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * AND - AND memory with Accumulator 
 */
template <AddressingMode Mode>
inline void Cpu::_and()
{
    auto operand = resolveReadOperand<Mode>();
    registers.a &= operand;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * EOR - Exclusive OR memory with Accumulator 
 */
template <AddressingMode Mode>
inline void Cpu::eor()
{
    auto operand = resolveReadOperand<Mode>();
    registers.a ^= operand;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * ORA - OR memory with Accumulator 
 */
template <AddressingMode Mode>
inline void Cpu::ora()
{
    auto operand = resolveReadOperand<Mode>();
    registers.a |= operand;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * BCC - Branch on Carry Clear 
 */
template <AddressingMode Mode>
inline void Cpu::bcc()
{
    executeBranchInstruction<Mode>(!registers.p.carry);
}

/**
 * BCS - Branch on Carry Set 
 */
template <AddressingMode Mode>
inline void Cpu::bcs()
{
    executeBranchInstruction<Mode>(registers.p.carry);
}

/**
 * BEQ - Branch on Result zero
 */
template <AddressingMode Mode>
inline void Cpu::beq()
{
    executeBranchInstruction<Mode>(registers.p.zero);
}

/**
 * BMI - Branch on Result Minus
 */
template <AddressingMode Mode>
inline void Cpu::bmi()
{
    executeBranchInstruction<Mode>(registers.p.negative);
}

/**
 * BNE - Branch on Result Not Zero 
 */
template <AddressingMode Mode>
inline void Cpu::bne()
{
    executeBranchInstruction<Mode>(!registers.p.zero);
}

/**
 * BPL - Branch on Result Plus
 */
template <AddressingMode Mode>
inline void Cpu::bpl()
{
    executeBranchInstruction<Mode>(!registers.p.negative);
}

/**
 * BVC - Branch on Overflow Clear 
 */
template <AddressingMode Mode>
inline void Cpu::bvc()
{
    executeBranchInstruction<Mode>(!registers.p.overflow);
}

/**
 * BVS - Branch on Overflow Set 
 */
template <AddressingMode Mode>
inline void Cpu::bvs()
{
    executeBranchInstruction<Mode>(registers.p.overflow);
}

/**
 * ASL - Arithmetic Shift Left 
 */
template <AddressingMode Mode>
inline void Cpu::asl()
{
    auto aslOp = [this](u8& value) {
        registers.p.carry = (value >> 7) & 0x1;
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
        registers.p.carry = value & 0x1;
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
        auto oldCarry = registers.p.carry;
        registers.p.carry = (value >> 7) & 0x1;
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
        auto oldCarry = registers.p.carry;
        registers.p.carry = value & 0x1;
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
    auto operand = resolveReadOperand<Mode>();
    registers.p.carry = operand <= registers.a;
    auto result = registers.a - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
}

/**
 * CPX - Compare memory with register X 
 */
template <AddressingMode Mode>
inline void Cpu::cpx()
{
    auto operand = resolveReadOperand<Mode>();
    registers.p.carry = operand <= registers.x;
    auto result = registers.x - operand;
    updateZeroFlag(result);
    updateNegativeFlag(result);
}

/**
 * CPY - Compare memory with register Y 
 */
template <AddressingMode Mode>
inline void Cpu::cpy()
{
    auto operand = resolveReadOperand<Mode>();
    registers.p.carry = operand <= registers.y;
    auto result = registers.y - operand;
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
        auto oldFlags = registers.p;
        mmu->readFromMemory(registers.s);
        registers.p = popFromStack8() | oldFlags & 0x20;
        registers.pc = popFromStack16();
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
        mmu->readFromMemory(registers.pc);
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
    auto operand = resolveReadOperand<Mode>();
    updateNegativeFlag(operand);
    registers.p.overflow = (operand >> 6) & 0x1;
    auto result = registers.a & operand;
    updateZeroFlag(result);
}

/**
 * ALR [Unofficial] - AND oper + LSR
 */
template <AddressingMode Mode>
inline void Cpu::alr()
{
    static_assert(Mode == AddressingMode::Immediate, "ALR instruction only supports Immediate addressing");
    auto operand = resolveReadOperand<Mode>();
    registers.a &= operand;
    registers.p.carry = registers.a & 0x1;
    registers.a >>= 1;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * ANC [Unofficial] - AND oper + set carry as ASL/ROL 
 */
template <AddressingMode Mode>
inline void Cpu::anc()
{
    static_assert(Mode == AddressingMode::Immediate, "ANC instruction only supports Immediate addressing");
    auto operand = resolveReadOperand<Mode>();
    registers.a &= operand;
    registers.p.carry = (registers.a >> 7) & 0x1;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * XAA [Unofficial] - A and X and oper -> A 
 */
template <AddressingMode Mode>
inline void Cpu::xaa()
{
    static_assert(Mode == AddressingMode::Immediate, "XAA instruction only supports Immediate addressing");
    auto operand = resolveReadOperand<Mode>();
    registers.a = (registers.a | 0xFF) & registers.x & operand;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * ARR [Unofficial] - AND oper + ROR
 */
template <AddressingMode Mode>
inline void Cpu::arr()
{
    static_assert(Mode == AddressingMode::Immediate, "XAA instruction only supports Immediate addressing");
    auto oldCarry = registers.p.carry;
    auto operand = resolveReadOperand<Mode>();
    registers.a &= operand;
    // Operation involves adder and flag is set according to (A and oper) + oper
    auto sum = registers.a + operand;
    registers.p.overflow = ((registers.a ^ sum) & (operand ^ sum) & 0x80) > 0;
    registers.p.carry = registers.a & 0x1;
    registers.a = (registers.a >> 1) | (oldCarry << 7);
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * DCP [Unofficial] - DEC oper + CMP oper
 */
template <AddressingMode Mode>
inline void Cpu::dcp()
{
    auto dcpOp = [this](u8& value){
        value--;
        s8 result = registers.a - value;
        registers.p.carry = value <= registers.a;
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
        value++;
        u16 result = registers.a - value - !registers.p.carry;
        registers.p.carry = !((result >> 8) & 0x1);
        registers.p.overflow = ((registers.a ^ result) & (~value ^ result) & 0x80) > 0;
        registers.a = result & 0xFF;
        updateZeroFlag(registers.a);
        updateNegativeFlag(registers.a);
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
    registers.s &= value;
    registers.x = registers.s = registers.s;
    updateZeroFlag(registers.s);
    updateNegativeFlag(registers.s);
}

/**
 * LAX [Unofficial] - LDA oper + LDX oper
 */
template <AddressingMode Mode>
inline void Cpu::lax()
{
    auto operand = resolveReadOperand<Mode>();
    registers.a = operand;
    registers.x = registers.a;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * LXA [Unofficial] - Store * AND oper in A and X
 */
template <AddressingMode Mode>
inline void Cpu::lxa()
{
    auto operand = resolveReadOperand<Mode>();
    registers.a = (registers.a | 0xFF) & operand;
    registers.x = registers.a;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * RLA [Unofficial] - ROL oper + AND oper
 */
template <AddressingMode Mode>
inline void Cpu::rla()
{
    auto rlaOp = [this](u8& value){
        auto oldCarry = registers.p.carry;
        registers.p.carry = (value >> 7) & 0x1;
        value = value << 1 | oldCarry;
        registers.a &= value;
        updateZeroFlag(registers.a);
        updateNegativeFlag(registers.a);
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
        auto oldCarry = registers.p.carry;
        registers.p.carry = value & 0x1;
        value = value >> 1 | (oldCarry << 7);
        u16 result = registers.a + value + registers.p.carry;
        registers.p.carry = (result >> 8) & 0x1;
        registers.p.overflow = ((registers.a ^ result) & (value ^ result) & 0x80) > 0;
        registers.a = result & 0xFF;
        updateNegativeFlag(registers.a);
        updateZeroFlag(registers.a);
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
    auto result = registers.a & registers.x;
    mmu->writeIntoMemory(address, result);
}

/**
 * AXS [Unofficial] - CMP and DEX at once, sets flags like CMP
 */
template <AddressingMode Mode>
inline void Cpu::axs()
{
    static_assert(Mode == AddressingMode::Immediate, "AXS instruction supports only Immediate addressing");
    auto operand = fetchImmedate8();
    auto intermediate = registers.a & registers.x;
    registers.p.carry = operand > intermediate;
    registers.x = intermediate - operand;
    updateZeroFlag(registers.x);
    updateNegativeFlag(registers.x);
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
    auto result = registers.a & registers.x & (((address >> 8) + 1) & 0xFF);
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
    u8 result = registers.x & (((address >> 8) & 0xFF) + 1);
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
    u8 result = registers.y & (((address >> 8) & 0xFF) + 1);
    mmu->writeIntoMemory(address, result);
}

/**
 * SLO [Unofficial] - ASL oper + ORA oper 
 */
template <AddressingMode Mode>
inline void Cpu::slo()
{
    auto sloOp = [this](u8& value){
        registers.p.carry = (value >> 7) & 0x1;
        value <<= 1;
        registers.a |= value;
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
        registers.p.carry = value & 0x1;
        value >>= 1;
        registers.a ^= value;
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
    registers.s = registers.a & registers.x;
    auto value = registers.a & registers.x & ((address >> 8) + 1);
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