; CHIP-8 Disassembly
; Size: 478 bytes
; Start address: 0x200

; Registers: V0-VF (VF = flags)
; I = Index register (16-bit)
; DT = Delay Timer
; ST = Sound Timer
; K = Key input

  0x200:  124E  JP   L_24E
  0x202:  EAAC  DATA 0xEAAC
  0x204:  AAEA  LD   I, 0xAEA
  0x206:  CEAA  RND  VE, 0xAA
  0x208:  AAAE  LD   I, 0xAAE
  0x20A:  E0A0  DATA 0xE0A0
  0x20C:  A0E0  LD   I, 0x0E0
  0x20E:  C040  RND  V0, 0x40
  0x210:  40E0  SNE  V0, 0xE0
  0x212:  E020  DATA 0xE020
  0x214:  C0E0  RND  V0, 0xE0
  0x216:  E060  DATA 0xE060
  0x218:  20E0  CALL L_0E0
  0x21A:  A0E0  LD   I, 0x0E0
  0x21C:  2020  CALL L_020
  0x21E:  6040  LD   V0, 0x40
  0x220:  2040  CALL L_040
  0x222:  E080  DATA 0xE080
  0x224:  E0E0  DATA 0xE0E0
  0x226:  E020  DATA 0xE020
  0x228:  2020  CALL L_020
  0x22A:  E0E0  DATA 0xE0E0
  0x22C:  A0E0  LD   I, 0x0E0
  0x22E:  E0E0  DATA 0xE0E0
  0x230:  20E0  CALL L_0E0
  0x232:  40A0  SNE  V0, 0xA0
  0x234:  E0A0  DATA 0xE0A0
  0x236:  E0C0  DATA 0xE0C0
  0x238:  80E0  LD   V0, VE
  0x23A:  E080  DATA 0xE080
  0x23C:  C080  RND  V0, 0x80
  0x23E:  A040  LD   I, 0x040
  0x240:  A0A0  LD   I, 0x0A0

L_242:
  0x242:  A202  LD   I, 0x202
  0x244:  DAB4  DRW  VA, VB, 4
  0x246:  00EE  RET

L_248:
  0x248:  A202  LD   I, 0x202
  0x24A:  DAB4  DRW  VA, VB, 4
  0x24C:  13DC  JP   L_3DC

