#include "Ppu.hpp"
#include "Cpu.hpp"

Ppu::Ppu(const std::shared_ptr<Cartridge>& cartridge,
    const std::function<void()>& nmiTriggerCallback,
    const std::function<void()>& vblankCallback)
    : cartridge(cartridge)
    , registers()
    , openBusDecayTimer(0)
    , openBusContents(0)
    , vramReadBuffer(0)
    , scanline(261)
    , scanlineEndPosition(341)
    , renderingPositionX(0)
    , offsetToggleLatch(false)
    , evenOddFrameToggle(false)
    , patternTableAddress(0)
    , attributeTableAddress(0)
    , nametableAddress(0)
    , tilePattern(0)
    , tileAttributes(0)
    , bgShiftPattern(0)
    , bgShiftAttributes(0)
    , vram()
    , oam()
    , oam2()
    , palette()
    , oamTempData(0)
    , spritePrimaryOamPosition(0)
    , spriteSecondaryOamPosition(0)
    , spriteRenderingPosition(0)
    , nmiTriggerCallback(nmiTriggerCallback)
    , vblankCallback(vblankCallback)
{
    registers.ppuCtrl = 0x00;
    registers.ppuMask = 0x00;
    registers.ppuStatus = 0xA0;
    registers.oamAddr = 0x00;
    registers.taddr = 0x0000;
    registers.vaddr = 0x0000;
    vramReadBuffer = 0x00;
}

void Ppu::reset()
{
    registers.ppuCtrl = 0x00;
    registers.ppuMask = 0x00;
    registers.ppuStatus = registers.ppuStatus & 0x80;
    offsetToggleLatch = false;
    evenOddFrameToggle = false;
}

/**
 * Read from MMIO (Memory mapped IO) PPU registers.
 */
u8 Ppu::read(u8 index)
{
    // Reads from non-read registers are returning open bus contents
    u8 result = openBusContents;
    if(index == 2) { // 0x2002 PPUSTATUS - Ppu status register
        // PPUSTATUS register only occupies 3 bits, 
        // so remaning 5 are populated with open bus contents
        result = registers.ppuStatus | (openBusContents & 0x1F);
        // Read form PPUSTATUS resets inVBlank flag
        registers.ppuStatus.inVBlank = false;
        // Read from PPUSTATUS also resets internal write latch 
        // that is used by PPUSCROLL and PPUADDR registers 
        offsetToggleLatch = false;
    } else if (index == 4) { // 0x2004 OAMDATA - OAM data port
        if (scanline < 240 && renderingPositionX <= 64) {
            // Before sprite evaluation, reads from 0x2004 always return 0xFF
            result = 0xFF;
        } else {
            // Read byte from OAM
            result = oam[registers.oamAddr];
        }
        // Bits 2,3,4 of sprite attributes in OAM are not used
        if ((registers.oamAddr.spriteDataIndex) == 2) {
            result &= 0xE3;
        }
        refreshOpenBus(result);
    } else if (index == 7) { // 0x2007 PPUDATA - Ppu data register
        // Read initial result from the buffer
        result = vramReadBuffer;
        auto ppuData = ppuRead(registers.vaddr.vramAddress);
        // Are we reading palette?
        if((registers.vaddr.vramAddress & 0x3F00) == 0x3F00) {
            // Reads from palette region are returning results immediately
            // Unused color bits are populated with open bus contents.
            result = (openBusContents & 0xC0) | (ppuData & 0x3F);
            // As a side effect buffer is populated with 
            // data coming from "mirrored" last page of nametable.
            vramReadBuffer = ppuRead(0x2F00 | (registers.vaddr.vramAddress & 0xFF));
        } else {
            // Just move the result to the buffer
            vramReadBuffer = ppuData;
        }
        refreshOpenBus(result);
        // Increment address register based on configuration in PPUCTRL
        auto increment = 1;
        if (registers.ppuCtrl.vramAddressIncrement) {
            // Increment amount can be increased by enabling speficic bit of PPUCTRL
            increment = 32;    
        }
        registers.vaddr.vramAddress = registers.vaddr.vramAddress + increment;
    }
    return result;
}

