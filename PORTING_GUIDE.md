# Guía de Portabilidad a LilyGO S3 Display

Este documento explica cómo portar el emulador CHIP-8 a la plataforma LilyGO S3 Display.

## Arquitectura del Proyecto

El emulador está diseñado con una clara separación de responsabilidades:

```
Core (Chip8.cpp)
    ↓ usa interfaces
Interfaces Abstractas (IDisplay, IInput, IAudio, IPlatform)
    ↓ implementadas por
Implementación SDL3 ← → Implementación LilyGO S3
```

## Interfaces a Implementar

### 1. IDisplay (Display del dispositivo)

Para LilyGO S3, necesitarás implementar la interfaz usando la biblioteca del display TFT:

```cpp
// include/lilygo/LilyGoDisplay.h
#ifndef LILYGODISPLAY_H
#define LILYGODISPLAY_H

#include "../interfaces/IDisplay.h"
#include <TFT_eSPI.h>  // Biblioteca del LilyGO S3

class LilyGoDisplay : public IDisplay {
public:
    LilyGoDisplay(int scale = 4);  // Display S3 es más grande
    ~LilyGoDisplay() override;
    
    bool init();
    void shutdown();
    
    void clear() override;
    void setPixel(int x, int y, bool on) override;
    void render() override;
    bool getPixel(int x, int y) const override;
    
private:
    static const int WIDTH = 64;
    static const int HEIGHT = 32;
    
    TFT_eSPI tft;
    int scale;
    std::array<std::array<bool, WIDTH>, HEIGHT> pixels;
};

#endif
```

```cpp
// src/lilygo/LilyGoDisplay.cpp
#include "../../include/lilygo/LilyGoDisplay.h"

LilyGoDisplay::LilyGoDisplay(int scale) : scale(scale) {
    for (auto& row : pixels) {
        row.fill(false);
    }
}

bool LilyGoDisplay::init() {
    tft.init();
    tft.setRotation(1);  // Ajustar según orientación deseada
    tft.fillScreen(TFT_BLACK);
    return true;
}

void LilyGoDisplay::clear() {
    for (auto& row : pixels) {
        row.fill(false);
    }
}

void LilyGoDisplay::setPixel(int x, int y, bool on) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        pixels[y][x] = on;
    }
}

bool LilyGoDisplay::getPixel(int x, int y) const {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        return pixels[y][x];
    }
    return false;
}

void LilyGoDisplay::render() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            uint16_t color = pixels[y][x] ? TFT_WHITE : TFT_BLACK;
            tft.fillRect(x * scale, y * scale, scale, scale, color);
        }
    }
}
```

### 2. IInput (Botones del dispositivo)

```cpp
// include/lilygo/LilyGoInput.h
#ifndef LILYGOINPUT_H
#define LILYGOINPUT_H

#include "../interfaces/IInput.h"
#include <array>

class LilyGoInput : public IInput {
public:
    LilyGoInput();
    ~LilyGoInput() override = default;
    
    bool isKeyPressed(uint8_t key) override;
    uint8_t waitForKeyPress() override;
    void update() override;
    
private:
    std::array<bool, 16> keys;
    std::array<int, 16> gpioMapping;  // Mapeo de teclas CHIP-8 a GPIO
    
    void setupGPIO();
};

#endif
```

```cpp
// src/lilygo/LilyGoInput.cpp
#include "../../include/lilygo/LilyGoInput.h"
#include <Arduino.h>

LilyGoInput::LilyGoInput() {
    keys.fill(false);
    setupGPIO();
}

void LilyGoInput::setupGPIO() {
    // Configurar los pines GPIO para los botones
    // Ejemplo: si tienes botones en ciertos pines
    gpioMapping[0x1] = 12;  // GPIO 12 para tecla 1
    gpioMapping[0x2] = 13;  // GPIO 13 para tecla 2
    // ... configurar el resto
    
    for (int pin : gpioMapping) {
        if (pin > 0) {
            pinMode(pin, INPUT_PULLUP);
        }
    }
}

void LilyGoInput::update() {
    for (size_t i = 0; i < 16; ++i) {
        if (gpioMapping[i] > 0) {
            keys[i] = (digitalRead(gpioMapping[i]) == LOW);
        }
    }
}

bool LilyGoInput::isKeyPressed(uint8_t key) {
    if (key < 16) {
        return keys[key];
    }
    return false;
}

uint8_t LilyGoInput::waitForKeyPress() {
    while (true) {
        update();
        for (uint8_t i = 0; i < 16; ++i) {
            if (keys[i]) {
                return i;
            }
        }
        delay(10);
    }
}
```

