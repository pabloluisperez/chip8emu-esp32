#!/usr/bin/env python3
"""
Herramienta para convertir archivos ROM de CHIP-8 a arrays de bytes de C++
para embeber en el código de LilyGO S3.

Uso: python rom_to_array.py <archivo_rom>
"""

import sys
import os

def rom_to_cpp_array(filename):
    """Convierte una ROM a un array de C++"""
    try:
        with open(filename, 'rb') as f:
            data = f.read()
        
        basename = os.path.basename(filename)
        name = os.path.splitext(basename)[0].upper().replace('-', '_').replace(' ', '_')
        
        print(f"// ROM: {basename} ({len(data)} bytes)")
        print(f"const uint8_t {name}_ROM[] = {{")
        
        # Imprimir bytes en filas de 12
        for i in range(0, len(data), 12):
            chunk = data[i:i+12]
            hex_values = ', '.join(f'0x{b:02X}' for b in chunk)
            print(f"    {hex_values},")
        
        print("};")
        print(f"\nconst size_t {name}_ROM_SIZE = sizeof({name}_ROM);")
        
        print(f"\n// Para usar en main.cpp:")
        print(f"// chip8.loadROMFromMemory({name}_ROM, {name}_ROM_SIZE);")
        
    except FileNotFoundError:
        print(f"Error: No se encontró el archivo '{filename}'")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

def main():
    if len(sys.argv) != 2:
        print("Uso: python rom_to_array.py <archivo_rom>")
        print("\nEjemplo:")
        print("  python rom_to_array.py roms/pong.ch8")
        sys.exit(1)
    
    rom_file = sys.argv[1]
    rom_to_cpp_array(rom_file)

if __name__ == '__main__':
    main()

