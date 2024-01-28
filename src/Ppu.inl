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
                auto& ppuCtrl = registers.getPpuCtrl();
                const auto& ppuStatus = registers.getPpuStatus();
                auto oldVBlankNmi = ppuCtrl.VBlankNmi;
                ppuCtrl = data;
                if(!oldVBlankNmi && ppuCtrl.VBlankNmi && ppuStatus.inVBlank) {
                    triggerNmi();
                }
            }
            break;
        case 1: // 0x2001 PPUMASK - Ppu mask register
            if constexpr (IsWrite) {
                auto& ppuMask = registers.getPpuMask();
                ppuMask = data;
            }
            break;
        case 2: // 0x2002 PPUSTATUS - Ppu status register
            if constexpr (!IsWrite) {
                auto& ppuStatus = registers.getPpuStatus();
                result = ppuStatus | (openBusContents & 0x1F);
                ppuStatus.inVBlank = false;
                offsetToggleLatch = false;
            }
            break;
        case 3: // 0x2003 OAMADDR - OAM address port
            if constexpr (IsWrite) {
                auto& oamAddr = registers.getOamAddr();
                oamAddr = data;
            }
            break;
        case 4: // 0x2004 OAMDATA - OAM data port
            if constexpr (IsWrite) {
                auto& oamAddr = registers.getOamAddr();
                oam.raw[oamAddr] = data;
                oamAddr = oamAddr + 1;
            } else {
                const auto& oamAddr = registers.getOamAddr();
                result = oam.raw[oamAddr];
                refreshOpenBus(result);
            }
            break;
        case 5: // 0x2005 PPUSCROLL - Ppu scrolling position register (X Scroll on first write, Y Scroll on second write)
            if constexpr (IsWrite) {
                auto& ppuScroll = registers.getPpuScroll();
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
                auto ppuAddress = registers.getPpuAddr();
                if(offsetToggleLatch) {
                    ppuAddress.low = data;
                } else {
                    ppuAddress.high = data;
                }
                offsetToggleLatch = !offsetToggleLatch;
            }
            break;
        case 7: // 0x2007 PPUDATA - Ppu data register
            result = vramReadBuffer;
            if constexpr(IsWrite) {
                // TODO: Write to PPU address space
            } else {
                const auto& ppuCtrl = registers.getPpuCtrl();
                auto& ppuAddress = registers.getPpuAddr();
                result = vramReadBuffer;
                // TODO: Load from PPU address space into the vramReadBuffer
                refreshOpenBus(result);
                ppuAddress = ppuAddress + (ppuCtrl.vramAddressIncrement ? 32 : 1);
            }
            break;
        default:
            break;
    }
    return result;
}