# Arquitectura del Emulador CHIP-8

Este documento explica la arquitectura modular del emulador.

## Diagrama de Arquitectura

```
┌─────────────────────────────────────────────────────────────┐
│                    APLICACIÓN PRINCIPAL                      │
│                        (main.cpp)                            │
│                                                              │
│   #ifdef PLATFORM_LILYGO        |        #else (SDL3)       │
│   ┌──────────────────┐          |     ┌──────────────────┐  │
│   │ LilyGO Platform  │          |     │  SDL3 Platform   │  │
│   └──────────────────┘          |     └──────────────────┘  │
└────────────────┬────────────────┴───────────┬───────────────┘
                 │                            │
                 ▼                            ▼
        ┌─────────────────────────────────────────────┐
        │          INTERFACES ABSTRACTAS              │
        │  ┌──────────┬──────────┬─────────────────┐ │
        │  │ IDisplay │  IInput  │  IAudio │ IPlatform│
        │  └──────────┴──────────┴─────────┴────────┘ │
        └─────────────────┬───────────────────────────┘
                          │
                          ▼
              ┌────────────────────────┐
              │    CORE DEL EMULADOR   │
              │      (Chip8.cpp)       │
              │                        │
              │  - Memoria (4KB)       │
              │  - Registros (V0-VF)   │
              │  - Stack (16 niveles)  │
              │  - Timers (60Hz)       │
              │  - 35 Opcodes          │
              └────────────────────────┘
```

## Capas de la Arquitectura

### 1. Core del Emulador (Independiente de Plataforma)

**Ubicación:** `src/core/` e `include/core/`

El core implementa la especificación completa de CHIP-8:

```cpp
class Chip8 {
    // Estado del emulador
    std::array<uint8_t, 4096> memory;  // 4KB RAM
    std::array<uint8_t, 16> V;         // Registros V0-VF
    uint16_t I;                         // Registro índice
    uint16_t pc;                        // Program counter
    std::array<uint16_t, 16> stack;    // Stack
    uint8_t sp;                         // Stack pointer
    uint8_t delayTimer;                 // Delay timer (60Hz)
    uint8_t soundTimer;                 // Sound timer (60Hz)
    
    // Métodos públicos
    void emulateCycle();                // Ejecuta un ciclo
    void updateTimers();                // Actualiza timers
    bool loadROM(const std::string&);   // Carga ROM desde archivo
    bool loadROMFromMemory(...);        // Carga ROM desde memoria
};
```

**Características clave:**
- ✅ No depende de ninguna plataforma específica
- ✅ Recibe interfaces como parámetros del constructor
- ✅ El mismo código funciona en SDL3 y LilyGO
- ✅ Fácil de testear de forma aislada

### 2. Interfaces Abstractas (Capa de Abstracción)

**Ubicación:** `include/interfaces/`

Definen contratos que todas las plataformas deben cumplir:

```cpp
// IDisplay.h - Abstracción del display
class IDisplay {
    virtual void clear() = 0;
    virtual void setPixel(int x, int y, bool on) = 0;
    virtual void render() = 0;
    virtual bool getPixel(int x, int y) const = 0;
};

// IInput.h - Abstracción del input
class IInput {
    virtual bool isKeyPressed(uint8_t key) = 0;
    virtual uint8_t waitForKeyPress() = 0;
    virtual void update() = 0;
};

// IAudio.h - Abstracción del audio
class IAudio {
    virtual void playBeep() = 0;
    virtual void stopBeep() = 0;
};

// IPlatform.h - Abstracción de la plataforma
class IPlatform {
    virtual bool init() = 0;
    virtual void shutdown() = 0;
    virtual bool shouldQuit() = 0;
    virtual void delay(uint32_t ms) = 0;
    virtual uint32_t getTicks() = 0;
};
```

**Beneficios:**
- ✅ Desacopla el core de la implementación
- ✅ Permite múltiples implementaciones
- ✅ Fácil testing con mocks
- ✅ Código limpio y mantenible

