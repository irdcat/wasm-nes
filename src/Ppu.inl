template <bool IsWrite>
u8 Ppu::access(u8 index, u8 data)
{
    static auto offsetToggleLatch = false;
    auto refreshOpenBus = [this](u8 value){
        openBusDecayTimer = 77777;
        openBusContents = value;
    };

    u8 result = openBusContents;
    if constexpr (IsWrite) {
        refreshOpenBus(data);
    }

    switch(index)
    {
        case 0: // 0x2000 PPUCTRL - Ppu control register
            if constexpr (IsWrite) {
                auto oldVBlankNmi = registers.ppuCtrl.VBlankNmi;
                registers.ppuCtrl = data;
                if(!oldVBlankNmi && registers.ppuCtrl.VBlankNmi && registers.ppuStatus.inVBlank) {
                    triggerNmi();
                }
            }
            break;
        case 1: // 0x2001 PPUMASK - Ppu mask register
            if constexpr (IsWrite) {
                registers.ppuMask = data;
            }
            break;
        case 2: // 0x2002 PPUSTATUS - Ppu status register
            if constexpr (!IsWrite) {
                result = registers.ppuStatus | (openBusContents & 0x1F);
                registers.ppuStatus.inVBlank = false;
                offsetToggleLatch = false;
            }
            break;
        case 3: // 0x2003 OAMADDR - OAM address port
            if constexpr (IsWrite) {
                registers.oamAddr = data;
            }
            break;
        case 4: // 0x2004 OAMDATA - OAM data port
            if constexpr (IsWrite) {
                oam.raw[registers.oamAddr] = data;
                registers.oamAddr = registers.oamAddr + 1;
            } else {
                result = oam.raw[registers.oamAddr];
                refreshOpenBus(result);
            }
            break;
        case 5: // 0x2005 PPUSCROLL - Ppu scrolling position register (X Scroll on first write, Y Scroll on second write)
            if constexpr (IsWrite) {
                if(offsetToggleLatch) {
                    registers.ppuScroll.y = data;
                } else {
                    registers.ppuScroll.x = data;
                }
                offsetToggleLatch = !offsetToggleLatch;
            }
            break;
        case 6: // 0x2006 PPUADDR - Ppu address register (MSB on first write, LSB on second write)
            if constexpr (IsWrite) {
                if(offsetToggleLatch) {
                    registers.ppuAddr.low = data;
                } else {
                    registers.ppuAddr.high = data;
                }
                offsetToggleLatch = !offsetToggleLatch;
            }
            break;
        case 7: // 0x2007 PPUDATA - Ppu data register
            result = vramReadBuffer;
            if constexpr(IsWrite) {
                ppuWrite(registers.ppuAddr, data);
                result = data;
            } else {
                auto ppuData = ppuRead(registers.ppuAddr);
                if((registers.ppuAddr & 0x3F00) == 0x3F00) {
                    result = (openBusContents & 0xC0) | (ppuData & 0x3F);
                }
                vramReadBuffer = ppuData;
            }
            refreshOpenBus(result);
            registers.ppuAddr = registers.ppuAddr + (registers.ppuCtrl.vramAddressIncrement ? 32 : 1);
            break;
        default:
            break;
    }
    return result;
}