/**
 * Write into MMIO (Memory mapped IO) PPU registers.
 */
void Ppu::write(u8 index, u8 data)
{
    refreshOpenBus(data);
    if(index == 0) { // 0x2000 PPUCTRL - Ppu control register
        auto oldVBlankNmi = registers.ppuCtrl.VBlankNmi;
        registers.ppuCtrl = data;
        // Update temporary address register base nametable
        registers.taddr.baseNametable = registers.ppuCtrl.baseNametableAddress;
        if(!oldVBlankNmi && registers.ppuCtrl.VBlankNmi && registers.ppuStatus.inVBlank) {
            // Triggers NMI if PPU has reached VBlank, 
            // but NMI was enabled while being previously disabled
            nmiTriggerCallback();
        }
    } else if (index == 1) { // 0x2001 PPUMASK - Ppu mask register
        registers.ppuMask = data;
    } else if (index == 3) { // 0x2003 OAMADDR - OAM address port
        registers.oamAddr = data;
    } else if (index == 4) { // 0x2004 OAMDATA - OAM data port
        // Writes to OAM via OAMDATA register are resulting in oam address incrementation
        oam[registers.oamAddr.raw++] = data;
    } else if (index == 5) { // 0x2005 PPUSCROLL - Ppu scrolling position register (X Scroll on first write, Y Scroll on second write)
        if(offsetToggleLatch) {
            // Second write to PPUSCROLL register updates Y scroll
            // components of temporary address register
            registers.taddr.coarseY = data >> 3;
            registers.taddr.fineY = data & 0x7;
        } else {
            // First write to PPUSCROLL register updates X scroll
            // components of temporary address register
            registers.taddr.coarseX = data >> 3;
            // In reality fine X is a separate 3 bit register, 
            // but for convienience here it is a part of internal V register.
            // However it's value is not included while resolving raw V value. 
            registers.vaddr.fineX = data & 0x7;
        }
        // Toggle write latch. Note that it is shared with PPUADDR.
        offsetToggleLatch = !offsetToggleLatch;
    } else if (index == 6) { // 0x2006 PPUADDR - Ppu address register (MSB on first write, LSB on second write)
        if(offsetToggleLatch) {
            // Second write to PPUADDR updates lower bits of internal T register. 
            registers.taddr.vramAddressLow = data;
            // After an update, value of T is copied into V.
            registers.vaddr.raw = registers.taddr.raw;
        } else {
            // First write to PPUADDR updates higher bits of internal T register.
            // 14th bit of T register is not updated
            registers.taddr.vramAddressHigh = data & 0x3F;
        }
        // Toggle write latch. Note that it is shared with PPUSCROLL
        offsetToggleLatch = !offsetToggleLatch;
    } else if (index == 7) { // 0x2007 PPUDATA - Ppu data register
        // Writes to PPUDATA immediately take effect on memory,
        // and they don't have any mirroring related side effects.
        auto& vramAddress = registers.vaddr.vramAddress;
        ppuWrite(vramAddress, data);
        refreshOpenBus(data);
        // Increment internal address register according to PPUCTRL configuration
        auto increment = 1;
        if (registers.ppuCtrl.vramAddressIncrement) {
            // Increment amount can be increased by enabling speficic bit of PPUCTRL
            increment = 32;
        }
        vramAddress = vramAddress + increment;
    }
}

/**
 * Single PPU tick. Called 3 times per CPU cycle.
 * Each call iterates through one pixel of the screen.
 * Screen is divided into 261 scanlines having 341 pixels each.
 * 
 * 0..239   Visible scanlines
 * 240..241 Idle
 * 241..260 Vertical blank
 * 261      Pre-render scanline
 * 
 * Only first 256 pixels of a scanline are visible on screen.
 * Remaining invisible pixels are processed during Horizontal blank,
 * and it involves fetching background data for 
 * the beginning of the next scanline and last parts of sprite evaluation.
 * 
 * When background rendering is enabled, 
 * pre-render scanline is either 340 or 341 pixels long.
 * This alternates each frame. 
 * In all other cases scanlines are always 341 pixels long.
 */
