#include <fstream>
#include <sstream>
#include <string>
#include <memory>

#include <gtest/gtest.h>

#include "util/SystemUnderTest.hpp"
#include "util/NesTestLogParser.hpp"

class CpuInstructionsTest : public ::testing::Test
{
    protected:
        std::unique_ptr<NesTestLogParser> nesTestLogParser;
        std::unique_ptr<SystemUnderTest> systemUnderTest;

        ~CpuInstructionsTest() = default;

        CpuInstructionsTest() = default;

        void SetUp() override
        {
            systemUnderTest = std::make_unique<SystemUnderTest>();
            auto cartridge = systemUnderTest->getCartridge();
            auto cpu = systemUnderTest->getCpu();
            auto logStream = std::ifstream("resources/nestest.log");
            nesTestLogParser = std::make_unique<NesTestLogParser>(std::move(logStream));
            if(cartridge->loadFromFile(std::ifstream("resources/nestest.nes", std::ios::binary))) {
                cpu->reset();
                cpu->getRegisters().pc = 0xC000;
            }
        }

        void TearDown() override
        {
        }
};

TEST_F(CpuInstructionsTest, Test)
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

    auto cpu = systemUnderTest->getCpu();
    auto mmu = systemUnderTest->getMmu();
    auto cycleCount = 0;
    while(nesTestLogParser->canParseNextLine())
    {
        auto expected = nesTestLogParser->parseNextLine();
        auto registers = cpu->getRegisters();
        auto lineNumber = expected.lineNumber;

        ASSERT_EQ(expected.instructionAddress, registers.pc) << printError(expected.instructionAddress, registers.pc, expected, "PC");
        ASSERT_EQ(expected.registerA, registers.a) << printError(expected.registerA, registers.a, expected, "A");
        ASSERT_EQ(expected.registerX, registers.x) << printError(expected.registerX, registers.x, expected, "X");
        ASSERT_EQ(expected.registerY, registers.y) << printError(expected.registerY, registers.y, expected, "Y");
        ASSERT_EQ(expected.registerS, registers.s) << printError(expected.registerS, registers.s, expected, "SP");
        ASSERT_EQ(expected.registerP, (u8) registers.p) << printError(expected.registerP, registers.p, expected, "P");
        
        auto opcode = cpu->fetchOpcode();
        ASSERT_EQ(expected.opcode, opcode) << printError(expected.opcode, opcode, expected, "Opcode");

        cycleCount += mmu->getAndResetTickCounterValue();
        ASSERT_EQ(expected.cycleCounter, cycleCount) << printError(expected.cycleCounter, cycleCount, expected, "Cycle Count", false);

        cpu->executeInstruction(opcode);
    }
}