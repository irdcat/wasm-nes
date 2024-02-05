#pragma once

#include "../src/Cpu.hpp"
#include "TestMmu.hpp"
#include "Test.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

struct NesTestLogData
{
    u16 instructionAddress; // PC before fetching opcode
    u8 opcode;              // Fetched opcode
    u8 registerA;           // Value of register A
    u8 registerX;           // Value of register X
    u8 registerY;           // Value of register Y
    u8 registerP;           // Value of Processor Status
    u8 stackPointer;        // Value of Stack pointer
    unsigned cycleCounter;  // Amount of cycles performed
    std::string line;
    std::string prevLine;
    unsigned lineNumber;
};

class NesTestLogParser
{
    public:
        NesTestLogParser(std::ifstream nesTestLog)
            : nesTestLogStream(std::move(nesTestLog))
        {
        }

        ~NesTestLogParser()
        {
            nesTestLogStream.close();
        }

        bool canParseNextLine() 
        {
            return nesTestLogStream.good() && !nesTestLogStream.eof();
        }

        NesTestLogData parseNextLine() 
        {
            static unsigned lineNumberCounter = 0;
            static std::string previousLine;

            std::string line;
            std::getline(nesTestLogStream, line);

            std::istringstream iss(line);
            NesTestLogData data;
            
            data.line = line;
            data.prevLine = previousLine;
            data.lineNumber = ++lineNumberCounter;

            std::string addressString;
            iss >> addressString;
            data.instructionAddress = std::stoul(addressString, nullptr, 16);

            std::string opcodeString;
            iss >> opcodeString;
            data.opcode = std::stoul(opcodeString, nullptr, 16);

            std::string hexValue;
            while(iss >> hexValue) {
                if (hexValue.find(':') != std::string::npos) {
                    char label = hexValue[0];
                    if(label == 'P' && hexValue[1] == 'P') {
                        continue;
                    }

                    std::istringstream hexStream(hexValue.substr(hexValue.find(':') + 1));
                    std::string valueString;
                    hexStream >> valueString;

                    switch(label) {
                        case 'A': data.registerA = std::stoul(valueString, nullptr, 16); break;
                        case 'X': data.registerX = std::stoul(valueString, nullptr, 16); break;
                        case 'Y': data.registerY = std::stoul(valueString, nullptr, 16); break;
                        case 'P': data.registerP = std::stoul(valueString, nullptr, 16); break;
                        case 'S': data.stackPointer = std::stoul(valueString, nullptr, 16); break;
                        case 'C': data.cycleCounter = std::stoul(valueString); break;
                        default: break;
                    }
                }
            }
            previousLine = line;
            return data;
        }

    private:
        std::ifstream nesTestLogStream;
};

class NesTestCpuTest : public Test
{
    public:
        ~NesTestCpuTest() = default;

        NesTestCpuTest()
            : nesTestLogParser(std::ifstream("nestest.log"))
            , testMmu(std::make_shared<TestMmu>())
            , cpu(std::make_unique<Cpu>(testMmu))
        {
        }

        void setUp() override
        {
            testMmu->clearMemory();
            auto romStream = std::ifstream("nestest.nes", std::ios::binary);
            auto startAddr = NES_TEST_START_ADDR;

            if(!romStream.is_open())
            {
                std::cout << "Unable to open file nestest.nes" << std::endl;
                romStream.exceptions(romStream.failbit);
            }

            romStream.seekg(0, std::ios::end);
            auto fileSize = romStream.tellg();
            romStream.seekg(16, std::ios::beg);

            std::vector<u8> data(fileSize);
            romStream.read((char *) &data[0], fileSize);
            std::for_each(data.begin(), data.end(), [&](auto byte){ testMmu->writeIntoMemory(startAddr++, byte); });
            testMmu->resetCycleCounter(0);
            cpu->getRegisters().s = 0;
            cpu->reset();
            cpu->getRegisters().pc = NES_TEST_START_ADDR;
        }

        int run()
        {
            auto printAssertionError = [](auto valueName, auto expected, auto actual, auto line, auto previousLine, auto lineNumber, bool useHex) {
                std::cout << "Actual " << valueName << " does not match the expectation" << std::endl
                            << (useHex ? std::hex : std::dec) 
                            << "Expected: " << std::uppercase << (unsigned)expected 
                            << " Actual: " << std::uppercase << (unsigned)actual << std::endl
                            << "Log lines: " << std::endl;
                
                if(lineNumber != 1) {
                    std::cout << std::dec << lineNumber - 1 << "\t" << previousLine << std::endl;
                }

                std::cout << std::dec << lineNumber << "\t" << line << std::endl;
            };

            while(nesTestLogParser.canParseNextLine()) {
                auto expected = nesTestLogParser.parseNextLine();
                auto registers = cpu->getRegisters();

                if(registers.pc != expected.instructionAddress) {
                    printAssertionError("PC", expected.instructionAddress, registers.pc, expected.line, expected.prevLine, expected.lineNumber, true);
                    return 1;
                }
                if(registers.a != expected.registerA) {
                    printAssertionError("A", expected.registerA, registers.a, expected.line, expected.prevLine, expected.lineNumber, true);
                    return 1;
                }
                if(registers.x != expected.registerX) {
                    printAssertionError("X", expected.registerX, registers.x, expected.line, expected.prevLine, expected.lineNumber, true);
                    return 1;
                }
                if(registers.y != expected.registerY) {
                    printAssertionError("Y", expected.registerY, registers.y, expected.line, expected.prevLine, expected.lineNumber, true);
                    return 1;
                }
                if(registers.p != expected.registerP) {
                    printAssertionError("P", expected.registerP, registers.p, expected.line, expected.prevLine, expected.lineNumber, true);
                    return 1;
                }
                if(registers.s != expected.stackPointer) {
                    printAssertionError("SP", expected.stackPointer, registers.s, expected.line, expected.prevLine, expected.lineNumber, true);
                    return 1;
                }

                auto opcode = cpu->fetchOpcode();

                if(opcode != expected.opcode) {
                    printAssertionError("opcode", expected.opcode, opcode, expected.line, expected.prevLine, expected.lineNumber, true);
                    return 1;
                }
                if(testMmu->getCycles() != expected.cycleCounter) {
                    printAssertionError("cycle count", expected.cycleCounter, testMmu->getCycles(), expected.line, expected.prevLine, expected.lineNumber, false);
                    return 1;
                }

                cpu->executeInstruction(opcode);
            }

            return 0;
        }

        std::string name() override
        {
            return std::string("NesTestCpuTest");
        }

    private:
        NesTestLogParser nesTestLogParser;
        std::shared_ptr<TestMmu> testMmu;
        std::unique_ptr<Cpu> cpu;

        static constexpr const unsigned NES_TEST_START_ADDR = 0xC000;
};