; CHIP-8 Disassembly
; Size: 246 bytes
; Start address: 0x200

; Registers: V0-VF (VF = flags)
; I = Index register (16-bit)
; DT = Delay Timer
; ST = Sound Timer
; K = Key input

  0x200:  6A02  LD   VA, 0x02
  0x202:  6B0C  LD   VB, 0x0C
  0x204:  6C3F  LD   VC, 0x3F
  0x206:  6D0C  LD   VD, 0x0C
  0x208:  A2EA  LD   I, 0x2EA
  0x20A:  DAB6  DRW  VA, VB, 6
  0x20C:  DCD6  DRW  VC, VD, 6
  0x20E:  6E00  LD   VE, 0x00
  0x210:  22D4  CALL L_2D4
  0x212:  6603  LD   V6, 0x03
  0x214:  6802  LD   V8, 0x02

L_216:
  0x216:  6060  LD   V0, 0x60
  0x218:  F015  LD   DT, V0

L_21A:
  0x21A:  F007  LD   V0, DT
  0x21C:  3000  SE   V0, 0x00
  0x21E:  121A  JP   L_21A
  0x220:  C717  RND  V7, 0x17
  0x222:  7708  ADD  V7, 0x08
  0x224:  69FF  LD   V9, 0xFF
  0x226:  A2F0  LD   I, 0x2F0
  0x228:  D671  DRW  V6, V7, 1

L_22A:
  0x22A:  A2EA  LD   I, 0x2EA
  0x22C:  DAB6  DRW  VA, VB, 6
  0x22E:  DCD6  DRW  VC, VD, 6
  0x230:  6001  LD   V0, 0x01
  0x232:  E0A1  SKNP V0
  0x234:  7BFE  ADD  VB, 0xFE
  0x236:  6004  LD   V0, 0x04
  0x238:  E0A1  SKNP V0
  0x23A:  7B02  ADD  VB, 0x02
  0x23C:  601F  LD   V0, 0x1F
  0x23E:  8B02  AND  VB, V0
  0x240:  DAB6  DRW  VA, VB, 6
  0x242:  600C  LD   V0, 0x0C
  0x244:  E0A1  SKNP V0
  0x246:  7DFE  ADD  VD, 0xFE
  0x248:  600D  LD   V0, 0x0D
  0x24A:  E0A1  SKNP V0
  0x24C:  7D02  ADD  VD, 0x02
  0x24E:  601F  LD   V0, 0x1F
  0x250:  8D02  AND  VD, V0
  0x252:  DCD6  DRW  VC, VD, 6
  0x254:  A2F0  LD   I, 0x2F0
  0x256:  D671  DRW  V6, V7, 1
  0x258:  8684  ADD  V6, V8
  0x25A:  8794  ADD  V7, V9
  0x25C:  603F  LD   V0, 0x3F
  0x25E:  8602  AND  V6, V0
  0x260:  611F  LD   V1, 0x1F
  0x262:  8712  AND  V7, V1
  0x264:  4602  SNE  V6, 0x02
  0x266:  1278  JP   L_278
  0x268:  463F  SNE  V6, 0x3F
  0x26A:  1282  JP   L_282

L_26C:
  0x26C:  471F  SNE  V7, 0x1F
  0x26E:  69FF  LD   V9, 0xFF
  0x270:  4700  SNE  V7, 0x00
  0x272:  6901  LD   V9, 0x01
  0x274:  D671  DRW  V6, V7, 1
  0x276:  122A  JP   L_22A

L_278:
  0x278:  6802  LD   V8, 0x02
  0x27A:  6301  LD   V3, 0x01
  0x27C:  8070  LD   V0, V7
  0x27E:  80B5  SUB  V0, VB
  0x280:  128A  JP   L_28A

L_282:
  0x282:  68FE  LD   V8, 0xFE
  0x284:  630A  LD   V3, 0x0A
  0x286:  8070  LD   V0, V7
  0x288:  80D5  SUB  V0, VD

L_28A:
  0x28A:  3F01  SE   VF, 0x01
  0x28C:  12A2  JP   L_2A2
  0x28E:  6102  LD   V1, 0x02
  0x290:  8015  SUB  V0, V1
  0x292:  3F01  SE   VF, 0x01
  0x294:  12BA  JP   L_2BA
  0x296:  8015  SUB  V0, V1
  0x298:  3F01  SE   VF, 0x01
  0x29A:  12C8  JP   L_2C8
  0x29C:  8015  SUB  V0, V1
  0x29E:  3F01  SE   VF, 0x01
  0x2A0:  12C2  JP   L_2C2

L_2A2:
  0x2A2:  6020  LD   V0, 0x20
  0x2A4:  F018  LD   ST, V0
  0x2A6:  22D4  CALL L_2D4
  0x2A8:  8E34  ADD  VE, V3
  0x2AA:  22D4  CALL L_2D4
  0x2AC:  663E  LD   V6, 0x3E
  0x2AE:  3301  SE   V3, 0x01
  0x2B0:  6603  LD   V6, 0x03
  0x2B2:  68FE  LD   V8, 0xFE
  0x2B4:  3301  SE   V3, 0x01
  0x2B6:  6802  LD   V8, 0x02
  0x2B8:  1216  JP   L_216

L_2BA:
  0x2BA:  79FF  ADD  V9, 0xFF
  0x2BC:  49FE  SNE  V9, 0xFE
  0x2BE:  69FF  LD   V9, 0xFF
  0x2C0:  12C8  JP   L_2C8

L_2C2:
  0x2C2:  7901  ADD  V9, 0x01
  0x2C4:  4902  SNE  V9, 0x02
  0x2C6:  6901  LD   V9, 0x01

L_2C8:
  0x2C8:  6004  LD   V0, 0x04
  0x2CA:  F018  LD   ST, V0
  0x2CC:  7601  ADD  V6, 0x01
  0x2CE:  4640  SNE  V6, 0x40
  0x2D0:  76FE  ADD  V6, 0xFE
  0x2D2:  126C  JP   L_26C

L_2D4:
  0x2D4:  A2F2  LD   I, 0x2F2
  0x2D6:  FE33  LD   B, VE
  0x2D8:  F265  LD   V2, [I]
  0x2DA:  F129  LD   F, V1
  0x2DC:  6414  LD   V4, 0x14
  0x2DE:  6500  LD   V5, 0x00
  0x2E0:  D455  DRW  V4, V5, 5
  0x2E2:  7415  ADD  V4, 0x15
  0x2E4:  F229  LD   F, V2
  0x2E6:  D455  DRW  V4, V5, 5
  0x2E8:  00EE  RET
  0x2EA:  8080  LD   V0, V8
  0x2EC:  8080  LD   V0, V8
  0x2EE:  8080  LD   V0, V8
  0x2F0:  8000  LD   V0, V0
  0x2F2:  0000  SYS  0x000
  0x2F4:  0000  SYS  0x000

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