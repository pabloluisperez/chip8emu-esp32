# Herramientas CHIP-8

Este directorio contiene utilidades para trabajar con ROMs de CHIP-8.

## 🛠️ Herramientas disponibles

### 1. `list_roms.py` - Listar ROMs disponibles

Lista todas las ROMs en el directorio con información útil.

#### Uso:

```bash
# Listar ROMs en el directorio por defecto (roms/)
python tools/list_roms.py

# Listar ROMs en un directorio específico
python tools/list_roms.py /ruta/a/mis/roms
```

#### Salida de ejemplo:

```
================================================================================
ROMs de CHIP-8 disponibles
================================================================================

Nombre               Tamaño  Primeras instrucciones
--------------------------------------------------------------------------------
IBM_Logo.ch8          132 B  00E0 A22A 600C 6108 D01F
pong.ch8              246 B  6A02 6B0C 6C3F 6D0C A2EA
test_opcode.ch8        21 B  00E0 610A 6214 7105 A210

Total: 3 ROM(s)
```

---

### 2. `disassembler.py` - Desensamblador de CHIP-8

Convierte archivos ROM binarios a código ensamblador legible.

#### Uso:

```bash
# Mostrar en pantalla
python tools/disassembler.py roms/pong.ch8

# Guardar en archivo
python tools/disassembler.py roms/pong.ch8 pong.asm
```

#### Características:

- ✅ Decodifica todas las instrucciones de CHIP-8
- ✅ Genera etiquetas automáticas para saltos y llamadas
- ✅ Muestra direcciones de memoria y bytes hexadecimales
- ✅ Incluye referencia completa de instrucciones
- ✅ Formato limpio y legible

#### Ejemplo de salida:

```assembly
; CHIP-8 Disassembly
; Size: 246 bytes
; Start address: 0x200

  0x200:  6A02  LD   VA, 0x02
  0x202:  6B0C  LD   VB, 0x0C
  0x204:  A2EA  LD   I, 0x2EA
  0x206:  DAB6  DRW  VA, VB, 6

L_216:
  0x216:  6060  LD   V0, 0x60
  0x218:  F015  LD   DT, V0
  0x21A:  121A  JP   L_21A
```

---

### 3. `rom_to_array.py` - Convertir ROM a array C/C++

Convierte una ROM a un array constante de C/C++ para embeber en firmware.

#### Uso:

```bash
python tools/rom_to_array.py roms/pong.ch8 > pong_rom.h
```

#### Ejemplo de salida:

```cpp
const uint8_t rom_data[] = {
    0x6A, 0x02, 0x6B, 0x0C, 0x6C, 0x3F, 0x6D, 0x0C,
    // ...
};
const size_t rom_size = 246;
```

---

## 📚 Referencia de instrucciones CHIP-8

### Mnemónicos:

| Instrucción | Descripción |
|-------------|-------------|
| `CLS` | Limpiar pantalla |
| `RET` | Retornar de subrutina |
| `JP addr` | Saltar a dirección |
| `CALL addr` | Llamar subrutina |
| `SE Vx, byte` | Saltar si Vx == byte |
| `SNE Vx, byte` | Saltar si Vx != byte |
| `LD Vx, byte` | Cargar byte en Vx |
| `ADD Vx, byte` | Sumar byte a Vx |
| `OR Vx, Vy` | Vx = Vx OR Vy |
| `AND Vx, Vy` | Vx = Vx AND Vy |
| `XOR Vx, Vy` | Vx = Vx XOR Vy |
| `SUB Vx, Vy` | Vx = Vx - Vy |
| `SHR Vx` | Desplazar Vx a la derecha |
| `SHL Vx` | Desplazar Vx a la izquierda |
| `RND Vx, byte` | Vx = random AND byte |
| `DRW Vx, Vy, n` | Dibujar sprite en (Vx, Vy) |
| `SKP Vx` | Saltar si tecla Vx presionada |
| `SKNP Vx` | Saltar si tecla Vx NO presionada |
| `LD I, addr` | Cargar dirección en I |
| `LD Vx, DT` | Cargar delay timer en Vx |
| `LD DT, Vx` | Cargar Vx en delay timer |
| `LD ST, Vx` | Cargar Vx en sound timer |
| `LD F, Vx` | I = dirección sprite del dígito Vx |
| `LD B, Vx` | Guardar BCD de Vx en [I] |
| `LD [I], Vx` | Guardar V0-Vx en memoria[I] |
| `LD Vx, [I]` | Cargar V0-Vx desde memoria[I] |

### Registros:

- **V0-VF**: Registros de propósito general (8-bit)
  - VF se usa como flag en algunas operaciones
- **I**: Registro índice (16-bit)
- **DT**: Delay Timer (8-bit, decrementa a 60 Hz)
- **ST**: Sound Timer (8-bit, decrementa a 60 Hz, suena mientras > 0)
- **PC**: Program Counter (16-bit)
- **SP**: Stack Pointer (8-bit)

---

## 💡 Casos de uso

### Debugging
Desensamble una ROM para entender su funcionamiento:
```bash
python tools/disassembler.py roms/test_opcode.ch8 > test.asm
```

### Análisis de juegos
Estudia cómo funcionan juegos clásicos:
```bash
python tools/disassembler.py roms/pong.ch8 > analysis/pong.asm
```

### Desarrollo de ROMs embebidas
Convierte ROMs para incluir en firmware ESP32:
```bash
python tools/rom_to_array.py roms/tetris.ch8 > include/lilygo/tetris_rom.h
```

---

## 🔧 Requisitos

- Python 3.6 o superior
- No requiere dependencias externas

## 📝 Notas

- Las ROMs de CHIP-8 comienzan en la dirección `0x200`
- Los primeros 512 bytes (`0x000-0x1FF`) están reservados para el intérprete
- Las instrucciones tienen 2 bytes de longitud
- El formato de archivo es binario puro (sin headers)