### 3. IAudio (Buzzer del dispositivo)

```cpp
// include/lilygo/LilyGoAudio.h
#ifndef LILYGOAUDIO_H
#define LILYGOAUDIO_H

#include "../interfaces/IAudio.h"

class LilyGoAudio : public IAudio {
public:
    LilyGoAudio(int buzzerPin = 25);
    ~LilyGoAudio() override;
    
    bool init();
    void shutdown();
    
    void playBeep() override;
    void stopBeep() override;
    
private:
    int buzzerPin;
    bool isPlaying;
    static const int FREQUENCY = 440;  // Hz
};

#endif
```

```cpp
// src/lilygo/LilyGoAudio.cpp
#include "../../include/lilygo/LilyGoAudio.h"
#include <Arduino.h>

LilyGoAudio::LilyGoAudio(int buzzerPin) 
    : buzzerPin(buzzerPin), isPlaying(false) {
}

bool LilyGoAudio::init() {
    pinMode(buzzerPin, OUTPUT);
    return true;
}

void LilyGoAudio::shutdown() {
    stopBeep();
}

void LilyGoAudio::playBeep() {
    if (!isPlaying) {
        tone(buzzerPin, FREQUENCY);
        isPlaying = true;
    }
}

void LilyGoAudio::stopBeep() {
    if (isPlaying) {
        noTone(buzzerPin);
        isPlaying = false;
    }
}
```

### 4. IPlatform (Control de la plataforma)

```cpp
// include/lilygo/LilyGoPlatform.h
#ifndef LILYGOPLATFORM_H
#define LILYGOPLATFORM_H

#include "../interfaces/IPlatform.h"
#include "LilyGoDisplay.h"
#include "LilyGoInput.h"
#include "LilyGoAudio.h"

class LilyGoPlatform : public IPlatform {
public:
    LilyGoPlatform(LilyGoDisplay& display, LilyGoInput& input, LilyGoAudio& audio);
    ~LilyGoPlatform() override;
    
    bool init() override;
    void shutdown() override;
    bool shouldQuit() override;
    void delay(uint32_t ms) override;
    uint32_t getTicks() override;
    
private:
    LilyGoDisplay& display;
    LilyGoInput& input;
    LilyGoAudio& audio;
};

#endif
```

```cpp
// src/lilygo/LilyGoPlatform.cpp
#include "../../include/lilygo/LilyGoPlatform.h"
#include <Arduino.h>

LilyGoPlatform::LilyGoPlatform(LilyGoDisplay& display, LilyGoInput& input, LilyGoAudio& audio)
    : display(display), input(input), audio(audio) {
}

bool LilyGoPlatform::init() {
    Serial.begin(115200);
    
    if (!display.init()) {
        return false;
    }
    
    if (!audio.init()) {
        Serial.println("Advertencia: No se pudo inicializar el audio");
    }
    
    return true;
}

void LilyGoPlatform::shutdown() {
    audio.shutdown();
    display.shutdown();
}

bool LilyGoPlatform::shouldQuit() {
    // En un dispositivo embebido normalmente nunca queremos salir
    return false;
}

void LilyGoPlatform::delay(uint32_t ms) {
    ::delay(ms);
}

uint32_t LilyGoPlatform::getTicks() {
    return millis();
}
```

## Main para Arduino/PlatformIO

