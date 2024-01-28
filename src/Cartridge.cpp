#include "Cartridge.hpp"

bool Cartridge::loadFromFile(std::string filename)
{
    // TODO: Implement reading cart from file
    return false;
}

void Cartridge::setMapper(std::unique_ptr<CartridgeMapper> mapper)
{
    this->mapper = std::move(mapper);
}

void Cartridge::write(u16 addr, u8 value)
{
    mapper->write(addr, value);
}

u8 Cartridge::read(u16 addr)
{
    return mapper->read(addr);
}
