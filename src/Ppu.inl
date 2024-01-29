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

    auto& ppuCtrl = registers.getPpuCtrl();
    auto& ppuMask = registers.getPpuMask();
    auto& ppuStatus = registers.getPpuStatus();
    auto& oamAddr = registers.getOamAddr();
    auto& ppuScroll = registers.getPpuScroll();
    auto& ppuAddr = registers.getPpuAddr();

    switch(index)
    {
        case 0: // 0x2000 PPUCTRL - Ppu control register
            if constexpr (IsWrite) {
                auto oldVBlankNmi = ppuCtrl.VBlankNmi;
                ppuCtrl = data;
                if(!oldVBlankNmi && ppuCtrl.VBlankNmi && ppuStatus.inVBlank) {
                    triggerNmi();
                }
            }
            break;
        case 1: // 0x2001 PPUMASK - Ppu mask register
            if constexpr (IsWrite) {
                ppuMask = data;
            }
            break;
        case 2: // 0x2002 PPUSTATUS - Ppu status register
            if constexpr (!IsWrite) {
                result = ppuStatus | (openBusContents & 0x1F);
                ppuStatus.inVBlank = false;
                offsetToggleLatch = false;
            }
            break;
        case 3: // 0x2003 OAMADDR - OAM address port
            if constexpr (IsWrite) {
                oamAddr = data;
            }
            break;
        case 4: // 0x2004 OAMDATA - OAM data port
            if constexpr (IsWrite) {
                oam.raw[oamAddr] = data;
                oamAddr = oamAddr + 1;
            } else {
                result = oam.raw[oamAddr];
                refreshOpenBus(result);
            }
            break;
        case 5: // 0x2005 PPUSCROLL - Ppu scrolling position register (X Scroll on first write, Y Scroll on second write)
            if constexpr (IsWrite) {
                if(offsetToggleLatch) {
                    ppuScroll.y = data;
                } else {
                    ppuScroll.x = data;
                }
                offsetToggleLatch = !offsetToggleLatch;
            }
            break;
        case 6: // 0x2006 PPUADDR - Ppu address register (MSB on first write, LSB on second write)
            if constexpr (IsWrite) {
                if(offsetToggleLatch) {
                    ppuAddr.low = data;
                } else {
                    ppuAddr.high = data;
                }
                offsetToggleLatch = !offsetToggleLatch;
            }
            break;
        case 7: // 0x2007 PPUDATA - Ppu data register
            result = vramReadBuffer;
            if constexpr(IsWrite) {
                ppuWrite(ppuAddr, data);
                result = data;
            } else {
                auto ppuData = ppuRead(ppuAddr);
                if((ppuAddr & 0x3F00) == 0x3F00) {
                    result = (openBusContents & 0xC0) | (ppuData & 0x3F);
                }
                vramReadBuffer = ppuData;
            }
            refreshOpenBus(result);
            ppuAddr = ppuAddr + (ppuCtrl.vramAddressIncrement ? 32 : 1);
            break;
        default:
            break;
    }
    return result;
}