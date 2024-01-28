#pragma once

#include <memory>
#include <string>

#include "CartridgeMapper.hpp"

class Cartridge
{
    public:
        Cartridge() = default;

        bool loadFromFile(std::string filename);

        void setMapper(std::unique_ptr<CartridgeMapper> mapper);

        void write(u16 addr, u8 value);

        u8 read(u16 addr);

    private:
        std::unique_ptr<CartridgeMapper> mapper;
};