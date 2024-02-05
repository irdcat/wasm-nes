#pragma once

#include <string>

class Test
{
    public:
        virtual ~Test() = default;

        virtual void setUp() {};

        virtual int run() = 0;

        virtual std::string name() = 0;
};