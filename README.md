# Emulador CHIP-8

Un emulador completo de CHIP-8 escrito en C++ con arquitectura modular que permite cambiar fácilmente entre diferentes implementaciones de hardware (SDL3 para PC, futuro soporte para LilyGO S3 Display).

## Características

- ✅ Implementación completa de los 35 opcodes de CHIP-8
- ✅ Arquitectura modular basada en interfaces abstractas
- ✅ Renderizado mediante SDL3 con ventana 640x320
- ✅ Audio funcional (beep de 440Hz)
- ✅ Sistema de input configurable mediante archivo
- ✅ ROMs de ejemplo incluidas
- ✅ Fácil portabilidad a otras plataformas

## Arquitectura

El proyecto está diseñado con una clara separación entre:

- **Core** (`src/core/`, `include/core/`): Núcleo del emulador independiente de hardware
- **Interfaces** (`include/interfaces/`): Clases abstractas para abstracción de hardware
- **SDL** (`src/sdl/`, `include/sdl/`): Implementación específica para SDL3

Esta arquitectura permite portar el emulador a otras plataformas (como LilyGO S3 Display) simplemente implementando las interfaces para el nuevo hardware sin modificar el core.

## Requisitos

- CMake 3.15 o superior
- Compilador C++17 o superior
- SDL3

### Instalación de SDL3

#### Windows
```bash
# Descargar desde https://github.com/libsdl-org/SDL/releases
# O usar vcpkg:
vcpkg install sdl3
```

#### Linux (Ubuntu/Debian)
```bash
# Compilar desde el código fuente o instalar desde PPA
git clone https://github.com/libsdl-org/SDL
cd SDL
cmake -B build
cmake --build build
sudo cmake --install build
```

#### macOS
```bash
brew install sdl3
```

## Compilación

### Para SDL3 (PC/Mac/Linux)

```bash
# Crear directorio de build
mkdir build
cd build

# Configurar con CMake
cmake ..

# Compilar
cmake --build .
```

En Windows con Visual Studio:
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Para LilyGO S3 Display

El proyecto incluye soporte completo para LilyGO T-Display S3 mediante PlatformIO:

```bash
# Instalar PlatformIO si no lo tienes
pip install platformio

# Compilar para LilyGO
pio run

# Subir a la placa
pio run --target upload

# Monitorear serial
pio device monitor
```

**Nota:** La ROM IBM Logo está embebida en el código para LilyGO. Para usar otras ROMs, edita el array `ROM_DATA` en `src/main.cpp`.

## Uso

### SDL3 (PC)
```bash
./chip8emu <archivo_rom>
```

Ejemplos:
```bash
# Ejecutar ROM de test IBM Logo
./chip8emu roms/IBM_Logo.ch8

# Ejecutar Pong
./chip8emu roms/pong.ch8

# Ejecutar test de opcodes
./chip8emu roms/test_opcode.ch8
```

### LilyGO S3 Display

Simplemente sube el firmware a la placa. La ROM IBM Logo está embebida por defecto. Para usar otras ROMs:

1. Convierte la ROM a array de bytes
2. Reemplaza el array `ROM_DATA` en `src/main.cpp`
3. Recompila y sube

## Controles

El mapeo de teclas por defecto es configurable en `config/keymap.cfg`:

```
Layout CHIP-8:     Layout Teclado:
  1 2 3 C            1 2 3 4
  4 5 6 D            Q W E R
  7 8 9 E            A S D F
  A 0 B F            Z X C V
```

- **ESC**: Salir del emulador

## Configuración

### Mapeo de Teclas

Edita `config/keymap.cfg` para cambiar el mapeo de teclas:

```ini
# Formato: TECLA_CHIP8=TECLA_SDL
0=X
1=1
2=2
...
F=V
```

## ROMs Incluidas

- **IBM_Logo.ch8**: ROM de test clásica que muestra el logo de IBM
- **test_opcode.ch8**: ROM simple para probar funcionalidad básica
- **pong.ch8**: Juego clásico de Pong para dos jugadores

## Arquitectura Multi-Plataforma

El emulador ya incluye implementaciones completas para:

1. **SDL3** (PC/Mac/Linux) - En `src/sdl/` e `include/sdl/`
2. **LilyGO S3 Display** - En `src/lilygo/` e `include/lilygo/`

El sistema de compilación selecciona automáticamente la plataforma:
- **CMake** → Compila versión SDL3
- **PlatformIO** → Compila versión LilyGO

### Configuración de Hardware LilyGO

En `src/lilygo/LilyGoInput.cpp` puedes configurar los pines GPIO para los botones:

```cpp
void LilyGoInput::setupGPIO() {
    gpioMapping[0x0] = 0;   // GPIO 0 (BOOT button)
    gpioMapping[0x1] = 14;  // GPIO 14 
    // ... más configuraciones
}
```

En `src/main.cpp` puedes cambiar el pin del buzzer:
```cpp
static LilyGoAudio audio(25);  // GPIO 25 para buzzer
```

### Portar a Otra Plataforma

La arquitectura hace muy fácil añadir nuevas plataformas. Solo necesitas:

1. Crear implementaciones de 4 interfaces (IDisplay, IInput, IAudio, IPlatform)
2. Añadir una sección `#ifdef` en `main.cpp`
3. El core del emulador funciona sin cambios

Ver `PORTING_GUIDE.md` para más detalles.

## Especificación CHIP-8

CHIP-8 es un lenguaje de programación interpretado desarrollado en los años 70:

- **Memoria**: 4KB (0x000-0xFFF)
- **Display**: 64x32 pixels monocromático
- **Registros**: 16 registros de 8 bits (V0-VF)
- **Stack**: 16 niveles
- **Timers**: Delay timer y sound timer (60 Hz)
- **Input**: Keypad hexadecimal de 16 teclas (0-F)
- **Opcodes**: 35 instrucciones

## Referencias

- [CHIP-8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [CHIP-8 Test Suite](https://github.com/Timendus/chip8-test-suite)

## Licencia

Este proyecto es de código abierto y está disponible bajo la licencia MIT.

## Autor

Creado como proyecto educativo de emulación de sistemas.

