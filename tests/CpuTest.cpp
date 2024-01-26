#include "../src/Cpu.hpp"
#include "../src/Mmu.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <array>

class TestMmu
    : public Mmu
{
    public:
        TestMmu() = default;

        ~TestMmu() = default;

        u8 readFromMemory(u16 addr) override 
        {
            cycles++;
            return memory[addr]; 
        };

        void writeIntoMemory(u16 addr, u8 value) override 
        {
            cycles++; 
            memory[addr] = value; 
        };

        void clearMemory() 
        {
            for(auto& byte : memory) 
                byte = 0; 
        };

        unsigned getCycles()
        {
            return cycles;
        }

        void resetCycleCounter(unsigned initial)
        {
            cycles = initial;
        }

    private:
        std::array<u8, 0xFFFF> memory;
        unsigned cycles;
};

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
            while(iss >> hexValue) 
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

int main()
{
    std::shared_ptr<TestMmu> testMmu = std::make_shared<TestMmu>();
    auto nesTestLogParser = NesTestLogParser(std::ifstream("nestest.log"));

    constexpr const unsigned NES_TEST_START_ADDR = 0xC000;

    auto loadRomIntoMemory = [&testMmu](auto name, auto startAddr) {
        auto romStream = std::ifstream(name, std::ios::binary);

        if(!romStream.is_open())
        {
            std::cout << "Unable to open file " << name << std::endl;
            romStream.exceptions(romStream.failbit);
        }

        romStream.seekg(0, std::ios::end);
        auto fileSize = romStream.tellg();
        romStream.seekg(16, std::ios::beg);

        std::vector<u8> data(fileSize);
        romStream.read((char *) &data[0], fileSize);

        for(const auto& byte : data)
        {
            testMmu->writeIntoMemory(startAddr++, byte);
        }

        std::cout << std::endl << "-----------------------------" << std::endl;
        std::cout << "File " << name << " loaded, size " << fileSize << "B" << std::endl;
    };

    testMmu->clearMemory();
    loadRomIntoMemory("nestest.nes", NES_TEST_START_ADDR);
    testMmu->resetCycleCounter(0);

    auto printAssertionError = [](auto valueName, auto expected, auto actual, auto line, auto previousLine, auto lineNumber, bool useHex) {
        std::cout << "Actual " << valueName << " does not match the expectation" << std::endl
                    << (useHex ? std::hex : std::dec) 
                    << "Expected: " << std::uppercase << (unsigned)expected 
                    << " Actual: " << std::uppercase << (unsigned)actual << std::endl
                    << "Log lines: " << std::endl;
        
        if(lineNumber != 1)
        {
            std::cout << std::dec << lineNumber - 1 << "\t" << previousLine << std::endl;
        }

        std::cout << std::dec << lineNumber << "\t" << line << std::endl;
    };

    std::unique_ptr<Cpu> cpu = std::make_unique<Cpu>(testMmu);
    auto& pc = cpu->getRegisters().getPc();
    pc = NES_TEST_START_ADDR;

    while(nesTestLogParser.canParseNextLine())
    {
        auto expected = nesTestLogParser.parseNextLine();
        auto registers = cpu->getRegisters();

        if(registers.getPc() != expected.instructionAddress)
        {
            printAssertionError("PC", expected.instructionAddress, registers.getPc(), expected.line, expected.prevLine, expected.lineNumber, true);
            return 1;
        }
        if(registers.getA() != expected.registerA)
        {
            printAssertionError("A", expected.registerA, registers.getA(), expected.line, expected.prevLine, expected.lineNumber, true);
            return 1;
        }
        if(registers.getX() != expected.registerX)
        {
            printAssertionError("X", expected.registerX, registers.getX(), expected.line, expected.prevLine, expected.lineNumber, true);
            return 1;
        }
        if(registers.getY() != expected.registerY)
        {
            printAssertionError("Y", expected.registerY, registers.getY(), expected.line, expected.prevLine, expected.lineNumber, true);
            return 1;
        }
        if(registers.getP() != expected.registerP)
        {
            printAssertionError("P", expected.registerP, registers.getP(), expected.line, expected.prevLine, expected.lineNumber, true);
            return 1;
        }
        if(registers.getS() != expected.stackPointer)
        {
            printAssertionError("SP", expected.stackPointer, registers.getS(), expected.line, expected.prevLine, expected.lineNumber, true);
            return 1;
        }

        auto opcode = cpu->fetchOpcode();

        if(opcode != expected.opcode)
        {
            printAssertionError("opcode", expected.opcode, opcode, expected.line, expected.prevLine, expected.lineNumber, true);
            return 1;
        }
        if(testMmu->getCycles() != expected.cycleCounter)
        {
            printAssertionError("cycle count", expected.cycleCounter, testMmu->getCycles(), expected.line, expected.prevLine, expected.lineNumber, false);
            return 1;
        }

        cpu->executeInstruction(opcode);
    }

    std::cout << "SUCCESS!" << std::endl;
    return 0;
}