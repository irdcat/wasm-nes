#include "TestExecutor.hpp"
#include <iostream>
#include <iomanip>

TestExecutor::TestExecutor()
    : tests()
    , sut(std::make_shared<SystemUnderTest>())
{
}

int TestExecutor::runAll()
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
        auto testName = test->getName();
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
}