void Ppu::tick()
{
    const auto& ppuMask = registers.ppuMask; 
    auto& ppuCtrl = registers.ppuCtrl;
    auto& ppuStatus = registers.ppuStatus;

    // Progress decay of open bus contents
    decayOpenBus();

    // Are we processing visible of pre-render scanline?
    if(scanline < 240 || scanline == 261) {
        // If rendering of sprites or background is enabled 
        // in PPUMASK register 
        if(ppuMask.showBgSp) {
            // Decoding rendered tiles
            decodeTiles();
            // In paralallel sprite evaluation also happens
            evaluateSprites();
        }
        // At dot 337 of pre-render scanline and if background rendering is enabled
        // and odd frame is rendered, pre-render scanline is set to be 1 dot shorter 
        if(renderingPositionX == 337 && scanline == 261 && evenOddFrameToggle && ppuMask.showBg) {
            scanlineEndPosition = 340;
        }
        if (ppuMask.showBgSp) {
            // On every 8th dot in range 0..255 or 320..335 starting from 3rd horizontal scroll is incremented
            if(renderingPositionX % 8 == 3 
                && (renderingPositionX < 256 || (renderingPositionX >= 320 && renderingPositionX < 335))) {
                incrementScrollX();
            }
            // At dot 251 vertical component of scroll is incremented
            if(renderingPositionX == 251) {
                incrementScrollY();
            }
            // At dot 257 horizontal scroll is reset
            if(renderingPositionX == 257) {
                resetScrollX();
            }
            // At dot 304 of pre-render scanline, vertical scroll is reset
            // In reality this operation is repeated dot by dot between dots 280..304.
            // There's no sense in such redundancy as in that range, nothing is fetched from nametable.
            if(renderingPositionX == 304 && scanline == 261) {
                resetScrollY();
            }
        }
        // While processing visible scanlines but not during HBLANK
        if(scanline != 261 && renderingPositionX < 256) {
            // Render processed pixel into the framebuffer
            renderPixel();
        }
    }

    // At the beginning of scanline 241 PPU enters VBlank
    if(scanline == 241 && renderingPositionX == 1) {
        ppuStatus.inVBlank = 1;
        if (ppuCtrl.VBlankNmi) {
            // Callback called whenever NMI is triggered by PPU
            nmiTriggerCallback();
        }
        // Callback called whenever we enter VBLANK
        vblankCallback();
    }

    // Close to the end of last scanline PPUSTATUS bits are reset
    if(scanline == 260 && renderingPositionX == 340) {
        ppuStatus.inVBlank = 0;
        evenOddFrameToggle = !evenOddFrameToggle;
    }

    if(scanline == 261 && renderingPositionX == 1) {
        ppuStatus.spriteZeroHit = 0;
        ppuStatus.spriteOverflow = 0;
    }

    // Update rendering position and proceed to the next scanline 
    // if the end of the current one is reached
    renderingPositionX = (renderingPositionX + 1) % scanlineEndPosition;
    if(renderingPositionX == 0) {
        scanlineEndPosition = 341;
        scanline = ((scanline + 1) % 262);
    }
}

/**
 * Return internal framebuffer. Such framebuffer was non-existent of a real PPU. 
 */
const Ppu::Framebuffer& Ppu::getFramebuffer()
{
    return framebuffer;
}

/**
 * Helper method responsible for interleaving pattern bits from 2 different memory locations.
 * 
 * Given the input:
 * msb: 7654 3210
 * lsb: HGFE DCBA
 * 
 * Output will be: 7H6G 5F4E 3D2C 1B0A 
 */
