#!/usr/bin/env python3
"""
Lista todas las ROMs disponibles en el directorio de ROMs
con información sobre su tamaño y primeras instrucciones

Uso:
    python list_roms.py [roms_directory]
"""

import sys
import os
from pathlib import Path

def read_first_instructions(rom_path, num_instructions=5):
    """Lee las primeras instrucciones de una ROM"""
    try:
        with open(rom_path, 'rb') as f:
            data = f.read(num_instructions * 2)  # 2 bytes por instrucción
        
        instructions = []
        for i in range(0, len(data)-1, 2):
            opcode = (data[i] << 8) | data[i + 1]
            instructions.append(f"{opcode:04X}")
        
        return " ".join(instructions)
    except Exception:
        return "Error al leer"

def get_rom_info(rom_path):
    """Obtiene información sobre una ROM"""
    size = os.path.getsize(rom_path)
    first_ops = read_first_instructions(rom_path)
    return {
        'name': rom_path.name,
        'size': size,
        'first_ops': first_ops,
        'path': str(rom_path)
    }

def format_size(size):
    """Formatea el tamaño en bytes de manera legible"""
    if size < 1024:
        return f"{size} B"
    elif size < 1024 * 1024:
        return f"{size / 1024:.1f} KB"
    else:
        return f"{size / (1024 * 1024):.1f} MB"

def main():
    # Determinar directorio de ROMs
    if len(sys.argv) > 1:
        roms_dir = Path(sys.argv[1])
    else:
        # Asumir que estamos en el directorio del proyecto
        script_dir = Path(__file__).parent
        roms_dir = script_dir.parent / "roms"
    
    if not roms_dir.exists():
        print(f"Error: El directorio '{roms_dir}' no existe")
        sys.exit(1)
    
    # Buscar archivos .ch8
    rom_files = sorted(roms_dir.glob("*.ch8"))
    
    if not rom_files:
        print(f"No se encontraron archivos .ch8 en '{roms_dir}'")
        sys.exit(1)
    
    # Recopilar información
    roms_info = [get_rom_info(rom) for rom in rom_files]
    
    # Mostrar tabla
    print("=" * 80)
    print("ROMs de CHIP-8 disponibles")
    print("=" * 80)
    print()
    
    # Calcular anchos de columna
    max_name_len = max(len(rom['name']) for rom in roms_info)
    
    # Encabezado
    print(f"{'Nombre':<{max_name_len}}  {'Tamaño':>10}  {'Primeras instrucciones'}")
    print("-" * 80)
    
    # Filas
    for rom in roms_info:
        print(f"{rom['name']:<{max_name_len}}  {format_size(rom['size']):>10}  {rom['first_ops']}")
    
    print()
    print(f"Total: {len(roms_info)} ROM(s)")
    print()
    print("Para desensamblar una ROM:")
    print(f"  python tools/disassembler.py {roms_info[0]['path']}")
    print()

if __name__ == "__main__":
    main()

