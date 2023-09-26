#include "Cpu.hpp"
template <AddressingMode Mode>
inline u8 Cpu::resolveOperand8()
{
    u8 operand = 0;

    if constexpr(Mode == AddressingMode::Immediate) {
        operand = fetchImmedate8();
    } else if constexpr(Mode == AddressingMode::ZeroPage) {
        u16 address = fetchImmedate8();
        operand = mmu->readFromMemory(address);
    } else if constexpr(Mode == AddressingMode::ZeroPageIndexedX) {
        u16 address = (static_cast<u16>(fetchImmedate8()) + registers.getX()) & 0xFF;
        operand = mmu->readFromMemory(address);
    } else if constexpr(Mode == AddressingMode::ZeroPageIndexedY) {
        u16 address = (static_cast<u16>(fetchImmedate8()) + registers.getY()) & 0xFF;
        operand = mmu->readFromMemory(address);
    } else if constexpr(Mode == AddressingMode::Absolute) {
        u16 address = fetchImmedate16();
        operand = mmu->readFromMemory(address);
    } else if constexpr(Mode == AddressingMode::AbsoluteIndexedX) {
        u16 address = fetchImmedate16() + registers.getX();
        operand = mmu->readFromMemory(address);
    } else if constexpr(Mode == AddressingMode::AbsoluteIndexedY) {
        u16 address = fetchImmedate16() + registers.getY();
        operand = mmu->readFromMemory(address);
    } else if constexpr(Mode == AddressingMode::IndirectX) {
        u16 indirectAddress = (static_cast<u16>(fetchImmedate8()) + registers.getX()) & 0xFF;
        u16 address = static_cast<u16>(mmu->readFromMemory(indirectAddress)) 
            + static_cast<u16>(mmu->readFromMemory(indirectAddress + 1));
        operand = mmu->readFromMemory(address);
    } else if constexpr(Mode == AddressingMode::IndirectY) {
        u16 indirectAddress = fetchImmedate8();
        u16 address = static_cast<u16>(mmu->readFromMemory(indirectAddress))
            + static_cast<u16>(mmu->readFromMemory(indirectAddress + 1))
            + registers.getY();
        operand = mmu->readFromMemory(address);
    } else {
        static_assert("Addressing mode '" + stringifyAddressingMode(Mode) +"' is unsupported for resolving 8-bit operand");
    }

    return operand;
}

template <AddressingMode Mode>
inline u16 Cpu::resolveOperand16()
{
    u16 operand = 0;

    if constexpr(Mode == AddressingMode::ZeroPage) {
        operand = fetchImmedate8();
    } else if constexpr(Mode == AddressingMode::ZeroPageIndexedX) {
        operand = (static_cast<u16>(fetchImmedate8()) + registers.getX()) & 0xFF;
    } else if constexpr(Mode == AddressingMode::Absolute) {
        operand = fetchImmedate16();
    } else if constexpr(Mode == AddressingMode::AbsoluteIndexedX) {
        operand = fetchImmedate16() + registers.getX();
    } else if constexpr(Mode == AddressingMode::AbsoluteIndexedY) {
        operand = fetchImmedate16() + registers.getY();
    } else if constexpr(Mode == AddressingMode::IndirectX) {
        u16 indirectAddress = fetchImmedate8();
        operand = static_cast<u16>(mmu->readFromMemory(indirectAddress))
            + static_cast<u16>(mmu->readFromMemory(indirectAddress + 1))
            + registers.getY();
    } else if constexpr(Mode == AddressingMode::IndirectY) {
        u16 indirectAddress = fetchImmedate8();
        operand = static_cast<u16>(mmu->readFromMemory(indirectAddress))
            + static_cast<u16>(mmu->readFromMemory(indirectAddress + 1))
            + registers.getY();
    } else {
        static_assert("Addressing mode '" + stringifyAddressingMode(Mode) +"' is unsupported for resolving 16-bit operand");
    }

    return operand;
}

template <AddressingMode Mode>
inline unsigned Cpu::lda()
{
    auto& accumulator = registers.getA();
    accumulator = resolveOperand8<Mode>();
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::ldx()
{
    auto& x = registers.getX();
    x = resolveOperand8();
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::ldy()
{
    auto& y = registers.getY();
    y = resolveOperand8();
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::sta()
{
    const auto& accumulator = registers.getA();
    u16 address = resolveOperand16<Mode>();
    mmu->writeIntoMemory(address, accumulator);
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::stx()
{
    const auto& x = registers.getX();
    u16 address = resolveOperand16<Mode>();
    mmu->writeIntoMemory(address, x);
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::sty()
{
    const auto& y = registers.getY();
    u16 address = resolveOperand16<Mode>();
    mmu->writeIntoMemory(address, y);
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::tax()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("TAX instruction does not support addressing mode other than Implied");
    }
    const auto& accumulator = registers.getA();
    auto& x = registers.getX();
    x = accumulator;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::tay()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("TAY instruction does not support addressing mode other than Implied");
    }
    const auto& accumulator = registers.getA();
    auto& y = registers.getY();
    y = accumulator;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::tsx()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("TSX instruction does not support addressing mode other than Implied");
    }
    const auto& s = registers.getS();
    auto& x = registers.getX();
    x = s;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::txa()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("TXA instruction does not support addressing mode other than Implied");
    }
    const auto& x = registers.getX();
    auto& accumulator = registers.getA();
    accumulator = x;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::txs()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("TXS instruction does not support addressing mode other than Implied");
    }
    const auto& x = registers.getX();
    auto& s = registers.getS();
    s = x;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::tya()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("TYA instruction does not support addressing mode other than Implied");
    }
    const auto& y = registers.getY();
    auto& accumulator = registers.getA();
    accumulator = y;
    return 0;
}
template <AddressingMode Mode>
inline unsigned Cpu::dec()
{
    u16 address = resolveOperand16<Mode>();
    u8 operand = mmu->readFromMemory(address);
    operand--;
    mmu->writeIntoMemory(address, operand);
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::dex()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("DEX instruction does not support addressing mode other than Implied");
    }
    auto& x = registers.getX();
    x--;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::dey()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("DEY instruction does not support addressing mode other than Implied");
    }
    auto& y = registers.getY();
    y--;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::inc()
{
    u16 address = resolveOperand16<Mode>();
    u8 operand = mmu->readFromMemory(address);
    operand++;
    mmu->writeIntoMemory(address, operand);
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::inx()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("INX instruction does not support addressing mode other than Implied");
    }
    auto& x = registers.getX();
    x++;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::iny()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("INY instruction does not support addressing mode other than Implied");
    }
    auto& y = registers.getY();
    y++;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::clc()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("CLC instruction does not support addressing mode other than Implied");
    }
    registers.getP().carry = 0;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::cld()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("CLD instruction does not support addressing mode other than Implied");
    }
    registers.getP().decimal = 0;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::cli()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("CLI instruction does not support addressing mode other than Implied");
    }
    registers.getP().interruptDisable = 0;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::clv()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("CLV instruction does not support addressing mode other than Implied");
    }
    registers.getP().overflow = 0;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::sec()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("SEC instruction does not support addressing mode other than Implied");
    }
    registers.getP().carry = 1;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::sed()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("SED instruction does not support addressing mode other than Implied");
    }
    registers.getP().decimal = 1;
    return 0;
}

template <AddressingMode Mode>
inline unsigned Cpu::sei()
{
    if constexpr(Mode != AddressingMode::Implied) {
        static_assert("SEI instruction does not support addressing mode other than Implied");
    }
    registers.getP().interruptDisable = 1;
    return 0;
}
