/**
 * One of the type of instruction is Read instruction.
 * It involves reading operand from memory to store it in the register 
 * and optionally performing operations in between of that.
 * Purpose of this method is resolving operand that is to be read from memory,
 * depending on Addressing Mode, while handling repeating patterns of bus activity (e.g. dummy reads).
 */
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
        result = readFromMemory8(address);
    } else if constexpr (isAbsolute(Mode) && isIndexed(Mode)) {
        auto address = fetchImmedate16();
        auto index = Mode == AbsoluteIndexedX ? registers.x : registers.y;
        auto effectiveAddress = address + index;
        result = misfire(address, effectiveAddress);
    } else if constexpr (Mode == ZeroPage) {
        auto address = fetchImmedate8();
        result = readFromMemory8(address);
    } else if constexpr (isZeroPage(Mode) && isIndexed(Mode)) {
        auto address = fetchImmedate8();
        auto index = Mode == ZeroPageIndexedX ? registers.x : registers.y;
        auto effectiveAddress = address + index;
        readFromZeroPage8(address);
        result = readFromZeroPage8(effectiveAddress);
    } else if constexpr (Mode == IndirectX) {
        auto pointerAddress = fetchImmedate8();
        readFromMemory8(pointerAddress);
        auto index = registers.x;
        auto effectivePointerAddress = pointerAddress + index;
        u16 address = readFromZeroPage16(effectivePointerAddress);
        result = readFromMemory8(address);
    } else if constexpr (Mode == IndirectY) {
        auto pointerAddress = fetchImmedate8();
        u16 address = readFromZeroPage16(pointerAddress);
        auto index = registers.y;
        auto effectiveAddress = address + index;
        result = misfire(address, effectiveAddress);
    }
    return result;
}

/**
 * One of the type of instruction is Write instruction.
 * It involves writing register value into memory. 
 * and optionally performing operations in between of that.
 * Purpose of this method is resolving address to be used while writing into memory,
 * depending on Addressing Mode, while handling repeating patterns of bus activity (e.g. dummy reads).
 */
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
        misread(address, effectiveAddress);
        result = effectiveAddress;
    } else if constexpr (Mode == ZeroPage) {
        result = fetchImmedate8();
    } else if constexpr (isZeroPage(Mode) && isIndexed(Mode)) {
        auto address = fetchImmedate8();
        auto index = Mode == ZeroPageIndexedX ? registers.x : registers.y;
        readFromMemory8(address);
        result = (address + index) & 0xFF;
    } else if constexpr (Mode == IndirectX) {
        auto pointerAddress = fetchImmedate8();
        readFromMemory8(pointerAddress);
        auto index = registers.x;
        auto effectivePointerAddress = pointerAddress + index;
        result = readFromZeroPage16(effectivePointerAddress);
    } else if constexpr (Mode == IndirectY) {
        auto pointerAddress = fetchImmedate8();
        u16 address = readFromZeroPage16(pointerAddress);
        auto index = registers.y;
        auto effectiveAddress = address + index;
        misread(address, effectiveAddress);
        result = effectiveAddress;
    }
    return result;
}

/**
 * Instructions with Implied addressing have their own repeating pattern of bus activity,
 * before performing actual operation.
 */
template <AddressingMode Mode>
inline void Cpu::executeImplied(const std::function<void()> &op)
{
    static_assert(Mode == AddressingMode::Implied, "Addressing mode other than Implied used in implied instruction");
    // Perform dummy read from current PC position before executing operations
    readFromMemory8(registers.pc);
    op();
}

/**
 * Branch instructions are sharing same behaviour when it comes to bus activity.
 * They may execute extra dummy reads depending on whether branch condition is met, 
 * or memory page boundary was crossed while taking a branch. 
 */
template <AddressingMode Mode>
inline void Cpu::executeBranchInstruction(bool condition)
{
    static_assert(Mode == AddressingMode::Relative, "Branch instructions only support Relative addressing");
    // Reading offset as signed value as destination will be relative to current position in the code.
    s8 offset = fetchImmedate8();
    auto oldPc = registers.pc;
    // When condition is met, perform dummy read before jumping
    if(condition) {
        readFromMemory8(registers.pc);
        registers.pc += offset;
    }
    // When page is crossed, perform dummy read
    if((oldPc & 0xFF00) != (registers.pc & 0xFF00)) {
        readFromMemory8(registers.pc);
    }
}

