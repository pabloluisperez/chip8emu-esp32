; CHIP-8 Disassembly
; Size: 132 bytes
; Start address: 0x200

; Registers: V0-VF (VF = flags)
; I = Index register (16-bit)
; DT = Delay Timer
; ST = Sound Timer
; K = Key input

  0x200:  00E0  CLS
  0x202:  A22A  LD   I, 0x22A
  0x204:  600C  LD   V0, 0x0C
  0x206:  6108  LD   V1, 0x08
  0x208:  D01F  DRW  V0, V1, 15
  0x20A:  7009  ADD  V0, 0x09
  0x20C:  A239  LD   I, 0x239
  0x20E:  D01F  DRW  V0, V1, 15
  0x210:  A248  LD   I, 0x248
  0x212:  7008  ADD  V0, 0x08
  0x214:  D01F  DRW  V0, V1, 15
  0x216:  7004  ADD  V0, 0x04
  0x218:  A257  LD   I, 0x257
  0x21A:  D01F  DRW  V0, V1, 15
  0x21C:  7008  ADD  V0, 0x08
  0x21E:  A266  LD   I, 0x266
  0x220:  D01F  DRW  V0, V1, 15
  0x222:  7008  ADD  V0, 0x08
  0x224:  A275  LD   I, 0x275
  0x226:  D01F  DRW  V0, V1, 15

L_228:
  0x228:  1228  JP   L_228

; ===== DATA SECTION (Sprites / Constants) =====
  0x22A:  FF00  .BYTE 0xFF, 0x00  ; 11111111 00000000
  0x22C:  FF00  .BYTE 0xFF, 0x00  ; 11111111 00000000
  0x22E:  3C00  .BYTE 0x3C, 0x00  ; 00111100 00000000
  0x230:  3C00  .BYTE 0x3C, 0x00  ; 00111100 00000000
  0x232:  3C00  .BYTE 0x3C, 0x00  ; 00111100 00000000
  0x234:  3C00  .BYTE 0x3C, 0x00  ; 00111100 00000000
  0x236:  FF00  .BYTE 0xFF, 0x00  ; 11111111 00000000
  0x238:  FFFF  .BYTE 0xFF, 0xFF  ; 11111111 11111111
  0x23A:  00FF  .BYTE 0x00, 0xFF  ; 00000000 11111111
  0x23C:  0038  .BYTE 0x00, 0x38  ; 00000000 00111000
  0x23E:  003F  .BYTE 0x00, 0x3F  ; 00000000 00111111
  0x240:  003F  .BYTE 0x00, 0x3F  ; 00000000 00111111
  0x242:  0038  .BYTE 0x00, 0x38  ; 00000000 00111000
  0x244:  00FF  .BYTE 0x00, 0xFF  ; 00000000 11111111
  0x246:  00FF  .BYTE 0x00, 0xFF  ; 00000000 11111111
  0x248:  8000  .BYTE 0x80, 0x00  ; 10000000 00000000
  0x24A:  E000  .BYTE 0xE0, 0x00  ; 11100000 00000000
  0x24C:  E000  .BYTE 0xE0, 0x00  ; 11100000 00000000
  0x24E:  8000  .BYTE 0x80, 0x00  ; 10000000 00000000
  0x250:  8000  .BYTE 0x80, 0x00  ; 10000000 00000000
  0x252:  E000  .BYTE 0xE0, 0x00  ; 11100000 00000000
  0x254:  E000  .BYTE 0xE0, 0x00  ; 11100000 00000000
  0x256:  80F8  .BYTE 0x80, 0xF8  ; 10000000 11111000
  0x258:  00FC  .BYTE 0x00, 0xFC  ; 00000000 11111100
  0x25A:  003E  .BYTE 0x00, 0x3E  ; 00000000 00111110
  0x25C:  003F  .BYTE 0x00, 0x3F  ; 00000000 00111111
  0x25E:  003B  .BYTE 0x00, 0x3B  ; 00000000 00111011
  0x260:  0039  .BYTE 0x00, 0x39  ; 00000000 00111001
  0x262:  00F8  .BYTE 0x00, 0xF8  ; 00000000 11111000
  0x264:  00F8  .BYTE 0x00, 0xF8  ; 00000000 11111000
  0x266:  0300  .BYTE 0x03, 0x00  ; 00000011 00000000
  0x268:  0700  .BYTE 0x07, 0x00  ; 00000111 00000000
  0x26A:  0F00  .BYTE 0x0F, 0x00  ; 00001111 00000000
  0x26C:  BF00  .BYTE 0xBF, 0x00  ; 10111111 00000000
  0x26E:  FB00  .BYTE 0xFB, 0x00  ; 11111011 00000000
  0x270:  F300  .BYTE 0xF3, 0x00  ; 11110011 00000000
  0x272:  E300  .BYTE 0xE3, 0x00  ; 11100011 00000000
  0x274:  43E0  .BYTE 0x43, 0xE0  ; 01000011 11100000
  0x276:  00E0  .BYTE 0x00, 0xE0  ; 00000000 11100000
  0x278:  0080  .BYTE 0x00, 0x80  ; 00000000 10000000
  0x27A:  0080  .BYTE 0x00, 0x80  ; 00000000 10000000
  0x27C:  0080  .BYTE 0x00, 0x80  ; 00000000 10000000
  0x27E:  0080  .BYTE 0x00, 0x80  ; 00000000 10000000
  0x280:  00E0  .BYTE 0x00, 0xE0  ; 00000000 11100000
  0x282:  00E0  .BYTE 0x00, 0xE0  ; 00000000 11100000

; ===== Instruction Reference =====
; CLS             - Clear screen
; RET             - Return from subroutine
; JP addr         - Jump to address
; CALL addr       - Call subroutine at address
; SE Vx, byte     - Skip next if Vx == byte
; SNE Vx, byte    - Skip next if Vx != byte
; LD Vx, byte     - Load byte into Vx
; ADD Vx, byte    - Add byte to Vx
; OR Vx, Vy       - Vx = Vx OR Vy
; AND Vx, Vy      - Vx = Vx AND Vy
; XOR Vx, Vy      - Vx = Vx XOR Vy
; SUB Vx, Vy      - Vx = Vx - Vy (VF = borrow)
; SHR Vx          - Shift Vx right by 1
; SHL Vx          - Shift Vx left by 1
; RND Vx, byte    - Vx = random AND byte
; DRW Vx, Vy, n   - Draw sprite at (Vx,Vy), height n
; SKP Vx          - Skip if key Vx is pressed
; SKNP Vx         - Skip if key Vx is not pressed
; LD I, addr      - Load address into I
; LD Vx, DT       - Load delay timer into Vx
; LD DT, Vx       - Load Vx into delay timer
; LD ST, Vx       - Load Vx into sound timer
; LD F, Vx        - Load sprite address for digit Vx into I
; LD B, Vx        - Store BCD of Vx at I, I+1, I+2
; LD [I], Vx      - Store V0-Vx at memory[I]
; LD Vx, [I]      - Load V0-Vx from memory[I]