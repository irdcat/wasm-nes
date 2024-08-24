#include "TestExecutor.hpp"
#include <iostream>
#include <sstream>
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
    std::cout << "|" << std::left << std::setw(TEST_NAME_COL_WIDTH) << std::setfill(' ') << "Test name";
    std::cout << "|" << std::left << std::setw(SET_UP_RESULT_COL_WIDTH) << std::setfill(' ') << "Set up";
    std::cout << "|" << std::left << std::setw(RUN_RESULT_COL_WIDTH) << std::setfill(' ') << "Run";
    std::cout << "|" << std::endl;
    for(auto i = 0; i < ALL_WIDTH; i++) {
        std::cout << '-';
    }
    std::cout << std::endl;
    for(auto& test : tests) {
        auto testName = test->getName();
        auto testResult = -1;
        
        std::cout << "|" << std::left << std::setw(TEST_NAME_COL_WIDTH) << std::setfill(' ') << testName;
        std::cout.flush();

        bool hasSetUp = test->setUp();
        std::cout << "|" << std::left << std::setw(SET_UP_RESULT_COL_WIDTH) << std::setfill(' ') << (hasSetUp ? "Done" : "Failed");
        std::cout.flush();

        std::stringstream ss;
        if(hasSetUp) {
            testResult = test->run();
            if(testResult == 0) {
                ss << "Passed";
            } else {
                ss << "Failed (Code: " << testResult << ')';
            }
        }
        std::cout << "|" << std::left << std::setw(RUN_RESULT_COL_WIDTH) << std::setfill(' ') << ss.str();
        std::cout << "|" << std::endl;

        auto failureReason = test->getFailureReason();
        if(failureReason.size() > 0) {
            std::cout << failureReason << std::endl << std::endl;
        }
        result |= testResult;
    }
    return result;
}
