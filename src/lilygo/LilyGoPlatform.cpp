#include "../../include/lilygo/LilyGoPlatform.h"
#include <Arduino.h>

LilyGoPlatform::LilyGoPlatform(LilyGoDisplay& display, LilyGoInput& input, LilyGoAudio& audio)
    : display(display), input(input), audio(audio) {
}

LilyGoPlatform::~LilyGoPlatform() {
    shutdown();
}

bool LilyGoPlatform::init() {
    Serial.begin(115200);
    Serial.println("Inicializando LilyGO Platform...");
    
    if (!display.init()) {
        Serial.println("Error al inicializar display");
        return false;
    }
    
    if (!audio.init()) {
        Serial.println("Advertencia: No se pudo inicializar el audio");
        // Continuar sin audio
    }
    
    Serial.println("LilyGO Platform inicializada");
    return true;
}

void LilyGoPlatform::shutdown() {
    audio.shutdown();
    display.shutdown();
}

bool LilyGoPlatform::shouldQuit() {
    // En dispositivos embebidos normalmente nunca queremos salir
    return false;
}

void LilyGoPlatform::delay(uint32_t ms) {
    ::delay(ms);
}

uint32_t LilyGoPlatform::getTicks() {
    return millis();
}

