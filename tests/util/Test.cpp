#include "Test.hpp"

Test::Test(const std::string &name)
    : name(name)
{
}

std::string Test::getName()
{
    return name;
}