/**
 * One of the type of instruction is Read-Modify-Write instruction.
 * It involves reading register or memory value, modifying it and writing modified value.
 * Purpose of this method is resolving address to be used while writing into memory,
 * depending on Addressing Mode, while handling repeating patterns of bus activity (e.g. dummy reads/writes).
 */
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
        value = readFromMemory8(address);
    } else if constexpr (Mode == ZeroPage) {
        address = fetchImmedate8();
        value = readFromZeroPage8(address);
    } else if constexpr (isAbsolute(Mode) && isIndexed(Mode)) {
        auto baseAddress = fetchImmedate16();
        auto index = Mode == AbsoluteIndexedX ? registers.x : registers.y;
        address = baseAddress + index;
        misread(baseAddress, address);
        value = readFromMemory8(address);
    } else if constexpr (isZeroPage(Mode) && isIndexed(Mode)) {
        auto baseAddress = fetchImmedate8();
        auto index = Mode == ZeroPageIndexedX ? registers.x : registers.y;
        address = baseAddress + index;
        readFromMemory8(baseAddress);
        value = readFromZeroPage8(address);
    } else if constexpr (Mode == IndirectX) {
        auto pointerAddress = fetchImmedate8();
        auto index = registers.x;
        readFromMemory8(pointerAddress);
        auto effectivePointerAddress = pointerAddress + index;
        address = readFromZeroPage16(effectivePointerAddress);
        value = readFromMemory8(address);
    } else if constexpr (Mode == IndirectY) {
        auto pointerAddress = fetchImmedate8();
        auto baseAddress = readFromZeroPage16(pointerAddress);
        auto index = registers.y;
        address = baseAddress + index;
        readFromMemory8(baseAddress);
        value = readFromMemory8(address);
    }
    // Before modifying operand, perform dummy write
    if constexpr(isZeroPage(Mode)) {
        writeIntoZeroPage8(address, value);
        op(value);
        writeIntoZeroPage8(address, value);
    } else {
        writeIntoMemory8(address, value);
        op(value);
        writeIntoMemory8(address, value);
    }
}

/**
 * Accumulator addressing involves it's own special pattern of bus activity.
 * The rest of characteristics of Read-Modify-Write instructions, stays the same. 
 */
template <>
inline void Cpu::executeReadModifyWrite<AddressingMode::Accumulator>(const std::function<void(u8&)>& op) {
    // Perform dummy read from current PC position before executing operations
    readFromMemory8(registers.pc);
    op(registers.a);
}

/**
 * LDA - Load Accumulator
 * Loads Accumulator with the value read from memory.
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
 * Loads index register X with the value read from memory.
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
 * Loads index register Y with the value read from memory.
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
 * Stores Accumulator value in memory.
 */
template <AddressingMode Mode>
inline void Cpu::sta()
{
    auto address = resolveWriteAddress<Mode>();
    writeIntoMemory8(address, registers.a);
}

/**
 * STX - Store X register
 * Stores index register X value in memory.
 */
template <AddressingMode Mode>
inline void Cpu::stx()
{
    auto address = resolveWriteAddress<Mode>();
    writeIntoMemory8(address, registers.x);
}

/**
 * STY - Store Y register
 * Stores index register Y value in memory.
 */
template <AddressingMode Mode>
inline void Cpu::sty()
{
    auto address = resolveWriteAddress<Mode>();
    writeIntoMemory8(address, registers.y);
}

/**
 * TAX - Transfer Accmulator to X
 * Value of Accumulator is loaded into register X. Accumulator value is left unchanged.
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
 * Value of Accumulator is loaded into register Y. Accumulator value is left unchanged.
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
 * Value of Stack Pointer is loaded into register X. Stack pointer value is left unchanged.
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
 * Value of Register X is loaded into Accumulator. Register X value is left unchanged.
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
 * Value of Register X is loaded into Stack Pointer. Register X value is left unchanged.
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
 * Value of Register Y is loaded into Accumulator. Register Y value is left unchanged.
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
 * Decrements value in memory.
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
 * Decrements value of register X.
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
 * Decrements value of register Y.
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
 * Increments value in memory.
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
 * Increments value of register X.
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
 * Increments value of register Y.
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
 * Carry flag bit is set to 0.
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
 * Decimal flag bit is set to 0.
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
 * Interrupt Disable flag bit is set to 0.
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
 * Overflow flag bit is set to 0.
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
 * Carry flag bit is set to 1.
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
 * Decimal flag bit is set to 1. 
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
 * Interrupt Disable flag bit is set to 1.
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
 * Accumulator value is pushed to the stack. Value of Accumulator is left unchanged.
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
 * Pushes value of Processor Status flags with bits 4 and 5 set, but doesn't set those bits in register value itself. 
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
 * Loads Accumulator with value popped from stack.
 */