```cpp
// main.cpp para LilyGO S3
#include "include/core/Chip8.h"
#include "include/core/Config.h"
#include "include/lilygo/LilyGoDisplay.h"
#include "include/lilygo/LilyGoInput.h"
#include "include/lilygo/LilyGoAudio.h"
#include "include/lilygo/LilyGoPlatform.h"

// ROM embebida en el código (ejemplo con IBM Logo)
const uint8_t ROM_DATA[] = {
    0x00, 0xE0, 0xA2, 0x2A, 0x60, 0x0C, 0x61, 0x08, 0xD0, 0x1F, 0x70, 0x09,
    // ... resto de la ROM
};

void setup() {
    // Crear componentes LilyGO
    LilyGoDisplay display(4);
    LilyGoInput input;
    LilyGoAudio audio(25);  // Pin del buzzer
    LilyGoPlatform platform(display, input, audio);
    
    // Inicializar plataforma
    if (!platform.init()) {
        Serial.println("Error al inicializar la plataforma");
        while(1) delay(1000);
    }
    
    // Crear emulador
    Chip8 chip8(display, input, audio);
    
    // Cargar ROM desde memoria
    chip8.loadROMFromMemory(ROM_DATA, sizeof(ROM_DATA));
    
    Serial.println("Emulador CHIP-8 iniciado");
    
    // Ciclo principal
    const int CYCLES_PER_FRAME = 10;
    uint32_t lastTimerUpdate = platform.getTicks();
    
    while (!platform.shouldQuit()) {
        uint32_t frameStart = platform.getTicks();
        
        // Ejecutar ciclos
        for (int i = 0; i < CYCLES_PER_FRAME; ++i) {
            chip8.emulateCycle();
        }
        
        // Actualizar timers a 60 Hz
        uint32_t currentTime = platform.getTicks();
        if (currentTime - lastTimerUpdate >= 16) {
            chip8.updateTimers();
            lastTimerUpdate = currentTime;
        }
        
        // Renderizar
        if (chip8.isDrawFlag()) {
            display.render();
            chip8.clearDrawFlag();
        }
        
        // Control de velocidad (60 FPS)
        uint32_t frameTime = platform.getTicks() - frameStart;
        if (frameTime < 16) {
            platform.delay(16 - frameTime);
        }
    }
}

void loop() {
    // El loop() de Arduino no se usa, todo está en setup()
}
```

## Modificaciones Necesarias en el Core

Para soportar ROMs embebidas en memoria (sin sistema de archivos):

```cpp
// Añadir a include/core/Chip8.h
bool loadROMFromMemory(const uint8_t* data, size_t size);

// Añadir a src/core/Chip8.cpp
bool Chip8::loadROMFromMemory(const uint8_t* data, size_t size) {
    if (size > (4096 - 512)) {
        return false;
    }
    
    std::memcpy(&memory[512], data, size);
    return true;
}
```

## PlatformIO Configuration

```ini
; platformio.ini
[env:lilygo-t-display-s3]
platform = espressif32
board = lilygo-t-display-s3
framework = arduino

lib_deps =
    bodmer/TFT_eSPI@^2.5.0

build_flags =
    -D USER_SETUP_LOADED=1
    -D ST7789_DRIVER=1
    -D TFT_WIDTH=170
    -D TFT_HEIGHT=320
    ; ... otros flags según tu display
```

## Compilación Condicional

Para mantener ambas plataformas en el mismo código:

```cpp
// main.cpp unificado
#ifdef PLATFORM_LILYGO
    #include "include/lilygo/LilyGoDisplay.h"
    #include "include/lilygo/LilyGoInput.h"
    #include "include/lilygo/LilyGoAudio.h"
    #include "include/lilygo/LilyGoPlatform.h"
    
    using PlatformDisplay = LilyGoDisplay;
    using PlatformInput = LilyGoInput;
    using PlatformAudio = LilyGoAudio;
    using PlatformBase = LilyGoPlatform;
#else
    #include "include/sdl/SDLDisplay.h"
    #include "include/sdl/SDLInput.h"
    #include "include/sdl/SDLAudio.h"
    #include "include/sdl/SDLPlatform.h"
    
    using PlatformDisplay = SDLDisplay;
    using PlatformInput = SDLInput;
    using PlatformAudio = SDLAudio;
    using PlatformBase = SDLPlatform;
#endif
```

## Consideraciones de Rendimiento

- El LilyGO S3 tiene menos potencia que un PC, ajusta el número de ciclos por frame si es necesario
- El display TFT es más lento, considera usar double buffering o actualización parcial
- La memoria es limitada, embebe solo las ROMs necesarias

## Resumen

La belleza de esta arquitectura es que:

1. **El core (`Chip8.cpp`) no cambia**: Toda la lógica del emulador es independiente de la plataforma
2. **Solo implementas 4 interfaces**: Display, Input, Audio, Platform
3. **El main cambia mínimamente**: Solo la creación de objetos difiere
4. **Fácil de mantener**: Bugs corregidos en el core benefician a todas las plataformas

¡Buena suerte con el port a LilyGO S3! 🚀