u16 Ppu::interleavePatternBytes(u8 lsb, u8 msb)
{
    // Given the input as in example above
    // Pattern initially is 7654 3210 HGFE DCBA
    auto pattern = u16(lsb) | u16(msb) << 8;
    // First hex digits in the middle swap places
    // Result will be 7654 HGFE 3210 DCBA
    pattern = (pattern & 0xF00F) | ((pattern & 0xF00) >> 4) | ((pattern & 0xF0) << 4);
    // Then 2 bit portions of the pattern gets swapped between each other
    // Result will be 76HG 54FE 32DC 10BA
    pattern = (pattern & 0xC3C3) | ((pattern & 0x3030) >> 2) | ((pattern & 0xC0C) << 2);
    // Then the last step is to swap places between individual bits
    // It will produce desired outcome as currently pattern consists of bits arranged into pairs
    // So the last step is to swap bits between these pairs
    // Result will be 7H6G 5F4E 3D2C 1B0A
    pattern = (pattern & 0x9999) | ((pattern & 0x4444) >> 1) | ((pattern & 0x2222) << 1);
    return pattern;
}

/**
 * Increment horizontal scrolling components of internal V register.
 * Fine X is not modified during rendering.
 */
void Ppu::incrementScrollX()
{
    auto& vaddr = registers.vaddr;
    vaddr.coarseX++;
    if(vaddr.coarseX == 0) {
        vaddr.baseHorizontalNametable = ~vaddr.baseHorizontalNametable;
    }
}

/**
 * Increment vertical scrolling components of internal V register. 
 */
void Ppu::incrementScrollY()
{
    auto& vaddr = registers.vaddr;
    vaddr.fineY++;
    if (vaddr.fineY == 0) {
        if (vaddr.coarseY == 29) {
            // Because the nametable can only hold 30 rows of tiles
            // Coarse Y value is wrapped after reaching 30, 
            // and this is accompanied by vertical nametable switch
            vaddr.coarseY = 0;
            vaddr.baseVerticalNametable = ~vaddr.baseVerticalNametable;
        } else {
            // Because Coarse Y coordinate is 5 bits long, 
            // it can be set to value greater than 29 by developer.
            // Outcome of this is something that resembles negative scroll 
            // and it is used tu push 1 or 2 topmost rows of the nametable to the overscan area.
            // As a side effect, during Tile ID resolution, 
            // attribute table bytes are being interpreted as nametable bytes. 
            vaddr.coarseY++;
        }
    }
}

/**
 * Resets horizontal scrolling components to values from internal T register
 */
void Ppu::resetScrollX()
{
    auto& vaddr = registers.vaddr;
    const auto& taddr = registers.taddr;
    vaddr.coarseX = static_cast<u8>(taddr.coarseX);
    vaddr.baseHorizontalNametable = static_cast<u8>(taddr.baseHorizontalNametable);
}

/**
 * Resets vertical scrolling components to values from internal T register
 */
void Ppu::resetScrollY()
{
    auto& vaddr = registers.vaddr;
    const auto& taddr = registers.taddr;
    vaddr.coarseY = static_cast<u8>(taddr.coarseY);
    vaddr.baseVerticalNametable = static_cast<u8>(taddr.baseVerticalNametable);
    vaddr.fineY = static_cast<u8>(taddr.fineY);
}