template <AddressingMode Mode>
inline void Cpu::pla()
{
    auto plaOp = [this](){
        readFromMemory8(registers.s);
        registers.a = popFromStack8();
        updateZeroFlag(registers.a);
        updateNegativeFlag(registers.a);
    };
    executeImplied<Mode>(plaOp);
}

/**
 * PLP - Pull processor status from stack 
 * Loads Processor Status flags from stack, without taking bit 4 into account and retaining value of bit 5 before popping.
 */
template <AddressingMode Mode>
inline void Cpu::plp()
{
    auto plpOp = [this](){
        readFromMemory8(registers.s);
        registers.p = (registers.p & (1 << 5)) | (popFromStack8() & ~(1 << 4));
    };
    executeImplied<Mode>(plpOp);
}

/**
 * JMP - Jump
 * Jump to another place in memory, by setting Program Counter value with new address depending on Addressing Mode.
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
        // Page crossing does not happen while reading jump destination from the pointer
        registers.pc = readAndWrapFromMemory16(pointer);
    }
}

/**
 * JSR - Jump to subroutine 
 * Jumping to subroutine involves pushing Program Counter to the stack and then setting it to the address of subroutine to be called.
 */
template <AddressingMode Mode>
inline void Cpu::jsr()
{
    static_assert(Mode == AddressingMode::Absolute, "JSR instruction only supports Absolute addressing");
    auto address = fetchImmedate16();
    readFromMemory8(registers.s);
    pushIntoStack16(registers.pc - 1);
    registers.pc = address;
}

/**
 * RTS - Return from subroutine
 * Returning from subroutine involves jumping to the incremented address popped from stack, that was pushed while calling subroutine.
 */
template <AddressingMode Mode>
inline void Cpu::rts()
{
    auto rtsOp = [this](){
        readFromMemory8(registers.s);
        registers.pc = popFromStack16() + 1;
        readFromMemory8(registers.pc);
    };
    executeImplied<Mode>(rtsOp);
}

/**
 * ADC - Add memory to Accumulator with Carry 
 * Byte from the the memory and carry flag value is added to the accumulator.
 * Result of the operation is stored in the accumulator.
 */
