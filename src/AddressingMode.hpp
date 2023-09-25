#pragma once

#include <string_view>

enum class AddressingMode
{
    Accumulator,
    Absolute,
    AbsoluteIndexedX,
    AbsoluteIndexedY,
    Immediate,
    Implied,
    Indirect,
    IndirectX,
    IndirectY,
    Relative,
    ZeroPage,
    ZeroPageIndexedX,
    ZeroPageIndexedY
};

constexpr std::string_view stringifyAddressingMode(AddressingMode mode) 
{
    switch (mode)
    {
        case AddressingMode::Accumulator: return "Accumulator";
        case AddressingMode::Absolute: return "Absolute";
        case AddressingMode::AbsoluteIndexedX: return "Absolute Indexed X";
        case AddressingMode::AbsoluteIndexedY: return "Absolute Indexed Y";
        case AddressingMode::Immediate: return "Immediate";
        case AddressingMode::Implied: return "Implied";
        case AddressingMode::Indirect: return "Indirect";
        case AddressingMode::IndirectX: return "Indirect X";
        case AddressingMode::IndirectY: return "Indirect Y";
        case AddressingMode::Relative: return "Relative";
        case AddressingMode::ZeroPage: return "Zero-Page";
        case AddressingMode::ZeroPageIndexedX: return "Zero-Page Indexed X";
        case AddressingMode::ZeroPageIndexedY: return "Zero-Page Indexed Y";
        default: return "Unknown";
    }

    return "Unknown";
}