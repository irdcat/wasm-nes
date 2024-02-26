#pragma once

#include <string>

class Test
{
    public:
        virtual ~Test() = default;

        virtual bool setUp() { return true; };

        virtual int run() = 0;

        virtual std::string name() = 0;
};