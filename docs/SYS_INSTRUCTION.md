# Sobre la instrucción SYS (0x0NNN) en CHIP-8

## ❓ ¿Qué es SYS?

`SYS 0x0NNN` era una instrucción del CHIP-8 original (años 70) que se ejecutaba en el ordenador RCA 1802 COSMAC. Permitía saltar a rutinas de máquina del sistema operativo en la dirección `0x0NNN`.

## ❌ ¿Por qué no está en tu emulador?

**Es correcto que NO esté implementada** porque:

1. **Obsoleta**: Era específica del hardware RCA 1802 original
2. **No portable**: Las rutinas del sistema no existen en emuladores modernos
3. **Nunca se usa**: Las ROMs modernas de CHIP-8 nunca usan esta instrucción
4. **Especificación moderna**: Solo `0x00E0` (CLS) y `0x00EE` (RET) son relevantes del rango `0x0NNN`

## 🔍 ¿Por qué aparece en el desensamblador?

Cuando ves algo como:

```assembly
0x23A:  00FF  SYS  0x0FF
```

**NO es una instrucción real**, son **datos** (sprites, constantes) que el desensamblador está interpretando como código.

### Ejemplo real: IBM Logo

```assembly
L_228:
  0x228:  1228  JP   L_228              ; ← Bucle infinito, el programa termina aquí
  
; A partir de aquí son DATOS de sprites, no código:
  0x22A:  FF00  .BYTE 0xFF, 0x00  ; 11111111 00000000
  0x23A:  00FF  .BYTE 0x00, 0xFF  ; 00000000 11111111  ← Esto NO es SYS!
```

El byte `0x00FF` forma parte de un sprite para dibujar el logo de IBM, no es código ejecutable.

## ✅ Implementación correcta en tu emulador

Tu código en `Chip8.cpp` líneas 249-268:

```cpp
void Chip8::execute0xxx(uint16_t opcode) {
    switch (opcode & 0x00FF) {
        case 0x00E0: // CLS - Clear screen
            display.clear();
            drawFlag = true;
            pc += 2;
            break;
            
        case 0x00EE: // RET - Return from subroutine
            --sp;
            pc = stack[sp];
            pc += 2;
            break;
            
        default:
            // Ignorar SYS y otros opcodes 0x0NNN no reconocidos
            std::cerr << "Opcode 0x0xxx desconocido: 0x" 
                      << std::hex << opcode << std::endl;
            pc += 2;
            break;
    }
}
```

**Esto es perfecto**. Si tu emulador encuentra un `SYS`:
- Muestra un warning (para debugging)
- Avanza el PC (no se cuelga)
- Continúa ejecutando

En la práctica, **nunca verás este warning** porque:
1. Las ROMs modernas no usan SYS
2. Los datos después de saltos infinitos nunca se ejecutan

## 📊 Instrucciones del rango 0x0NNN

| Opcode | Mnemónico | ¿Soportado? | Descripción |
|--------|-----------|-------------|-------------|
| `0x00E0` | `CLS` | ✅ Sí | Limpiar pantalla |
| `0x00EE` | `RET` | ✅ Sí | Retornar de subrutina |
| `0x0NNN` | `SYS` | ❌ No (obsoleto) | Saltar a rutina del sistema |

## 🎯 Conclusión

**Tu emulador está correcto**. La instrucción `SYS` es obsoleta y no necesitas implementarla. Si aparece en el desensamblador, probablemente son datos que están siendo interpretados como código.

El desensamblador mejorado ahora detecta automáticamente estas secciones y las marca como `DATA SECTION` para evitar confusión.

## 📚 Referencias

- [CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [Cowgod's Chip-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0nnn)
- Especificación original: "BYTE Magazine, Volume 3, Issues 1-12" (1978)

