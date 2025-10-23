#!/usr/bin/env python3
"""
Desensamblador de CHIP-8
Convierte ROMs binarias a código ensamblador legible

Uso:
    python disassembler.py <rom_file> [output_file]
    
Si no se especifica output_file, se muestra por pantalla.
"""

import sys
import os
from typing import Dict, List, Tuple, Optional

class Chip8Disassembler:
    def __init__(self):
        self.labels: Dict[int, str] = {}  # Direcciones de salto -> etiquetas
        self.label_counter = 0
        
    def decode_instruction(self, opcode: int, addr: int) -> Tuple[str, Optional[int]]:
        """
        Decodifica una instrucción de 2 bytes y retorna (mnemónico, target_address)
        target_address es la dirección de salto si aplica, None en otro caso
        """
        # Extraer nibbles
        first = (opcode & 0xF000) >> 12
        x = (opcode & 0x0F00) >> 8
        y = (opcode & 0x00F0) >> 4
        n = opcode & 0x000F
        nn = opcode & 0x00FF
        nnn = opcode & 0x0FFF
        
        target = None  # Dirección de salto/llamada
        
        if opcode == 0x00E0:
            return "CLS", None
        elif opcode == 0x00EE:
            return "RET", None
        elif first == 0x0:
            return f"SYS  0x{nnn:03X}", None
        elif first == 0x1:
            target = nnn
            return f"JP   0x{nnn:03X}", target
        elif first == 0x2:
            target = nnn
            return f"CALL 0x{nnn:03X}", target
        elif first == 0x3:
            return f"SE   V{x:X}, 0x{nn:02X}", None
        elif first == 0x4:
            return f"SNE  V{x:X}, 0x{nn:02X}", None
        elif first == 0x5 and n == 0:
            return f"SE   V{x:X}, V{y:X}", None
        elif first == 0x6:
            return f"LD   V{x:X}, 0x{nn:02X}", None
        elif first == 0x7:
            return f"ADD  V{x:X}, 0x{nn:02X}", None
        elif first == 0x8:
            if n == 0x0:
                return f"LD   V{x:X}, V{y:X}", None
            elif n == 0x1:
                return f"OR   V{x:X}, V{y:X}", None
            elif n == 0x2:
                return f"AND  V{x:X}, V{y:X}", None
            elif n == 0x3:
                return f"XOR  V{x:X}, V{y:X}", None
            elif n == 0x4:
                return f"ADD  V{x:X}, V{y:X}", None
            elif n == 0x5:
                return f"SUB  V{x:X}, V{y:X}", None
            elif n == 0x6:
                return f"SHR  V{x:X}", None
            elif n == 0x7:
                return f"SUBN V{x:X}, V{y:X}", None
            elif n == 0xE:
                return f"SHL  V{x:X}", None
            else:
                return f"DATA 0x{opcode:04X}", None
        elif first == 0x9 and n == 0:
            return f"SNE  V{x:X}, V{y:X}", None
        elif first == 0xA:
            return f"LD   I, 0x{nnn:03X}", None
        elif first == 0xB:
            return f"JP   V0, 0x{nnn:03X}", None
        elif first == 0xC:
            return f"RND  V{x:X}, 0x{nn:02X}", None
        elif first == 0xD:
            return f"DRW  V{x:X}, V{y:X}, {n}", None
        elif first == 0xE:
            if nn == 0x9E:
                return f"SKP  V{x:X}", None
            elif nn == 0xA1:
                return f"SKNP V{x:X}", None
            else:
                return f"DATA 0x{opcode:04X}", None
        elif first == 0xF:
            if nn == 0x07:
                return f"LD   V{x:X}, DT", None
            elif nn == 0x0A:
                return f"LD   V{x:X}, K", None
            elif nn == 0x15:
                return f"LD   DT, V{x:X}", None
            elif nn == 0x18:
                return f"LD   ST, V{x:X}", None
            elif nn == 0x1E:
                return f"ADD  I, V{x:X}", None
            elif nn == 0x29:
                return f"LD   F, V{x:X}", None
            elif nn == 0x33:
                return f"LD   B, V{x:X}", None
            elif nn == 0x55:
                return f"LD   [I], V{x:X}", None
            elif nn == 0x65:
                return f"LD   V{x:X}, [I]", None
            else:
                return f"DATA 0x{opcode:04X}", None
        else:
            return f"DATA 0x{opcode:04X}", None
    
    def first_pass(self, data: bytes, start_addr: int = 0x200) -> None:
        """
        Primera pasada: identifica direcciones de salto para crear etiquetas
        """
        addr = start_addr
        i = 0
        
        while i < len(data) - 1:
            opcode = (data[i] << 8) | data[i + 1]
            mnemonic, target = self.decode_instruction(opcode, addr)
            
            # Si hay un salto o llamada, crear etiqueta
            if target is not None and target not in self.labels:
                self.labels[target] = f"L_{target:03X}"
            
            addr += 2
            i += 2
    
    def disassemble(self, data: bytes, start_addr: int = 0x200) -> List[str]:
        """
        Desensambla la ROM y retorna líneas de código ensamblador
        """
        # Primera pasada para identificar etiquetas
        self.first_pass(data, start_addr)
        
        lines = []
        lines.append("; CHIP-8 Disassembly")
        lines.append(f"; Size: {len(data)} bytes")
        lines.append(f"; Start address: 0x{start_addr:03X}")
        lines.append("")
        lines.append("; Registers: V0-VF (VF = flags)")
        lines.append("; I = Index register (16-bit)")
        lines.append("; DT = Delay Timer")
        lines.append("; ST = Sound Timer")
        lines.append("; K = Key input")
        lines.append("")
        
        addr = start_addr
        i = 0
        
        while i < len(data) - 1:
            opcode = (data[i] << 8) | data[i + 1]
            mnemonic, target = self.decode_instruction(opcode, addr)
            
            # Si esta dirección tiene una etiqueta, mostrarla
            if addr in self.labels:
                lines.append("")
                lines.append(f"{self.labels[addr]}:")
            
            # Si el mnemónico hace referencia a una etiqueta, reemplazar dirección
            if target is not None and target in self.labels:
                mnemonic = mnemonic.replace(f"0x{target:03X}", self.labels[target])
            
            # Formatear línea: dirección | bytes | mnemónico
            line = f"  0x{addr:03X}:  {opcode:04X}  {mnemonic}"
            lines.append(line)
            
            addr += 2
            i += 2
        
        # Si hay un byte impar al final
        if len(data) % 2 == 1:
            lines.append(f"  0x{addr:03X}:  {data[-1]:02X}    DATA 0x{data[-1]:02X}")
        
        return lines
    
    def disassemble_with_comments(self, data: bytes, start_addr: int = 0x200) -> List[str]:
        """
        Desensambla con comentarios adicionales sobre las instrucciones
        """
        lines = self.disassemble(data, start_addr)
        
        # Agregar leyenda al final
        lines.append("")
        lines.append("; ===== Instruction Reference =====")
        lines.append("; CLS             - Clear screen")
        lines.append("; RET             - Return from subroutine")
        lines.append("; JP addr         - Jump to address")
        lines.append("; CALL addr       - Call subroutine at address")
        lines.append("; SE Vx, byte     - Skip next if Vx == byte")
        lines.append("; SNE Vx, byte    - Skip next if Vx != byte")
        lines.append("; LD Vx, byte     - Load byte into Vx")
        lines.append("; ADD Vx, byte    - Add byte to Vx")
        lines.append("; OR Vx, Vy       - Vx = Vx OR Vy")
        lines.append("; AND Vx, Vy      - Vx = Vx AND Vy")
        lines.append("; XOR Vx, Vy      - Vx = Vx XOR Vy")
        lines.append("; SUB Vx, Vy      - Vx = Vx - Vy (VF = borrow)")
        lines.append("; SHR Vx          - Shift Vx right by 1")
        lines.append("; SHL Vx          - Shift Vx left by 1")
        lines.append("; RND Vx, byte    - Vx = random AND byte")
        lines.append("; DRW Vx, Vy, n   - Draw sprite at (Vx,Vy), height n")
        lines.append("; SKP Vx          - Skip if key Vx is pressed")
        lines.append("; SKNP Vx         - Skip if key Vx is not pressed")
        lines.append("; LD I, addr      - Load address into I")
        lines.append("; LD Vx, DT       - Load delay timer into Vx")
        lines.append("; LD DT, Vx       - Load Vx into delay timer")
        lines.append("; LD ST, Vx       - Load Vx into sound timer")
        lines.append("; LD F, Vx        - Load sprite address for digit Vx into I")
        lines.append("; LD B, Vx        - Store BCD of Vx at I, I+1, I+2")
        lines.append("; LD [I], Vx      - Store V0-Vx at memory[I]")
        lines.append("; LD Vx, [I]      - Load V0-Vx from memory[I]")
        
        return lines


def main():
    if len(sys.argv) < 2:
        print("Uso: python disassembler.py <rom_file> [output_file]")
        print("Ejemplo: python disassembler.py ../roms/pong.ch8 pong.asm")
        sys.exit(1)
    
    rom_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else None
    
    if not os.path.exists(rom_file):
        print(f"Error: El archivo '{rom_file}' no existe")
        sys.exit(1)
    
    # Leer ROM
    try:
        with open(rom_file, 'rb') as f:
            data = f.read()
    except Exception as e:
        print(f"Error al leer el archivo: {e}")
        sys.exit(1)
    
    # Desensamblar
    disasm = Chip8Disassembler()
    lines = disasm.disassemble_with_comments(data)
    
    # Salida
    if output_file:
        try:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write('\n'.join(lines))
            print(f"Desensamblado guardado en: {output_file}")
            print(f"Total de líneas: {len(lines)}")
        except Exception as e:
            print(f"Error al escribir el archivo: {e}")
            sys.exit(1)
    else:
        for line in lines:
            print(line)


if __name__ == "__main__":
    main()

