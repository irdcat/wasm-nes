#include "Test.hpp"

#include "NesTestCpuTest.hpp"

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
                test->setUp();
                std::cout << "Done" << std::endl;
                std::cout << "Running " << testName << "\t";
                auto testResult = test->run();
                std::cout << (testResult == 0 ? "Passed" : "Failed") << std::endl;
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

    return testExecutor.runAll();
}