template <AddressingMode Mode>
inline void Cpu::adc()
{
    auto operand = resolveReadOperand<Mode>();
    u16 result = registers.a + operand + registers.p.carry;
    registers.p.carry = (result >> 8) & 0x1;
    // Checking for arithmetic overflow (change of sign as a result of operation)
    registers.p.overflow = ((registers.a ^ result) & (operand ^ result) & 0x80) > 0;
    registers.a = result & 0xFF;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * SBC - Subtract memory from accumulator with borrow
 * Byte from the the memory and negated carry flag is subtracted from accumulator.
 * Result of the operation is stored in the accumulator.
 */
template <AddressingMode Mode>
inline void Cpu::sbc()
{
    auto operand = resolveReadOperand<Mode>();
    u16 result = registers.a - operand - !registers.p.carry;
    registers.p.carry = !((result >> 8) & 0x1);
    // Checking for arithmetic overflow (change of sign as a result of operation)
    registers.p.overflow = ((registers.a ^ result) & (~operand ^ result) & 0x80) > 0;
    registers.a = result & 0xFF;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * AND - AND memory with Accumulator 
 * Byte from the memory is bitwise ANDed with Accumulator.
 * Result of the operation is stored in Accumulator.
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
 * Byte from the memory is bitwise XORed with Accumulator.
 * Result of the operation is stored in Accumulator.
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
 * Byte from the memory is bitwise ORed with Accumulator.
 * Result of the operation is stored in Accumulator.
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
 * Jumps to specified location when carry flag is 0.
 */
template <AddressingMode Mode>
inline void Cpu::bcc()
{
    executeBranchInstruction<Mode>(!registers.p.carry);
}

/**
 * BCS - Branch on Carry Set 
 * Jumps to specified location when carry flag is 1.
 */
template <AddressingMode Mode>
inline void Cpu::bcs()
{
    executeBranchInstruction<Mode>(registers.p.carry);
}

/**
 * BEQ - Branch on Result zero
 * Jumps to specified location when zero flag is 1.
 */
template <AddressingMode Mode>
inline void Cpu::beq()
{
    executeBranchInstruction<Mode>(registers.p.zero);
}

/**
 * BMI - Branch on Result Minus
 * Jumps to specified location when negative flag is 1.
 */
template <AddressingMode Mode>
inline void Cpu::bmi()
{
    executeBranchInstruction<Mode>(registers.p.negative);
}

/**
 * BNE - Branch on Result Not Zero 
 * Jumps to specified location when zero flag is 0.
 */
template <AddressingMode Mode>
inline void Cpu::bne()
{
    executeBranchInstruction<Mode>(!registers.p.zero);
}

/**
 * BPL - Branch on Result Plus
 * Jumps to specified location when negative flag is 0.
 */
template <AddressingMode Mode>
inline void Cpu::bpl()
{
    executeBranchInstruction<Mode>(!registers.p.negative);
}

/**
 * BVC - Branch on Overflow Clear 
 * Jumps to specified location when overflow flag is 0.
 */
template <AddressingMode Mode>
inline void Cpu::bvc()
{
    executeBranchInstruction<Mode>(!registers.p.overflow);
}

/**
 * BVS - Branch on Overflow Set 
 * Jumps to specified location when overflow flag is 1.
 */
template <AddressingMode Mode>
inline void Cpu::bvs()
{
    executeBranchInstruction<Mode>(registers.p.overflow);
}

/**
 * ASL - Arithmetic Shift Left 
 * Carry is updated with value of most significant bit of accumulator.
 * Then accumulator is shifted left by one bit.
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
 * LSR - Logical Shift Right 
 * Carry is updated with value of least significant bit of accumulator.
 * Then accumulator is shifted right by one bit.
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
 * Accumulator is rotated left, which means that carry is set to most significant bit of accumulator, 
 * value of accumulator is then shifted left by one bit and bit 0 of accumulator is set to old value of carry.
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
 * Accumulator is rotated right, which means that carry is set to least significant bit of accumulator, 
 * value of accumulator is then shifted right by one bit and bit 7 of accumulator is set to old value of carry.
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
 * Performs subtraction without borrow. 
 * Result of the operation is discarded as the goal of this operation is to only update Processor Status flags.
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
 * Performs subtraction without borrow. 
 * Result of the operation is discarded as the goal of this operation is to only update Processor Status flags.
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
 * Performs subtraction without borrow. 
 * Result of the operation is discarded as the goal of this operation is to only update Processor Status flags.
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
 * Executes BRK - Software interrupt, that shares the same interrupt vector as IRQ.
 */
template <AddressingMode Mode>
inline void Cpu::brk()
{
    static_assert(Mode == AddressingMode::Implied, "BRK instruction only supports Implied addressing");
    return handleInterrupt(InterruptType::BRK);
}

/**
 * RTI - Return from Interrupt 
 * Returns from interrupt. 
 * Value of Processor Status is popped from stack and after that new Program Counter is popped from stack.
 */
template <AddressingMode Mode>
inline void Cpu::rti()
{
    auto rtiOp = [this](){
        auto oldFlags = registers.p;
        readFromMemory8(registers.s);
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
        readFromMemory8(registers.pc);
    } else {
        resolveReadOperand<Mode>();
    }
}

/**
 * BIT - Test bits in memory with Accumulator 
 * Byte from memory is ANDed with Accumulator. 
 * Result of the operation is discarded as the goal of this operation is to only update Processor Status flags.
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
 * Byte from memory and accumulator are placed in the bus at the same time effectively resulting in AND operation.
 * Then LSR instruction is performed.
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
 * Byte from memory and accumulator are placed in the bus at the same time effectively resulting in AND operation.
 * Then carry flag is updated same way as it is for ASL or ROL instruction.
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
 * A base value of Accumulator is determined based on the contets of A and a constant, which may be typically $00, $FF, $EE, etc. 
 * The value of this constant depends on temerature, the chip series, and maybe other factors, as well.
 * $FF is used to eliminate those uncertanties.
 * Byte from memory, Accumulator and register X are placed in the bus at the same time effectively resulting in AND operation.
 * Result of that operation is stored in Accumulator.
 */
template <AddressingMode Mode>
inline void Cpu::xaa()
{
    static_assert(Mode == AddressingMode::Immediate, "XAA instruction only supports Immediate addressing");
    auto operand = resolveReadOperand<Mode>();
    registers.a = 0xFF & registers.x & operand;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * ARR [Unofficial] - AND oper + ROR
 * Byte from memory and accumulator are placed in the bus at the same time effectively resulting in AND operation.
 * Then ROR instruction is performed.
 * Because this instruction involves the adder, thus overflow flag is set according to: (A AND operand) + operand.
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
 * Decrements value in memory and after modification compares it with Accumulator.
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
 * Increments value in memory and after modification performs Subtraction with Borrow.
 * Result of that operation is stored in Accumulator.
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
 * Mix of LDA and TSX instructions. Byte from memory and Stack Pointer is placed in the bus at the same time resulting in AND operation.
 * Result of that operation is stored in Accumulator, Register X and Stack Pointer.
 */
template <AddressingMode Mode>
inline void Cpu::las()
{
    static_assert(Mode == AddressingMode::AbsoluteIndexedY, "LAS instruction supports only Absolute Indexed Y addressing");
    auto value = resolveReadOperand<Mode>();
    registers.s &= value;
    registers.x = registers.a = registers.s;
    updateZeroFlag(registers.s);
    updateNegativeFlag(registers.s);
}

/**
 * LAX [Unofficial] - LDA oper + LDX oper
 * Mix of LDA and LDX instructions. Byte from memory is stored in Accumulator and Register X.
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
 * A base value of Accumulator is determined based on the contets of A and a constant, which may be typically $00, $FF, $EE, etc. 
 * The value of this constant depends on temerature, the chip series, and maybe other factors, as well.
 * $FF is used to eliminate those uncertanties.
 * Byte from memory and accumulator are placed in the bus at the same time effectively resulting in AND operation.
 * Result is stored in Accumulator and Register X.
 */
template <AddressingMode Mode>
inline void Cpu::lxa()
{
    auto operand = resolveReadOperand<Mode>();
    registers.a = 0xFF & operand;
    registers.x = registers.a;
    updateZeroFlag(registers.a);
    updateNegativeFlag(registers.a);
}

/**
 * RLA [Unofficial] - ROL oper + AND oper
 * Byte from memory is rotated left, then result is ANDed with Accumulator.
 * Result is stored in Accumulator.
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
 * Byte from memory is rotated right, then result is added with updated carry to Accumulator.
 * Result is stored in accumulator.
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
 * SAX [Unofficial] 
 * A and X are put on the bus at the same time (resulting effectively in an AND operation) and stored in memory. 
 */
template <AddressingMode Mode>
inline void Cpu::sax()
{
    auto address = resolveWriteAddress<Mode>();
    auto result = registers.a & registers.x;
    writeIntoMemory8(address, result);
}

/**
 * AXS [Unofficial] - CMP + DEX
 * Accumulator and Register X is placed on the bus at the same time (resulting effectively in an AND operation).
 * Accumulator is then compared with the result of the operation. But instead of discarding the result it is stored in Register X.
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
    writeIntoMemory8(address, result);
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
    writeIntoMemory8(address, result);
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
    writeIntoMemory8(address, result);
}

/**
 * SLO [Unofficial] - ASL oper + ORA oper 
 * Arithmetically shifts left byte from memory. Result is the ORed with Accumulator.
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
 * Logically shifts left byte from memory. Result is the XORed with Accumulator.
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
    writeIntoMemory8(address, value);
}

/**
 * STP [Unofficial] - Freeze the CPU 
 * This instruction is an example of a case where some "accidentally" triggered
 * CPU micro-operations never reset the internal instruction clock, 
 * thus making it stuck in the middle of instruction execution effectively halting the CPU.
 */
template <AddressingMode Mode>
inline void Cpu::stp()
{
    static_assert(Mode == AddressingMode::Implied, "STP instruction supports only Implied addressing");
    halted = true;
}