/**
 * Decode currently rendered tiles. 
 * This includes background tiles but also sprite tiles, 
 * that are rendered on the current scanline.
 * 
 * PPU VRAM memory is divided into 2 parts:
 * - Nametable (960 bytes)
 * - Attribute table (64 bytes)
 * 
 * Nametable holds references to tiles that are kept in CHR ROM. CHR ROM is divided into 1KB pattern tables.
 * Pattern holds information about a tile in which information about a pixel is held within 2 bits, 
 * thus each byte from pattern table holds information about 4 pixels.
 * These 2 bits does not directly encode information about a color, 
 * but instead it is an index of the color from the pallete.
 * 
 * Pallete color indexes are resolved as follows:
 * 
 *   LSBs         MSBs        Result
 * 
 * 01000001     00000001     01000003
 * 11000010     00000010     11000030
 * 01000100     00000100     01000300
 * 01001000  +  00001000  =  01003000
 * 00010000     00010110     00030220
 * 00100000     00100001     00300002
 * 01000000     01000010     03000020
 * 10000000     10000111     30000222
 * 
 * Starting position of the bytes for tile is always divisible by 8.
 * Bytes are ordered in memory in a simple way: 8 bytes with LSBs then 8 bytes with MSBs.
 * To combine two pattern bytes from memory at address [addr], 
 * LSBs has to be read from address [addr], and MSBs from [addr+8]
 * 
 * PPU in it's pallete memory is capable of holding 8 different 4 color palettes.
 * 
 * Attribute table holds the information about which palette should be used while rendering certain tiles.
 * However PPU is not capable of specyfing different palettes for each tile separately. 
 * Instead each atribute table byte controls the palette of a 4x4 tile part (32x32 px) of the nametable that are arranged this way:
 * 
 * +-------+-------+
 * |   .   |   .   |
 * | - + - | - + - |
 * |   .   |   .   |
 * +-------+-------+
 * |   .   |   .   |
 * | - + - | - + - |
 * |   .   |   .   |
 * +-------+-------+
 * 
 * Attribute table byte is divided into 2 bit areas and each of them covers 2x2 tile (16x16 px) block.
 * Bits of attribute table byte cover areas as follows:
 * 
 * 76543210
 * ||||||++--- Color bits for top left quadrant of this byte
 * ||||++----- Color bits for top right quadrant of this byte
 * ||++------- Color bits for bottom left quadrant of this byte
 * ++--------- Color bits for bottom right quadrant of this byte
 */
