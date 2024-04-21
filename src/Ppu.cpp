#include "Ppu.hpp"
#include "Cpu.hpp"

Ppu::Ppu(const std::shared_ptr<Cartridge>& cartridge, 
    const std::shared_ptr<Framebuffer>& framebuffer,
    const std::function<void()>& nmiTriggerCallback,
    const std::function<void()>& vblankInterruptCallback)
    : cartridge(cartridge)
    , framebuffer(framebuffer)
    , registers()
    , openBusDecayTimer(0)
    , openBusContents(0)
    , vramReadBuffer(0)
    , scanline(-1)
    , scanlineEndPosition(341)
    , renderingPositionX(0)
    , offsetToggleLatch(false)
    , evenOddFrameToggle(false)
    , patternTableAddress(0)
    , attributeTableAddress(0)
    , tilePattern(0)
    , tileAttributes(0)
    , bgShiftPattern(0)
    , bgShiftAttributes(0)
    , oam()
    , oam2()
    , palette()
    , oamTempData(0)
    , spritePrimaryOamPosition(0)
    , spriteSecondaryOamPosition(0)
    , spriteRenderingPosition(0)
    , nmiTriggerCallback(nmiTriggerCallback)
    , vblankInterruptCallback(vblankInterruptCallback)
{
}

u8 Ppu::read(u8 index)
{
    u8 result = openBusContents;
    if(index == 2) { // 0x2002 PPUSTATUS - Ppu status register
        result = registers.ppuStatus | (openBusContents & 0x1F);
        registers.ppuStatus.inVBlank = false;
        offsetToggleLatch = false;
    } else if (index == 4) { // 0x2004 OAMDATA - OAM data port
        result = (registers.oamAddr & 3) == 2 ? oam[registers.oamAddr] & 0xE3 : oam[registers.oamAddr];
        refreshOpenBus(result);
    } else if (index == 7) { // 0x2007 PPUDATA - Ppu data register
        result = vramReadBuffer;
        auto ppuData = ppuRead(registers.vaddr.vramAddress);
        if((registers.vaddr.vramAddress & 0x3F00) == 0x3F00) {
            result = (openBusContents & 0xC0) | (ppuData & 0x3F);
            vramReadBuffer = ppuRead(0x2F00 | (registers.vaddr.vramAddress & 0xFF));
        } else {
            vramReadBuffer = ppuData;
        }
        refreshOpenBus(result);
        registers.vaddr.vramAddress = registers.vaddr.vramAddress + (registers.ppuCtrl.vramAddressIncrement ? 32 : 1);
    }
    return result;
}

void Ppu::write(u8 index, u8 data)
{
    refreshOpenBus(data);
    if(index == 0) { // 0x2000 PPUCTRL - Ppu control register
        auto oldVBlankNmi = registers.ppuCtrl.VBlankNmi;
        registers.ppuCtrl = data;
        registers.taddr.baseNametable = registers.ppuCtrl.baseNametableAddress;
        if(!oldVBlankNmi && registers.ppuCtrl.VBlankNmi && registers.ppuStatus.inVBlank) {
            nmiTriggerCallback();
        }
    } else if (index == 1) { // 0x2001 PPUMASK - Ppu mask register
        registers.ppuMask = data;
    } else if (index == 3) { // 0x2003 OAMADDR - OAM address port
        registers.oamAddr = data;
    } else if (index == 4) { // 0x2004 OAMDATA - OAM data port
        oam[registers.oamAddr++] = data;
    } else if (index == 5) { // 0x2005 PPUSCROLL - Ppu scrolling position register (X Scroll on first write, Y Scroll on second write)
        if(offsetToggleLatch) {
            registers.taddr.coarseY = data >> 3;
            registers.taddr.fineY = data & 0x7;
        } else {
            registers.taddr.coarseX = data >> 3;
            registers.vaddr.fineX = data & 0x7;
        }
        offsetToggleLatch = !offsetToggleLatch;
    } else if (index == 6) { // 0x2006 PPUADDR - Ppu address register (MSB on first write, LSB on second write)
        if(offsetToggleLatch) {
            registers.taddr.vramAddressLow = data;
            registers.vaddr.raw = registers.taddr.raw;
        } else {
            registers.taddr.vramAddressHigh = data & 0x3F;
        }
        offsetToggleLatch = !offsetToggleLatch;
    } else if (index == 7) { // 0x2007 PPUDATA - Ppu data register
        auto& vramAddress = registers.vaddr.vramAddress;
        ppuWrite(vramAddress, data);
        refreshOpenBus(data);
        vramAddress = vramAddress + (registers.ppuCtrl.vramAddressIncrement ? 32 : 1);
    }
}