L_24E:
  0x24E:  6801  LD   V8, 0x01
  0x250:  6905  LD   V9, 0x05
  0x252:  6A0A  LD   VA, 0x0A
  0x254:  6B01  LD   VB, 0x01
  0x256:  652A  LD   V5, 0x2A
  0x258:  662B  LD   V6, 0x2B
  0x25A:  A216  LD   I, 0x216
  0x25C:  D8B4  DRW  V8, VB, 4
  0x25E:  A23E  LD   I, 0x23E
  0x260:  D9B4  DRW  V9, VB, 4
  0x262:  A202  LD   I, 0x202
  0x264:  362B  SE   V6, 0x2B
  0x266:  A206  LD   I, 0x206
  0x268:  DAB4  DRW  VA, VB, 4
  0x26A:  6B06  LD   VB, 0x06
  0x26C:  A21A  LD   I, 0x21A
  0x26E:  D8B4  DRW  V8, VB, 4
  0x270:  A23E  LD   I, 0x23E
  0x272:  D9B4  DRW  V9, VB, 4
  0x274:  A206  LD   I, 0x206
  0x276:  452A  SNE  V5, 0x2A
  0x278:  A202  LD   I, 0x202
  0x27A:  DAB4  DRW  VA, VB, 4
  0x27C:  6B0B  LD   VB, 0x0B
  0x27E:  A21E  LD   I, 0x21E
  0x280:  D8B4  DRW  V8, VB, 4
  0x282:  A23E  LD   I, 0x23E
  0x284:  D9B4  DRW  V9, VB, 4
  0x286:  A206  LD   I, 0x206
  0x288:  5560  SE   V5, V6
  0x28A:  A202  LD   I, 0x202
  0x28C:  DAB4  DRW  VA, VB, 4
  0x28E:  6B10  LD   VB, 0x10
  0x290:  A226  LD   I, 0x226
  0x292:  D8B4  DRW  V8, VB, 4
  0x294:  A23E  LD   I, 0x23E
  0x296:  D9B4  DRW  V9, VB, 4
  0x298:  A206  LD   I, 0x206
  0x29A:  76FF  ADD  V6, 0xFF
  0x29C:  462A  SNE  V6, 0x2A
  0x29E:  A202  LD   I, 0x202
  0x2A0:  DAB4  DRW  VA, VB, 4
  0x2A2:  6B15  LD   VB, 0x15
  0x2A4:  A22E  LD   I, 0x22E
  0x2A6:  D8B4  DRW  V8, VB, 4
  0x2A8:  A23E  LD   I, 0x23E
  0x2AA:  D9B4  DRW  V9, VB, 4
  0x2AC:  A206  LD   I, 0x206
  0x2AE:  9560  SNE  V5, V6
  0x2B0:  A202  LD   I, 0x202
  0x2B2:  DAB4  DRW  VA, VB, 4
  0x2B4:  6B1A  LD   VB, 0x1A
  0x2B6:  A232  LD   I, 0x232
  0x2B8:  D8B4  DRW  V8, VB, 4
  0x2BA:  A23E  LD   I, 0x23E
  0x2BC:  D9B4  DRW  V9, VB, 4
  0x2BE:  2242  CALL L_242
  0x2C0:  6817  LD   V8, 0x17
  0x2C2:  691B  LD   V9, 0x1B
  0x2C4:  6A20  LD   VA, 0x20
  0x2C6:  6B01  LD   VB, 0x01
  0x2C8:  A20A  LD   I, 0x20A
  0x2CA:  D8B4  DRW  V8, VB, 4
  0x2CC:  A236  LD   I, 0x236
  0x2CE:  D9B4  DRW  V9, VB, 4
  0x2D0:  A202  LD   I, 0x202
  0x2D2:  DAB4  DRW  VA, VB, 4
  0x2D4:  6B06  LD   VB, 0x06
  0x2D6:  A22A  LD   I, 0x22A
  0x2D8:  D8B4  DRW  V8, VB, 4
  0x2DA:  A20A  LD   I, 0x20A
  0x2DC:  D9B4  DRW  V9, VB, 4
  0x2DE:  A206  LD   I, 0x206
  0x2E0:  8750  LD   V7, V5
  0x2E2:  472A  SNE  V7, 0x2A
  0x2E4:  A202  LD   I, 0x202
  0x2E6:  DAB4  DRW  VA, VB, 4
  0x2E8:  6B0B  LD   VB, 0x0B
  0x2EA:  A22A  LD   I, 0x22A
  0x2EC:  D8B4  DRW  V8, VB, 4
  0x2EE:  A20E  LD   I, 0x20E
  0x2F0:  D9B4  DRW  V9, VB, 4
  0x2F2:  A206  LD   I, 0x206
  0x2F4:  672A  LD   V7, 0x2A
  0x2F6:  87B1  OR   V7, VB
  0x2F8:  472B  SNE  V7, 0x2B
  0x2FA:  A202  LD   I, 0x202
  0x2FC:  DAB4  DRW  VA, VB, 4
  0x2FE:  6B10  LD   VB, 0x10
  0x300:  A22A  LD   I, 0x22A
  0x302:  D8B4  DRW  V8, VB, 4
  0x304:  A212  LD   I, 0x212
  0x306:  D9B4  DRW  V9, VB, 4
  0x308:  A206  LD   I, 0x206
  0x30A:  6678  LD   V6, 0x78
  0x30C:  671F  LD   V7, 0x1F
  0x30E:  8762  AND  V7, V6
  0x310:  4718  SNE  V7, 0x18
  0x312:  A202  LD   I, 0x202
  0x314:  DAB4  DRW  VA, VB, 4
  0x316:  6B15  LD   VB, 0x15
  0x318:  A22A  LD   I, 0x22A
  0x31A:  D8B4  DRW  V8, VB, 4
  0x31C:  A216  LD   I, 0x216
  0x31E:  D9B4  DRW  V9, VB, 4
  0x320:  A206  LD   I, 0x206
  0x322:  6678  LD   V6, 0x78
  0x324:  671F  LD   V7, 0x1F
  0x326:  8763  XOR  V7, V6
  0x328:  4767  SNE  V7, 0x67
  0x32A:  A202  LD   I, 0x202
  0x32C:  DAB4  DRW  VA, VB, 4
  0x32E:  6B1A  LD   VB, 0x1A
  0x330:  A22A  LD   I, 0x22A
  0x332:  D8B4  DRW  V8, VB, 4
  0x334:  A21A  LD   I, 0x21A
  0x336:  D9B4  DRW  V9, VB, 4
  0x338:  A206  LD   I, 0x206
  0x33A:  668C  LD   V6, 0x8C
  0x33C:  678C  LD   V7, 0x8C
  0x33E:  8764  ADD  V7, V6
  0x340:  4718  SNE  V7, 0x18
  0x342:  A202  LD   I, 0x202
  0x344:  DAB4  DRW  VA, VB, 4
  0x346:  682C  LD   V8, 0x2C
  0x348:  6930  LD   V9, 0x30
  0x34A:  6A34  LD   VA, 0x34
  0x34C:  6B01  LD   VB, 0x01
  0x34E:  A22A  LD   I, 0x22A
  0x350:  D8B4  DRW  V8, VB, 4
  0x352:  A21E  LD   I, 0x21E
  0x354:  D9B4  DRW  V9, VB, 4
  0x356:  A206  LD   I, 0x206
  0x358:  668C  LD   V6, 0x8C
  0x35A:  6778  LD   V7, 0x78
  0x35C:  8765  SUB  V7, V6
  0x35E:  47EC  SNE  V7, 0xEC
  0x360:  A202  LD   I, 0x202
  0x362:  DAB4  DRW  VA, VB, 4
  0x364:  6B06  LD   VB, 0x06
  0x366:  A22A  LD   I, 0x22A
  0x368:  D8B4  DRW  V8, VB, 4
  0x36A:  A222  LD   I, 0x222
  0x36C:  D9B4  DRW  V9, VB, 4
  0x36E:  A206  LD   I, 0x206
  0x370:  66E0  LD   V6, 0xE0
  0x372:  866E  SHL  V6
  0x374:  46C0  SNE  V6, 0xC0
  0x376:  A202  LD   I, 0x202
  0x378:  DAB4  DRW  VA, VB, 4
  0x37A:  6B0B  LD   VB, 0x0B
  0x37C:  A22A  LD   I, 0x22A
  0x37E:  D8B4  DRW  V8, VB, 4
  0x380:  A236  LD   I, 0x236
  0x382:  D9B4  DRW  V9, VB, 4
  0x384:  A206  LD   I, 0x206
  0x386:  660F  LD   V6, 0x0F
  0x388:  8666  SHR  V6
  0x38A:  4607  SNE  V6, 0x07
  0x38C:  A202  LD   I, 0x202
  0x38E:  DAB4  DRW  VA, VB, 4
  0x390:  6B10  LD   VB, 0x10
  0x392:  A23A  LD   I, 0x23A
  0x394:  D8B4  DRW  V8, VB, 4
  0x396:  A21E  LD   I, 0x21E
  0x398:  D9B4  DRW  V9, VB, 4
  0x39A:  A3E8  LD   I, 0x3E8
  0x39C:  6000  LD   V0, 0x00
  0x39E:  6130  LD   V1, 0x30
  0x3A0:  F155  LD   [I], V1
  0x3A2:  A3E9  LD   I, 0x3E9
  0x3A4:  F065  LD   V0, [I]
  0x3A6:  A206  LD   I, 0x206
  0x3A8:  4030  SNE  V0, 0x30
  0x3AA:  A202  LD   I, 0x202
  0x3AC:  DAB4  DRW  VA, VB, 4
  0x3AE:  6B15  LD   VB, 0x15
  0x3B0:  A23A  LD   I, 0x23A
  0x3B2:  D8B4  DRW  V8, VB, 4
  0x3B4:  A216  LD   I, 0x216
  0x3B6:  D9B4  DRW  V9, VB, 4
  0x3B8:  A3E8  LD   I, 0x3E8
  0x3BA:  6689  LD   V6, 0x89
  0x3BC:  F633  LD   B, V6
  0x3BE:  F265  LD   V2, [I]
  0x3C0:  A202  LD   I, 0x202
  0x3C2:  3001  SE   V0, 0x01
  0x3C4:  A206  LD   I, 0x206
  0x3C6:  3103  SE   V1, 0x03
  0x3C8:  A206  LD   I, 0x206
  0x3CA:  3207  SE   V2, 0x07
  0x3CC:  A206  LD   I, 0x206
  0x3CE:  DAB4  DRW  VA, VB, 4
  0x3D0:  6B1A  LD   VB, 0x1A
  0x3D2:  A20E  LD   I, 0x20E
  0x3D4:  D8B4  DRW  V8, VB, 4
  0x3D6:  A23E  LD   I, 0x23E
  0x3D8:  D9B4  DRW  V9, VB, 4
  0x3DA:  1248  JP   L_248

L_3DC:
  0x3DC:  13DC  JP   L_3DC

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