void Ppu::decodeTiles()
{
    // During dots 0..255 PPU decodes data for the next tile to be rendered
    // During dots 320..335 PPU decodes data for tiles that 
    // will be rendered at the beginning of the next scanlines
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
            // Fetch nametable address into internal register.
            // Low 12 bits of internal V register are ORed with 2 most significant bits set to 0x2.
            nametableAddress = 0x2000 + (vaddr.raw & 0xFFF);
            break;

        case 1: // Nametable access
            // Fetch address of pattern of the next tile
            // Pattern table is chosen based on bit in PPUCTRL
            patternTableAddress = ppuCtrl.backgroundPatternTableAddress << 12;
            // Tile pattern is chosen based on tile ID read from nametable
            // Tile pattern row is chosen based on fineY value
            patternTableAddress += (ppuRead(nametableAddress) << 4) + vaddr.fineY;
            // Shift previously read tile pattern and attributes into internal shift registers
            if(shouldDecodeTile) {
                // Multiplication by 0x10000 is an equivalent of shifting left by 16 bits
                bgShiftPattern = (bgShiftPattern >> 16) | 0x00010000 * tilePattern;
                // Attributes for the 4 tiles are encoded into 2 bit variables.
                // Multiplying it by 0x5555 will cause it to be spread and repeated across 16 bits.
                bgShiftAttributes = (bgShiftAttributes >> 16) | tileAttributes * 0x55550000;
            }
            if (renderingPositionX == 257) {
                // Reset pointer to OAM3
                spriteRenderingPosition = 0;
            }
            break;

        case 2: // Point to attribute table
            if(shouldDecodeTile) {
                // Tiles in the nametable only occupy 960 bytes.
                // The rest of 64 bytes are occupied by tile attributes.
                // Here the address of attribute table is fetched.
                // Because attribute table bytes control whole quadrants of nametable,
                // lower bits of coarse coordinates are not used.
                attributeTableAddress = 0x23C0
                    | (vaddr.baseNametable << 10) 
                    | ((vaddr.coarseY >> 2) << 3) 
                    | (vaddr.coarseX >> 2);
            }
            break;

        case 3: // Attribute table access or sprite pattern table access
            // Are we decoding tiles?
            if(shouldDecodeTile) {
                // Read next tile attributes are read.
                tileAttributes = (ppuRead(attributeTableAddress) >> ((vaddr.coarseX & 2) + 2 * (vaddr.coarseY & 2))) & 3;
            } else if (spriteRenderingPosition < spriteSecondaryOamPosition) {
                // OAM 3 is an arbitrary structure that didn't exist on a real PPU,
                // but because secondary OAM contains sprites to be rendered on the next scanline
                // similar structure has been introduced to contain sprites to be rendered in the current scanline
                auto& currentSprite = oam3[spriteRenderingPosition];
                currentSprite = oam2[spriteRenderingPosition];
                unsigned y = scanline - currentSprite.positionY;
                if(currentSprite.attributes.verticalFlip) {
                    y ^= (ppuCtrl.spriteSize ? 15 : 7);
                }
                // Pattern table for the sprite pattern is chosen either based on PPUCTRL bit
                // or from sprite attributes if we're dealing with 8x16 sprites
                patternTableAddress = 0x1000 * (ppuCtrl.spriteSize ? currentSprite.tileIndexNumber.bank : ppuCtrl.spritePatternTableAddress);
                // Least significant bit is not taken into account while dealing with large sprites,
                // because their pattern is contained within 2 bytes
                patternTableAddress += 0x10 * (ppuCtrl.spriteSize ? currentSprite.tileIndexNumber.tileNumber : currentSprite.tileIndexNumber.raw);
                // Choose appropriate row based on previously calculated position that accounts for vertical flip
                patternTableAddress += (y & 7) + (y & 8) * 2;
            }
            break;

        case 5: // Background LSB
            // Read low byte of tile pattern of the next tile
            tilePattern = ppuRead(patternTableAddress);
            break;

        case 7: // Background MSB
            // Read high byte of tile pattern of the next tile
            // after that result is immediately interleaved with low byte
            tilePattern = interleavePatternBytes(tilePattern, ppuRead(patternTableAddress | 8));
            // If we're not decoding tiles, then we're reading pattern of the sprite to be rendered
            if(!shouldDecodeTile && spriteRenderingPosition < spriteSecondaryOamPosition) {
                oam3[spriteRenderingPosition++].pattern = tilePattern;
            }
            break;

        default:
            break;
    }
}

/**
 * Evaluate sprites that will be rendered on the next scanline. 
 */
void Ppu::evaluateSprites()
{
    auto& ppuCtrl = registers.ppuCtrl;
    auto& ppuMask = registers.ppuMask;
    auto& ppuStatus = registers.ppuStatus;
    auto& oamAddr = registers.oamAddr;

    if (renderingPositionX == 0) {
        // At the start of each scanline, internal pointers to OAMs are reset to 0
        spritePrimaryOamPosition = 0;
        spriteSecondaryOamPosition = 0;
        if(ppuMask.showSp) {
            // If sprite rendering is enabled, OAMADDR is also reset
            oamAddr = 0;
        }
    }

    if (renderingPositionX < 64) {
        // Sprite evaluation does not take place before dot 64
        return;
    } else if (renderingPositionX == 64) {
        for(auto& sprite : oam2) {
            sprite.raw[0] = sprite.raw[1] = sprite.raw[2] = sprite.raw[3] = 0xFF;
        }
        oamTempData = oam[oamAddr];
    } else if(renderingPositionX <= 256 && renderingPositionX % 2 != 0) {
        // On even cycles PPU reads from primary OAM
        // on odd cycles previously read data is transfered to secondary OAM.
        auto spriteEvaluationPhase = oamAddr.spriteDataIndex;
        oamAddr.raw++;
        if (spriteEvaluationPhase == 0) {
            spritePrimaryOamPosition++;
            if(spritePrimaryOamPosition > 64) {
                oamAddr = 0;
                return;
            }
        }

        // When secondary OAM hasn't been populated yet, transfer the data from primary OAM
        if (spriteSecondaryOamPosition < 8) {
            oam2[spriteSecondaryOamPosition].raw[spriteEvaluationPhase] = oamTempData;
        }

        if (spriteEvaluationPhase == 0) {
            oam2[spriteSecondaryOamPosition].spriteIndex = oamAddr.spriteIndex;
            // Check whether sprite vertical position overlaps currently rendered scanline
            u8 top = oamTempData;
            u8 bottom = top + (ppuCtrl.spriteSize ? 16 : 8);
            if(scanline >= top && scanline < bottom) {
                return;
            }
            oamAddr = oamAddr.raw + 3;
        }

        if (spriteEvaluationPhase == 3) {
            // TODO: Implement sprite overflow bug
            if (spriteSecondaryOamPosition < 8) {
                spriteSecondaryOamPosition++;
            } else {
                // Update overflow flag in PPUSTATUS
                ppuStatus.spriteOverflow = 1;
            }
        }
    } else {
        oamTempData = oam[oamAddr];
    }
}