void Ppu::tick()
{
    if(openBusDecayTimer > 0) {
        openBusDecayTimer--;
        if(openBusDecayTimer == 0) {
            openBusContents = 0;
        }
    }

    if(scanline < 240) {
        if(registers.ppuMask.showBgSp) {
            renderingTick();
        }
        if(scanline != -1 && renderingPositionX < 256) {
            renderPixel();
        }
    }

    if(scanline == 241 && renderingPositionX == 1) {
        registers.ppuStatus.inVBlank = 1;
        nmiTriggerCallback();
        vblankInterruptCallback();
    }

    if(scanline == 260 && renderingPositionX == 330) {
        registers.ppuStatus.spriteZeroHit = 0;
        registers.ppuStatus.spriteOverflow = 0;
        registers.ppuStatus.inVBlank = 0;
        evenOddFrameToggle = !evenOddFrameToggle;
    }

    renderingPositionX++;
    if(renderingPositionX >= scanlineEndPosition) {
        renderingPositionX = 0;
        scanlineEndPosition = 341;
        scanline++;
        if(scanline == 261) {
            scanline = -1;
        }
    }
}

u8 Ppu::getColorFromPalette(u8 paletteId, u8 pixel)
{
    return ppuRead(0x3F00 + (paletteId * 4) + pixel) & 0x3F;
}

u16 Ppu::interleavePatternBytes(u8 lsb, u8 msb)
{
    auto pattern = u16(lsb) | u16(msb) << 8;
    pattern = (pattern & 0xF00F) | ((pattern & 0xF00) >> 4) | ((pattern & 0xF0) << 4);
    pattern = (pattern & 0xC3C3) | ((pattern & 0x3030) >> 2) | ((pattern & 0xC0C) << 2);
    pattern = (pattern & 0x9999) | ((pattern & 0x4444) >> 1) | ((pattern & 0x2222) << 1);
    return pattern;
}

void Ppu::incrementScrollX()
{
    auto& vaddr = registers.vaddr;
    if(++vaddr.coarseX == 0) {
        vaddr.baseHorizontalNametable = ~vaddr.baseHorizontalNametable;
    }
}

void Ppu::incrementScrollY()
{
    auto& vaddr = registers.vaddr;
    if(++vaddr.fineY == 0 && ++vaddr.coarseY == 30) {
        vaddr.coarseY = 0;
        vaddr.baseVerticalNametable = ~vaddr.baseVerticalNametable;
    }
}

