#include <iostream>
#include <iomanip>

#include "Test.hpp"

#include "NesTestCpuTest.hpp"
#include "PpuVblankNmiTest.hpp"
#include "PpuOpenBusTest.hpp"
#include "PpuReadBufferTest.hpp"
#include "PaletteRamTest.hpp"
#include "SpriteRamTest.hpp"
#include "VblClearTimeTest.hpp"
#include "VramAccessTest.hpp"

class TestExecutor
{
    public:
        void registerTest(std::unique_ptr<Test> test) 
        {
            tests.push_back(std::move(test));
        }

        int runAll()
        {
            int result = 0;
            
            std::cout << "Running tests:\n" << std::endl;
            std::cout << "|" << std::left << std::setw(20) << std::setfill(' ') << "Test name";
            std::cout << "|" << std::left << std::setw(15) << std::setfill(' ') << "Set up";
            std::cout << "|" << std::left << std::setw(30) << std::setfill(' ') << "Run";
            std::cout << "|" << std::endl;
            
            for(auto i = 0; i < 69; i++) {
                std::cout << '-';
            }
            std::cout << std::endl;

            for(auto& test : tests) {
                auto testName = test->name();
                auto testResult = -1;
                
                std::cout << "|" << std::left << std::setw(20) << std::setfill(' ') << testName;
                
                bool hasSetUp = test->setUp();
                std::cout << "|" << std::left << std::setw(15) << std::setfill(' ') << (hasSetUp ? "Done" : "Failed");

                testResult = test->run();
                std::stringstream ss;
                if(testResult == 0) {
                    ss << "Passed";
                } else {
                    ss << "Failed (Code: " << testResult << ')';
                }
                std::cout << "|" << std::left << std::setw(30) << std::setfill(' ') << ss.str();
                std::cout << "|" << std::endl;
                result |= testResult;
            }
            return result;
        };

    private:
        std::vector<std::unique_ptr<Test>> tests;
};

int main()
{
    TestExecutor testExecutor;

    testExecutor.registerTest(std::make_unique<NesTestCpuTest>());
    testExecutor.registerTest(std::make_unique<PpuVblankNmiTest>());
    testExecutor.registerTest(std::make_unique<PpuOpenBusTest>());
    testExecutor.registerTest(std::make_unique<PpuReadBufferTest>());
    testExecutor.registerTest(std::make_unique<PaletteRamTest>());
    testExecutor.registerTest(std::make_unique<SpriteRamTest>());
    testExecutor.registerTest(std::make_unique<VblClearTimeTest>());
    testExecutor.registerTest(std::make_unique<VramAccessTest>());

    return testExecutor.runAll();
}