/**
 * Render pixel based on data fetched into internal shift registers,
 * and internal PPU configuration. 
 */
void Ppu::renderPixel()
{
    bool isOnEdge = renderingPositionX < 8 || renderingPositionX >= 248;
    bool showSprites = registers.ppuMask.showSp && (!isOnEdge || registers.ppuMask.showSp8);
    bool showBackground = registers.ppuMask.showBg && (!isOnEdge || registers.ppuMask.showBg8);

    unsigned fx = registers.vaddr.fineX;
    bool xDivisibleBy8 = (renderingPositionX & 7) == 0;
    unsigned patternPosition = 15 - (((renderingPositionX & 7) + fx + 8 * !xDivisibleBy8) & 15);
    unsigned pixel = 0;
    unsigned attributes = 0;
    // If we have to render background pixel
    if(showBackground) {
        // Get pixel from tile pattern data from the internal shift register
        pixel = (bgShiftPattern >> (patternPosition * 2)) & 3;
        // Get tile attributes for the given pixel, 
        // however reading attributes is pointless when dealing with transparent pixels (pixel 0)
        attributes = (bgShiftAttributes >> (patternPosition * 2)) & (pixel > 0 ? 3 : 0);
    }

    // If we have to render sprite pixel
    if(showSprites) {
        // Get sprites that have to be rendered in the current scanline from OAM3
        for(unsigned spriteNumber = 0; spriteNumber < spriteRenderingPosition; spriteNumber++) {
            const auto& sprite = oam3[spriteNumber];
            unsigned xDiff = renderingPositionX - sprite.positionX;
            // Assert whether sprite's horizontal position overlaps position of currently rendered pixel
            if(xDiff >= 8) {
                continue;
            }
            // Flip horizontaly if enabled in sprite attributes
            if(!(sprite.attributes.horizontalFlip)) {
                xDiff = 7 - xDiff;
            }
            // Get pixel pattern
            u8 spritePixel = (sprite.pattern >> (xDiff * 2)) & 3;
            // If pixel is transparent, then we are rendering background pixel or nothing
            if(spritePixel == 0) {
                continue;
            }
            // Check for sprite 0 hit when opaque background pixel overlaps or is overlapped by opaque sprite pixel.
            // In real world, use case for using Sprite 0 Hit flag is to check whether PPU,
            // has reached certain Y position, given by the Y position of sprite with index 0.
            if(renderingPositionX < 255 && pixel > 0 && sprite.spriteIndex == 0) {
                registers.ppuStatus.spriteZeroHit = 1;
            }
            // If sprite's priority is set to 0, that means that sprite should be in front of background.
            // Or background pixel is transparent, render sprites pixel.
            if(sprite.attributes.priority == 0 || pixel == 0) {
                pixel = spritePixel;
                // Sprites use palletes 4-7, that are indexed by a 2 bit attribute
                attributes = (sprite.attributes.pallete) + 4;
            }
            break;
        }
    }

    // Choose pixel color from the palette that is initialized by the executed program.
    // Apply greyscale if enabled in PPUMASK register
    pixel = palette[(attributes * 4 + pixel) & 0x1F] & (registers.ppuMask.greyscale ? 0x30 : 0x3F);
    // Update internal framebuffer
    // Internal framebuffer holds palette color indexes that can be translated later to RGB colors
    framebuffer[scanline * SCREEN_WIDTH + renderingPositionX] = pixel;
}