void Ppu::renderingTick()
{
    auto shouldDecodeTile = (renderingPositionX >= 0 && renderingPositionX <= 255) 
        || (renderingPositionX >= 320 && renderingPositionX <= 335);
    
    auto& vaddr = registers.vaddr;
    auto& taddr = registers.taddr;
    auto& ppuCtrl = registers.ppuCtrl;
    auto& ppuMask = registers.ppuMask;
    auto& oamAddr = registers.oamAddr;

    switch (renderingPositionX % 8)
    {
        case 0: // Point to nametable
            attributeTableAddress = 0x2000 + (vaddr.raw & 0xFFF);
            if(renderingPositionX == 0) {
                spritePrimaryOamPosition = 0;
                spriteSecondaryOamPosition = 0;
                if(ppuMask.showSp) {
                    oamAddr = 0;
                }
            }
            if(ppuMask.showBg) {
                if(renderingPositionX == 304 && scanline == -1) {
                    vaddr.raw = (unsigned) taddr.raw;
                }
                if(renderingPositionX == 256) {
                    vaddr.coarseX = (unsigned) taddr.coarseX;
                    vaddr.baseHorizontalNametable = (unsigned) taddr.baseHorizontalNametable;
                    spriteRenderingPosition = 0;
                }
            }
            break;

        case 1: // Nametable access
            if(renderingPositionX == 337 && scanline == -1 && evenOddFrameToggle && ppuMask.showBg) {
                scanlineEndPosition = 340;
            }
            patternTableAddress = 0x1000 * ppuCtrl.backgroundPatternTableAddress;
            patternTableAddress += (ppuRead(attributeTableAddress) << 4) + vaddr.fineY;
            if(shouldDecodeTile) {
                bgShiftPattern = (bgShiftPattern >> 16) | 0x00010000 * tilePattern;
                bgShiftAttributes = (bgShiftAttributes >> 16) | tileAttributes * 0x55550000;
            }
            break;

        case 2: // Point to attribute table or nametable with sprites
            if(shouldDecodeTile) {
                attributeTableAddress = 0x23C0 | (vaddr.baseNametable << 11) 
                    | ((vaddr.coarseY >> 2) << 3) | (vaddr.coarseX >> 2);
            } else {
                attributeTableAddress = 0x2000 | (vaddr.raw & 0xFFF);
            }
            break;

        case 3: // Attribute table access
            if(shouldDecodeTile) {
                tileAttributes = (ppuRead(attributeTableAddress) >> ((vaddr.coarseX & 2) + 2 * (vaddr.coarseY & 2))) & 3;
                incrementScrollX();
                if(renderingPositionX == 251) {
                    incrementScrollY();
                }
            } else if (spriteRenderingPosition < spriteSecondaryOamPosition) {
                auto& currentSprite = oam3[spriteRenderingPosition];
                currentSprite = oam2[spriteRenderingPosition];
                unsigned y = scanline - currentSprite.positionY;
                if(currentSprite.attributes.verticalFlip) {
                    y ^= (ppuCtrl.spriteSize ? 15 : 7);
                }
                patternTableAddress = 0x1000 * (ppuCtrl.spriteSize ? currentSprite.tileIndexNumber.bank : ppuCtrl.spritePatternTableAddress);
                patternTableAddress += 0x10 * (ppuCtrl.spriteSize ? currentSprite.tileIndexNumber.tileNumber : currentSprite.tileIndexNumber.raw);
                patternTableAddress += (y & 7) + (y & 8) * 2;
            }
            break;

        case 5: // Background LSB
            tilePattern = ppuRead(patternTableAddress);
            break;

        case 7: // Background MSB
            tilePattern = interleavePatternBytes(tilePattern, ppuRead(patternTableAddress | 8));
            if(!shouldDecodeTile && spriteRenderingPosition < spriteSecondaryOamPosition) {
                oam3[spriteRenderingPosition++].pattern = tilePattern;
            }
            break;

        default:
            break;
    }
    spriteEvaluation();
}