### 3. Implementación SDL3 (Para PC/Mac/Linux)

**Ubicación:** `src/sdl/` e `include/sdl/`

Implementa las interfaces usando SDL3:

```cpp
// SDLDisplay - Ventana y renderizado
class SDLDisplay : public IDisplay {
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::array<std::array<bool, 64>, 32> pixels;
    // Ventana 640x320 (escala 10x)
};

// SDLInput - Teclado
class SDLInput : public IInput {
    std::array<bool, 16> keys;
    std::map<SDL_Scancode, uint8_t> keymapping;
    // Lee de config/keymap.cfg
};

// SDLAudio - Sistema de audio
class SDLAudio : public IAudio {
    SDL_AudioStream* audioStream;
    // Genera beep de 440Hz
};

// SDLPlatform - Inicialización y control
class SDLPlatform : public IPlatform {
    // Maneja ventana, eventos, timing
};
```

**Compilación:** CMake

### 4. Implementación LilyGO (Para ESP32-S3)

**Ubicación:** `src/lilygo/` e `include/lilygo/`

Implementa las interfaces para hardware embebido:

```cpp
// LilyGoDisplay - Display TFT
class LilyGoDisplay : public IDisplay {
    TFT_eSPI tft;
    std::array<std::array<bool, 64>, 32> pixels;
    // Display 170x320 (escala 4x)
};

// LilyGoInput - Botones GPIO
class LilyGoInput : public IInput {
    std::array<bool, 16> keys;
    std::array<int, 16> gpioMapping;
    // Lee botones físicos
};

// LilyGoAudio - Buzzer PWM
class LilyGoAudio : public IAudio {
    int buzzerPin;
    // Genera tono con PWM
};

// LilyGoPlatform - Control ESP32
class LilyGoPlatform : public IPlatform {
    // Inicialización, Serial, timing
};
```

**Compilación:** PlatformIO

## Flujo de Ejecución

### Inicialización

```
1. main.cpp detecta plataforma (#ifdef PLATFORM_LILYGO)
   ↓
2. Crea objetos de plataforma (SDL o LilyGO)
   ↓
3. Inicializa plataforma (display, audio, etc.)
   ↓
4. Crea Chip8 pasando interfaces
   ↓
5. Carga ROM (desde archivo o memoria)
   ↓
6. Entra al ciclo principal
```

### Ciclo Principal (60 FPS)

```
Loop infinito:
  1. Procesar eventos/input
     ↓
  2. Ejecutar N ciclos de CPU (~600 Hz)
     ├─ Fetch opcode
     ├─ Decode opcode
     └─ Execute opcode
     ↓
  3. Actualizar timers (cada 16ms)
     ├─ Decrementar delay timer
     └─ Decrementar sound timer → playBeep()
     ↓
  4. Renderizar (si drawFlag)
     └─ display.render()
     ↓
  5. Controlar velocidad (delay para 60 FPS)
```

### Ejecución de Opcode (Ejemplo: Draw)

```
DXYN - Draw sprite en (VX, VY) de altura N

1. Chip8::emulateCycle()
   ↓
2. Fetch: opcode = memory[pc] << 8 | memory[pc+1]
   ↓
3. Decode: case 0xD000
   ↓
4. Execute:
   - Para cada fila del sprite:
     - Leer byte del sprite desde memory[I+row]
     - Para cada pixel del byte:
       - Obtener estado actual: display.getPixel(x, y)
       - XOR con nuevo pixel
       - Actualizar: display.setPixel(x, y, newState)
       - Si hubo colisión: V[0xF] = 1
   ↓
5. Set drawFlag = true
   ↓
6. En el loop principal: display.render()
   ↓
7. SDL: Dibuja rectángulos en ventana
   LilyGO: Dibuja pixeles en TFT
```

## Ventajas de esta Arquitectura

