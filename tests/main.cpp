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
            std::cout << "Running tests" << std::endl;
            for(auto& test : tests) {
                auto testName = test->name();
                std::cout << "Setting up " << testName << "\t";
                if(!test->setUp()) {
                    std::cout << "Failed" << std::endl;
                    continue;
                }
                std::cout << "Done" << std::endl;
                std::cout << "Running " << testName << "\t";
                auto testResult = test->run();
                if(testResult == 0) {
                    std::cout << "Passed" << std::endl;
                } else {
                    std::cout << "Failed (Code: " << testResult << ')' << std::endl;
                }
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