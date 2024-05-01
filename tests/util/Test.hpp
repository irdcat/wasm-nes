#pragma once

#include <string>

class Test
{
    public:
        explicit Test(const std::string& name);

        virtual ~Test() = default;

        virtual bool setUp() { return true; };

        virtual int run() = 0;

        std::string getName();

        virtual std::string getFailureReason() { return std::string(); }
    
    private:
        std::string name;
};