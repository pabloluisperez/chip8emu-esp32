; CHIP-8 Disassembly
; Size: 21 bytes
; Start address: 0x200

; Registers: V0-VF (VF = flags)
; I = Index register (16-bit)
; DT = Delay Timer
; ST = Sound Timer
; K = Key input


L_200:
  0x200:  00E0  CLS
  0x202:  610A  LD   V1, 0x0A
  0x204:  6214  LD   V2, 0x14
  0x206:  7105  ADD  V1, 0x05
  0x208:  A210  LD   I, 0x210
  0x20A:  D125  DRW  V1, V2, 5
  0x20C:  1200  JP   L_200
  0x20E:  0000  SYS  0x000
  0x210:  F090  DATA 0xF090
  0x212:  9090  SNE  V0, V9
  0x214:  F0    .BYTE 0xF0

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