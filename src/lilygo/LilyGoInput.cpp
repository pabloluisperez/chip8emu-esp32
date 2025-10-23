#include "../../include/lilygo/LilyGoInput.h"
#include <Arduino.h>

LilyGoInput::LilyGoInput() {
    keys.fill(false);
    gpioMapping.fill(-1);  // -1 significa sin mapear
    setupGPIO();
}

void LilyGoInput::setupGPIO() {
    // Configuración de ejemplo para LilyGO S3
    // Ajustar según los pines reales disponibles en tu dispositivo
    // Los botones integrados del LilyGO T-Display S3 son:
    gpioMapping[0x0] = 0;   // GPIO 0 (BOOT button)
    gpioMapping[0x1] = 14;  // GPIO 14 (ejemplo)
    // Puedes mapear más teclas a pines GPIO según tu configuración
    
    // Configurar pines como INPUT_PULLUP
    for (int pin : gpioMapping) {
        if (pin >= 0) {
            pinMode(pin, INPUT_PULLUP);
        }
    }
}

void LilyGoInput::update() {
    for (size_t i = 0; i < 16; ++i) {
        if (gpioMapping[i] >= 0) {
            // Botón presionado cuando está en LOW (con pull-up)
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
                // Esperar a que se suelte la tecla
                while (keys[i]) {
                    update();
                    delay(10);
                }
                return i;
            }
        }
        delay(10);
    }
}

