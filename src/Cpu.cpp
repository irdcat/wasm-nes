#include "Cpu.hpp"

Cpu::Cpu(const std::shared_ptr<Mmu> &mmu)
    : mmu(mmu)
{
}

u8 Cpu::fetchOpcode()
{
    return fetchImmedate8();
}

unsigned Cpu::executeInstruction(u8 opcode)
{
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
