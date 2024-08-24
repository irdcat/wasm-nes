#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <memory>

#include "util/IntegrationTest.hpp"

class CpuInstructionsTest : public IntegrationTest
{
    private:
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
            NesTestLogParser(std::ifstream stream) 
                : nesTestLogStream(std::move(stream)), lineNumberCounter(1)
            {
            }
            
            ~NesTestLogParser() 
            { 
                nesTestLogStream.close();
            }

            bool canParseNextLine()
            {
                return nesTestLogStream.good()
                    && !nesTestLogStream.eof();
            }

            NesTestLogData parseNextLine()
            {
                static std::string previousLine;

                std::string line;
                std::getline(nesTestLogStream, line);
                std::istringstream lineStream(line);
                NesTestLogData data;
                data.line = line;
                data.previousLine = previousLine;
                data.lineNumber = lineNumberCounter++;

                std::string addressString;
                lineStream >> addressString;
                data.instructionAddress = std::stoul(addressString, nullptr, 16);

                std::string opcodeString;
                lineStream >> opcodeString;
                data.opcode = std::stoul(opcodeString, nullptr, 16);

                std::string hexValue;
                while(lineStream >> hexValue) 
                {
                    if (hexValue.find(':') != std::string::npos)
                    {
                        char label = hexValue[0];
                        if(label == 'P' && hexValue[1] == 'P')
                        {
                            continue;
                        }

                        std::istringstream hexStream(hexValue.substr(hexValue.find(':') + 1));
                        std::string valueString;
                        hexStream >> valueString;

                        switch(label)
                        {
                            case 'A': data.registerA = std::stoul(valueString, nullptr, 16); break;
                            case 'X': data.registerX = std::stoul(valueString, nullptr, 16); break;
                            case 'Y': data.registerY = std::stoul(valueString, nullptr, 16); break;
                            case 'P': data.registerP = std::stoul(valueString, nullptr, 16); break;
                            case 'S': data.registerS = std::stoul(valueString, nullptr, 16); break;
                            case 'C': data.cycleCounter = std::stoul(valueString); break;
                            default: break;
                        }
                    }
                }

                previousLine = line;
                return data;
            };

            private:
                std::ifstream nesTestLogStream;
                unsigned lineNumberCounter;
        };

        std::unique_ptr<NesTestLogParser> nesTestLogParser;
        std::string failureReason;

    public:
        ~CpuInstructionsTest() = default;

        CpuInstructionsTest(const std::shared_ptr<SystemUnderTest>& sut)
            : IntegrationTest("CpuInstructionsTest", sut)
        {
        }

        bool setUp() override
        {
            auto cartridge = getSystemUnderTest()->getCartridge();
            auto cpu = getSystemUnderTest()->getCpu();
            auto logStream = std::ifstream("resources/nestest.log");
            nesTestLogParser = std::make_unique<NesTestLogParser>(std::move(logStream));
            if(cartridge->loadFromFile(std::ifstream("resources/nestest.nes", std::ios::binary))) {
                cpu->reset();
                cpu->getRegisters().pc = 0xC000;
                return true;
            }
            return false;
        }

        int run() override
        {
            auto printError = [](auto expected, auto actual, auto data, auto valueName, bool hex = true){
                std::ostringstream oss;
                oss << "Value of " << valueName << " does not match at line " << data.lineNumber << std::endl;
                if(hex) {
                    oss << "Expected: 0x" << std::hex << (unsigned) expected << std::endl
                        << "Actual: 0x" << std::hex << (unsigned) actual << std::dec << std::endl;
                } else {
                    oss << "Expected: " << (unsigned) expected << std::endl 
                        << "Actual: " << (unsigned) actual << std::endl;
                }
                oss << std::endl << "Log lines:" << std::endl
                    << data.previousLine << std::endl
                    << data.line << std::endl;
                return oss.str();
            };

            auto cpu = getSystemUnderTest()->getCpu();
            auto mmu = getSystemUnderTest()->getMmu();
            auto cycleCount = 0;
            while(nesTestLogParser->canParseNextLine())
            {
                auto expected = nesTestLogParser->parseNextLine();
                auto registers = cpu->getRegisters();
                auto lineNumber = expected.lineNumber;

                if(registers.pc != expected.instructionAddress) {
                    failureReason = printError(expected.instructionAddress, registers.pc, expected, "PC");
                    return lineNumber;
                }
                if(registers.a != expected.registerA) {
                    failureReason = printError(expected.registerA, registers.a, expected, "A");
                    return lineNumber;
                }
                if(registers.x != expected.registerX) {
                    failureReason = printError(expected.registerX, registers.x, expected, "X");
                    return lineNumber;
                }
                if(registers.y != expected.registerY) {
                    failureReason = printError(expected.registerY, registers.y, expected, "Y");
                    return lineNumber;
                }
                if(registers.s != expected.registerS) {
                    failureReason = printError(expected.registerS, registers.s, expected, "SP");
                    return lineNumber;
                }
                if(registers.p != expected.registerP) {
                    failureReason = printError(expected.registerP, registers.p, expected, "P");
                    return lineNumber;
                }

                auto opcode = cpu->fetchOpcode();
                if(opcode != expected.opcode) {
                    failureReason = printError(expected.opcode, opcode, expected, "OpCode");
                    return expected.lineNumber;
                }

                cycleCount += mmu->getAndResetTickCounterValue();
                if(cycleCount != expected.cycleCounter) {
                    failureReason = printError(expected.cycleCounter, cycleCount, expected, "Cycle Count", false);
                    return expected.lineNumber;
                }

                cpu->executeInstruction(opcode);
            }
            return 0;
        }

        std::string getFailureReason() override
        {
            return failureReason;
        }
};