void Ppu::spriteEvaluation()
{
    auto& ppuCtrl = registers.ppuCtrl;
    auto& ppuStatus = registers.ppuStatus;
    auto& oamAddr = registers.oamAddr;

    bool inRangeForSpriteEvaluation = renderingPositionX >= 64 && renderingPositionX < 256;
    bool readFromPrimaryOam = renderingPositionX % 2 != 0;
    u8 spriteEvaluationPhase = 4;
    if(inRangeForSpriteEvaluation && readFromPrimaryOam) {
        spriteEvaluationPhase = registers.oamAddr++ & 0x3;
    }

    switch (spriteEvaluationPhase)
    {
        case 0:
            if(spritePrimaryOamPosition >= 64) {
                oamAddr = 0;
                break;
            }
            spritePrimaryOamPosition++;
            if(spriteSecondaryOamPosition < 8) {
                oam2[spriteSecondaryOamPosition].positionY = oamTempData;
                oam2[spriteSecondaryOamPosition].spriteIndex = (oamAddr >> 2);
            }
            {
                u8 top = oamTempData;
                u8 bottom = top + (ppuCtrl.spriteSize ? 16 : 8);
                if(scanline >= top && scanline < bottom) {
                    break;
                }
                oamAddr = oamAddr + 3;
            }
            break;
        
        case 1:
            if(spriteSecondaryOamPosition < 8) {
                oam2[spriteSecondaryOamPosition].tileIndexNumber.raw = oamTempData;
            }
            break;

        case 2:
            if(spriteSecondaryOamPosition < 8) {
                oam2[spriteSecondaryOamPosition].attributes.raw = oamTempData;
            }
            break;

        case 3:
            if(spriteSecondaryOamPosition < 8) {
                oam2[spriteSecondaryOamPosition].positionX = oamTempData;
                spriteSecondaryOamPosition++;
            } else {
                ppuStatus.spriteOverflow = 1;
            }
            break;
    
        default:
            oamTempData = oam[oamAddr];
            break;
    }
}

void Ppu::renderPixel()
{
    bool isOnEdge = renderingPositionX < 8 || renderingPositionX >= 248;
    bool showSprites =registers.ppuMask.showSp && (!isOnEdge || registers.ppuMask.showSp8);
    bool showBackground = registers.ppuMask.showBg && (!isOnEdge || registers.ppuMask.showBg8);

    unsigned fx = registers.vaddr.fineX;
    bool xDivisibleBy8 = (renderingPositionX & 7) > 0 ? 0 : 1;
    unsigned patternPosition = 15 - (((renderingPositionX & 7) + fx + 8 * !xDivisibleBy8) & 15);
    unsigned pixel = 0;
    unsigned attributes = 0;
    if(showBackground) {
        pixel = (bgShiftPattern >> (patternPosition * 2)) & 3;
        attributes = (bgShiftAttributes >> (patternPosition * 2)) & (pixel > 0 ? 3 : 0);
    }

    if(showSprites) {
        for(unsigned spriteNumber = 0; spriteNumber < spriteRenderingPosition; spriteNumber++) {
            const auto& sprite = oam3[spriteNumber];
            unsigned xDiff = renderingPositionX - sprite.positionX;
            if(xDiff >= 8) {
                continue;
            }
            if(!(sprite.attributes.horizontalFlip)) {
                xDiff = 7 - xDiff;
            }
            u8 spritePixel = (sprite.pattern >> (xDiff * 2)) & 3;
            if(spritePixel == 0) {
                continue;
            }
            if(renderingPositionX < 255 && pixel > 0 && sprite.spriteIndex == 0) {
                registers.ppuStatus.spriteZeroHit = 1;
            }
            if(sprite.attributes.priority == 0 || pixel == 0) {
                attributes = (sprite.attributes.pallete) + 4;
                pixel = spritePixel;
            }
            break;
        }
    }

    pixel = palette[(attributes * 4 + pixel) & 0x1F] & (registers.ppuMask.greyscale ? 0x30 : 0x3F);
    framebuffer->setColor(renderingPositionX, scanline, pixel);
}

void Ppu::refreshOpenBus(u8 value)
{
    openBusDecayTimer = OPEN_BUS_DECAY_TICKS;
    openBusContents = value;
}

u8 Ppu::ppuRead(u16 addr)
{
    addr &= 0x3FFF;
    if(addr >= 0x3F00) {
        addr = addr % 4 == 0 ? addr & 0xF : addr & 0x1F;
        return palette[addr];
    }

    return cartridge->read(addr);
}

void Ppu::ppuWrite(u16 addr, u8 value)
{
    addr &= 0x3FFF;
    if(addr >= 0x3F00) {
        addr = addr % 4 == 0 ? addr & 0xF : addr & 0x1F;
        palette[addr] = value;
        return;
    }

    cartridge->write(addr, value);
}
