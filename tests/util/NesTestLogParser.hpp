#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <sstream>

#include "../../src/core/Types.hpp"

struct NesTestLogData
{
    u16 instructionAddress; // Address of the executed instruction
    u8 opcode;              // Fetched instruction opcode
    u8 registerA;           // Value of register A before instruction execution
    u8 registerX;           // Value of register X before instruction execution
    u8 registerY;           // Value of register Y before instruction execution
    u8 registerP;           // Value of register P before instruction execution
    u8 registerS;           // Value of Stack Pointer before instruction execution
    unsigned cycleCounter;  // Cycles performed before before execution
    unsigned lineNumber;    // Log line number
    std::string line;
    std::string previousLine;
};

struct NesTestLogParser
{
    NesTestLogParser(std::ifstream stream);
    
    ~NesTestLogParser();

    bool canParseNextLine();

    NesTestLogData parseNextLine();

    private:
        std::ifstream nesTestLogStream;
        unsigned lineNumberCounter;
};