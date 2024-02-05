#include "TestMmu.hpp"

#include <algorithm>

u8 TestMmu::readFromMemory(u16 addr) 
{
    cycles++;
    return memory[addr];
}

void TestMmu::writeIntoMemory(u16 addr, u8 value)
{
    cycles++;
    memory[addr] = value;
}

void TestMmu::clearMemory()
{
    std::fill(memory.begin(), memory.end(), 0);
}

unsigned TestMmu::getCycles()
{
    return cycles;
}

void TestMmu::resetCycleCounter(unsigned initial)
{
    cycles = initial;
}