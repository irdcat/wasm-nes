.segment "HEADER"
  .byte $4E, $45, $53, $1A ; iNES header identifier
  .byte 2               ; 2x 16KB PRG data
  .byte 1               ; 1x 8KB CHR data
  .byte $01, $00        ; Mapper 0, vertical mirroring

.segment "VECTORS"
  .addr nmi             ; NMI vector
  .addr reset           ; Reset vector
  .addr 0               ; IRQ unused

.segment "STARTUP"

.segment "CODE"

reset:
  sei                   ; Disable IRQs
  cld                   ; Disable decimal mode
  ldx #$40
  stx $4017             ; Disable APU frame IRQ
  ldx #$ff
  txs                   ; Set up stack
  inx                   ; Reset X to 0
  stx $2000             ; Disable NMI
  stx $2001             ; Disable rendering
  stx $4010             ; Disable DMC IRQs

vblankwait1:
  bit $2002             ; Check VBLANK flag
  bpl vblankwait1       ; Loop until VBLANK occurs

clear_memory:
  lda #$00
  sta $0000, x
  sta $0100, x
  sta $0200, x
  sta $0300, x
  sta $0400, x
  sta $0500, x
  sta $0600, x
  sta $0700, x
  inx
  bne clear_memory      ; Loop until clearing all contents of the pages

vblankwait2:
  bit $2002
  bpl vblankwait2       ; Loop until VBLANK occurs, PPU is ready after this

main:
load_palettes:
  lda $2002
  lda #$3f
  sta $2006
  lda #$00
  sta $2006
  ldx #$00
@loop:
  lda palettes, x
  sta $2007
  inx
  cpx #$20
  bne @loop

enable_rendering:
  lda #$80                
  sta $2000             ; Enable NMI
  lda #$10
  sta $2001             ; Enable sprites

forever:
  jmp forever           ; Main loop

nmi:
  ldx #$00
  stx $2003             ; Set OAM address to 0
@loop:
  lda hello, x
  sta $2004             ; Load hello message into OAM
  inx
  cpx #$1c
  bne @loop
  rti

hello:
  .byte $00, $00, $00, $00
  .byte $00, $00, $00, $00
  .byte $6c, $00, $00, $6c
  .byte $6c, $01, $00, $76
  .byte $6c, $02, $00, $80
  .byte $6c, $02, $00, $8A
  .byte $6c, $03, $00, $94

palettes:
  ; Background Palette
  .byte $0f, $00, $00, $00
  .byte $0f, $00, $00, $00
  .byte $0f, $00, $00, $00
  .byte $0f, $00, $00, $00

  ; Sprite Palette
  .byte $0f, $20, $00, $00
  .byte $0f, $00, $00, $00
  .byte $0f, $00, $00, $00
  .byte $0f, $00, $00, $00

.segment "CHARS"
  .byte %11000011	; H (00)
  .byte %11000011
  .byte %11000011
  .byte %11111111
  .byte %11111111
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte $00, $00, $00, $00, $00, $00, $00, $00

  .byte %11111111	; E (01)
  .byte %11111111
  .byte %11000000
  .byte %11111100
  .byte %11111100
  .byte %11000000
  .byte %11111111
  .byte %11111111
  .byte $00, $00, $00, $00, $00, $00, $00, $00

  .byte %11000000	; L (02)
  .byte %11000000
  .byte %11000000
  .byte %11000000
  .byte %11000000
  .byte %11000000
  .byte %11111111
  .byte %11111111
  .byte $00, $00, $00, $00, $00, $00, $00, $00

  .byte %01111110	; O (03)
  .byte %11100111
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte %11100111
  .byte %01111110
  .byte $00, $00, $00, $00, $00, $00, $00, $00