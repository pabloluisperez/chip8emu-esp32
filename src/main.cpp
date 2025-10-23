#include "../include/core/Chip8.h"
#include "../include/core/Config.h"

// Compilación condicional según la plataforma
#ifdef PLATFORM_LILYGO
    #include "../include/lilygo/LilyGoDisplay.h"
    #include "../include/lilygo/LilyGoInput.h"
    #include "../include/lilygo/LilyGoAudio.h"
    #include "../include/lilygo/LilyGoPlatform.h"
    #include <Arduino.h>
    
    // ROM embebida para LilyGO (IBM Logo)
    const uint8_t ROM_DATA[] = {
        0x00, 0xE0, 0xA2, 0x2A, 0x60, 0x0C, 0x61, 0x08, 0xD0, 0x1F, 0x70, 0x09,
        0xA2, 0x39, 0xD0, 0x1F, 0xA2, 0x48, 0x70, 0x08, 0xD0, 0x1F, 0x70, 0x04,
        0xA2, 0x57, 0xD0, 0x1F, 0x70, 0x08, 0xA2, 0x66, 0xD0, 0x1F, 0x70, 0x08,
        0xA2, 0x75, 0xD0, 0x1F, 0x12, 0x28, 0xFF, 0x00, 0xFF, 0x00, 0x3C, 0x00,
        0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF,
        0x00, 0x38, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x38, 0x00, 0xFF, 0x00, 0xFF,
        0x80, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0x80, 0x00, 0x80, 0x00, 0xE0, 0x00,
        0xE0, 0x00, 0x80, 0xF8, 0x00, 0xFC, 0x00, 0x3E, 0x00, 0x3F, 0x00, 0x3B,
        0x00, 0x39, 0x00, 0xF8, 0x00, 0xF8, 0x03, 0x00, 0x07, 0x00, 0x0F, 0x00,
        0xBF, 0x00, 0xFB, 0x00, 0xF3, 0x00, 0xE3, 0x00, 0x43, 0xE0, 0x00, 0xE0,
        0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xE0, 0x00, 0xE0
    };
    
    using PlatformDisplay = LilyGoDisplay;
    using PlatformInput = LilyGoInput;
    using PlatformAudio = LilyGoAudio;
    using PlatformBase = LilyGoPlatform;
#else
    #include "../include/sdl/SDLDisplay.h"
    #include "../include/sdl/SDLInput.h"
    #include "../include/sdl/SDLAudio.h"
    #include "../include/sdl/SDLPlatform.h"
    #include <iostream>
    #include <string>
    
    using PlatformDisplay = SDLDisplay;
    using PlatformInput = SDLInput;
    using PlatformAudio = SDLAudio;
    using PlatformBase = SDLPlatform;
#endif

#ifdef PLATFORM_LILYGO
// ===== IMPLEMENTACIÓN PARA LILYGO =====

void setup() {
    Serial.begin(115200);
    Serial.println("=================================");
    Serial.println("   Emulador CHIP-8 para LilyGO");
    Serial.println("=================================");
    
    // Crear componentes LilyGO
    static LilyGoDisplay display(4);  // Escala 4x para el display más pequeño
    static LilyGoInput input;
    static LilyGoAudio audio(25);  // GPIO 25 para buzzer (ajustar según tu hardware)
    static LilyGoPlatform platform(display, input, audio);
    
    // Inicializar plataforma
    if (!platform.init()) {
        Serial.println("Error al inicializar la plataforma");
        while(1) delay(1000);
    }
    
    // Crear emulador
    static Chip8 chip8(display, input, audio);
    
    // Cargar ROM desde memoria
    if (!chip8.loadROMFromMemory(ROM_DATA, sizeof(ROM_DATA))) {
        Serial.println("Error al cargar ROM");
        while(1) delay(1000);
    }
    
    Serial.println("Emulador CHIP-8 iniciado");
    Serial.println("ROM: IBM Logo Test");
    
    // Ciclo principal
    const int CYCLES_PER_FRAME = 10;  // ~600 Hz
    uint32_t lastTimerUpdate = platform.getTicks();
    
    while (!platform.shouldQuit()) {
        uint32_t frameStart = platform.getTicks();
        
        // Actualizar input
        input.update();
        
        // Ejecutar varios ciclos por frame
        for (int i = 0; i < CYCLES_PER_FRAME; ++i) {
            chip8.emulateCycle();
        }
        
        // Actualizar timers a 60 Hz
        uint32_t currentTime = platform.getTicks();
        if (currentTime - lastTimerUpdate >= 16) {  // ~60 Hz
            chip8.updateTimers();
            lastTimerUpdate = currentTime;
        }
        
        // Renderizar si es necesario
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
    delay(1000);
}

#else
// ===== IMPLEMENTACIÓN PARA SDL3 =====

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Uso: " << argv[0] << " <archivo_rom>" << std::endl;
        std::cout << "Ejemplo: " << argv[0] << " roms/pong.ch8" << std::endl;
        return 1;
    }
    
    std::string romPath = argv[1];
    
    // Cargar configuración de teclas
    Config& config = Config::getInstance();
    config.loadKeymap("config/keymap.cfg");
    
    // Crear componentes SDL
    SDLDisplay display(10);  // Escala 10x
    SDLInput input;
    SDLAudio audio;
    SDLPlatform platform(display, input, audio);
    
    // Inicializar plataforma
    if (!platform.init()) {
        std::cerr << "Error al inicializar la plataforma SDL" << std::endl;
        return 1;
    }
    
    // Crear emulador
    Chip8 chip8(display, input, audio);
    
    // Cargar ROM
    if (!chip8.loadROM(romPath)) {
        platform.shutdown();
        return 1;
    }
    
    std::cout << "Emulador CHIP-8 iniciado" << std::endl;
    std::cout << "Presiona ESC para salir" << std::endl;
    
    // Ciclo principal
    const int TARGET_FPS = 60;
    const int FRAME_DELAY = 1000 / TARGET_FPS;
    const int CYCLES_PER_FRAME = 10;  // ~600 Hz
    
    uint32_t lastTimerUpdate = platform.getTicks();
    
    while (!platform.shouldQuit()) {
        uint32_t frameStart = platform.getTicks();
        
        // Procesar eventos
        platform.processEvents();
        
        // Ejecutar varios ciclos por frame
        for (int i = 0; i < CYCLES_PER_FRAME; ++i) {
            chip8.emulateCycle();
        }
        
        // Actualizar timers a 60 Hz
        uint32_t currentTime = platform.getTicks();
        if (currentTime - lastTimerUpdate >= 16) {  // ~60 Hz
            chip8.updateTimers();
            lastTimerUpdate = currentTime;
        }
        
        // Renderizar si es necesario
        if (chip8.isDrawFlag()) {
            display.render();
            chip8.clearDrawFlag();
        }
        
        // Control de velocidad
        uint32_t frameTime = platform.getTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            platform.delay(FRAME_DELAY - frameTime);
        }
    }
    
    platform.shutdown();
    std::cout << "Emulador cerrado" << std::endl;
    
    return 0;
}

#endif
