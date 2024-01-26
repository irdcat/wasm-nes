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
        case 0:
            if constexpr (IsWrite) {
                auto& ppuCtrl = registers.getPpuCtrl();
                ppuCtrl = data;
            }
            break;
        case 1:
            if constexpr (IsWrite) {
                auto& ppuMask = registers.getPpuMask();
                ppuMask = data;
            }
            break;
        case 2:
            if constexpr (!IsWrite) {
                auto& ppuStatus = registers.getPpuStatus();
                result = ppuStatus | (openBusContents & 0x1F);
                ppuStatus.inVBlank = false;
                offsetToggleLatch = false;
            }
            break;
        case 3:
            if constexpr (IsWrite) {
                auto& oamAddr = registers.getOamAddr();
                oamAddr = data;
            }
            break;
        case 4:
            if constexpr (IsWrite) {
                auto& oamData = registers.getOamData();
                oamData = data;
            } else {
                result = registers.getOamData();
            }
            break;
        case 5:
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
        case 6:
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
        case 7:
            if constexpr (IsWrite) {
                auto& ppuData = registers.getPpuData();
                ppuData = data;
            } else {
                result = registers.getPpuData();
            }
            break;
        default:
            break;
    }
    return result;
}