/**
 * Refreshed value that is currently kept as a open bus content.
 */
void Ppu::refreshOpenBus(u8 value)
{
    // Because time after which open bus value decay 
    // is dependent on many factors including external ones,
    // arbitrary value is chosen as an amount of ticks.
    openBusDecayTimer = OPEN_BUS_DECAY_TICKS;
    openBusContents = value;
}

/**
 * Decay open bus value. This is called each tick. 
 */
void Ppu::decayOpenBus()
{
    if(openBusDecayTimer > 0) {
        openBusDecayTimer--;
        if(openBusDecayTimer == 0) {
            openBusContents = 0;
        }
    }
}

/**
 * Read value from PPU bus. 
 */
u8 Ppu::ppuRead(u16 addr)
{
    addr &= 0x3FFF;
    if(addr >= 0x3F00) {
        // Addresses between 0x3F00 - 0x3FFF are occupied by a palette.
        return paletteRef(addr & 0xFF);
    } else if(addr >= 0x2000) {
        // Addresses between 0x2F00 - 0x3EFF are occupied by a VRAM
        if (addr >= 0x3000) {
            addr -= 0x1000;
        }
        auto mirroringType = cartridge->getMirroringType();
        addr = resolveNametableAddress(addr, mirroringType);
        return vram[addr];
    } 

    // Read something from cartridge.
    return cartridge->read(addr);
}

/**
 * Write value into PPU bus. 
 */
void Ppu::ppuWrite(u16 addr, u8 value)
{
    addr &= 0x3FFF;
    if(addr >= 0x3F00) {
        // Addresses between 0x3F00 - 0x3FFF are occupied by a palette.
        auto& palette = paletteRef(addr & 0xFF);
        palette = value;
        return;
    } else if (addr >= 0x2000) {
        // Addresses between 0x2000 - 0x3EFF are occupied by a VRAM
        if (addr >= 0x3000) {
            addr -= 0x1000;
        }
        auto mirroringType = cartridge->getMirroringType();
        addr = resolveNametableAddress(addr, mirroringType);
        vram[addr] = value;
    }

    // Read something from cartridge.
    cartridge->write(addr, value);
}

/**
 * Get a reference to value from palette memory.
 * Palette memory is divided into 8 palletes with 4 colors each.
 * Addresses 3F10, 3F14, 3F18, 3F1C are mirrors of 3F00, 3F04, 3F08, 3F0C.
 * 
 * First 4 palettes are used for background tiles.
 * Last 4 palettes are used for sprite tiles.
 * 
 * Every first color of palette is either universal background color (3F00, 3F10),
 * or is unused.
 */
u8& Ppu::paletteRef(u8 addr)
{
    if (addr % 4 == 0) {
        addr &= 0xF;
    } else {
        addr &= 0x1F;
    }

    return palette[addr];
}

u16 Ppu::resolveNametableAddress(u16 addr, MirroringType mirroringType)
{
    using enum MirroringType;
    switch (mirroringType)
    {
        case Horizontal: 
            addr = addr & 0x3FF | ((addr & 0x800) >> 1);
            break;
        
        case Vertical:
            addr = addr & 0x3FF | (addr & 0x400);
            break;
        
        case SingleScreenLow:
            addr = addr & 0x3FF;
            break;
        
        case SingleScreenHigh:
            addr = addr & 0x3FF | 0x400;
            break;

        default:
            // TODO: Handle other types of mirroring
            addr &= 0x7FF;
            break;
    }
    return addr;
}