### 1. Separación de Responsabilidades

- **Core**: Solo lógica de emulación
- **Interfaces**: Solo contratos
- **Implementaciones**: Solo detalles de plataforma

### 2. Fácil Testing

```cpp
// Mock para testing
class MockDisplay : public IDisplay {
    void setPixel(int x, int y, bool on) override {
        pixels[y][x] = on;
        // Log para test
    }
};

// Test
Chip8 chip8(mockDisplay, mockInput, mockAudio);
chip8.loadROMFromMemory(testROM, size);
chip8.emulateCycle();
// Assert en mockDisplay.pixels
```

### 3. Portabilidad

Para añadir una nueva plataforma (ej: Android):

```cpp
// 1. Implementar interfaces
class AndroidDisplay : public IDisplay { ... }
class AndroidInput : public IInput { ... }
class AndroidAudio : public IAudio { ... }
class AndroidPlatform : public IPlatform { ... }

// 2. Añadir a main.cpp
#ifdef PLATFORM_ANDROID
    AndroidDisplay display;
    AndroidInput input;
    // ...
#endif

// 3. El core funciona sin cambios
Chip8 chip8(display, input, audio);
```

### 4. Mantenibilidad

- Bugs en el core se corrigen una sola vez
- Mejoras en el core benefician a todas las plataformas
- Cada plataforma puede optimizarse independientemente
- Código limpio y fácil de entender

## Patrón de Diseño

Esta arquitectura usa varios patrones:

### Dependency Injection

```cpp
// El Chip8 recibe dependencias (no las crea)
Chip8::Chip8(IDisplay& display, IInput& input, IAudio& audio)
    : display(display), input(input), audio(audio) {
}

// Ventajas:
// - Fácil testing
// - Bajo acoplamiento
// - Flexible
```

### Strategy Pattern

```cpp
// Diferentes estrategias para display
IDisplay* display;

#ifdef PLATFORM_LILYGO
    display = new LilyGoDisplay();
#else
    display = new SDLDisplay();
#endif

// El core usa la estrategia sin saber cuál es
chip8.setPixel(x, y, true);  // → display.setPixel()
```

### Template Method (implícito)

```cpp
// El ciclo principal es el mismo
while (!platform.shouldQuit()) {
    processInput();    // Implementación varía
    emulateCycle();    // Igual para todos
    render();          // Implementación varía
}
```

## Métricas del Código

```
Core del Emulador:
  - Chip8.cpp: ~450 líneas
  - Config.cpp: ~80 líneas
  - Total Core: ~530 líneas

Interfaces:
  - 4 interfaces: ~60 líneas

Implementación SDL3:
  - 4 archivos .cpp: ~350 líneas
  - 4 archivos .h: ~100 líneas
  - Total SDL: ~450 líneas

Implementación LilyGO:
  - 4 archivos .cpp: ~250 líneas
  - 4 archivos .h: ~100 líneas
  - Total LilyGO: ~350 líneas

Main:
  - main.cpp: ~200 líneas

Total del Proyecto: ~1,640 líneas
```

## Diagrama de Dependencias

```
main.cpp
  ├─ depends on → SDL3 implementations ─┐
  ├─ depends on → LilyGO implementations ┤
  └─ depends on → Chip8 core ───────────┤
                                         │
Chip8 core                               │
  └─ depends on → Interfaces ←───────────┴─ All implementations
                                             implement these

SDL3 implementations
  ├─ implements → Interfaces
  └─ depends on → SDL3 library

LilyGO implementations
  ├─ implements → Interfaces
  └─ depends on → Arduino/ESP32 libraries
```

## Conclusión

Esta arquitectura hace que:
1. El core sea **portable** y **testeable**
2. Las plataformas sean **independientes**
3. El código sea **mantenible** y **extensible**
4. Añadir plataformas sea **simple**

Es un excelente ejemplo de **diseño orientado a interfaces** y **programación modular**. 🎯

