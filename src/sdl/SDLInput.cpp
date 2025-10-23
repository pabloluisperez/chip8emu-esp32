#include "../../include/sdl/SDLInput.h"
#include "../../include/core/Config.h"
#include <iostream>

SDLInput::SDLInput() {
    keys.fill(false);
    setupKeyMapping();
}

void SDLInput::setupKeyMapping() {
    // Mapeo basado en la configuración
    Config& config = Config::getInstance();
    
    // Mapear cada tecla CHIP-8 a su tecla SDL correspondiente
    for (uint8_t i = 0; i < 16; ++i) {
        std::string keyStr = config.getKeyForChip8Key(i);
        
        if (keyStr.empty()) continue;
        
        // Convertir string a SDL_Scancode
        SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;
        
        if (keyStr.length() == 1) {
            char c = keyStr[0];
            if (c >= '0' && c <= '9') {
                scancode = static_cast<SDL_Scancode>(SDL_SCANCODE_1 + (c - '1'));
                if (c == '0') scancode = SDL_SCANCODE_0;
            } else if (c >= 'A' && c <= 'Z') {
                scancode = static_cast<SDL_Scancode>(SDL_SCANCODE_A + (c - 'A'));
            } else if (c >= 'a' && c <= 'z') {
                scancode = static_cast<SDL_Scancode>(SDL_SCANCODE_A + (c - 'a'));
            }
        }
        
        if (scancode != SDL_SCANCODE_UNKNOWN) {
            keymapping[scancode] = i;
        }
    }
}

std::string SDLInput::scancodeToString(SDL_Scancode scancode) {
    if (scancode >= SDL_SCANCODE_A && scancode <= SDL_SCANCODE_Z) {
        char c = 'A' + (scancode - SDL_SCANCODE_A);
        return std::string(1, c);
    } else if (scancode >= SDL_SCANCODE_1 && scancode <= SDL_SCANCODE_9) {
        char c = '1' + (scancode - SDL_SCANCODE_1);
        return std::string(1, c);
    } else if (scancode == SDL_SCANCODE_0) {
        return "0";
    }
    return "";
}

bool SDLInput::isKeyPressed(uint8_t key) {
    if (key < 16) {
        return keys[key];
    }
    return false;
}

uint8_t SDLInput::waitForKeyPress() {
    // Este método no se usa directamente en el flujo actual
    // El core maneja el wait internamente
    return 0xFF;
}

void SDLInput::update() {
    // No necesita hacer nada aquí, el event loop maneja las teclas
}

void SDLInput::handleKeyDown(SDL_Scancode scancode) {
    auto it = keymapping.find(scancode);
    if (it != keymapping.end()) {
        keys[it->second] = true;
    }
}

void SDLInput::handleKeyUp(SDL_Scancode scancode) {
    auto it = keymapping.find(scancode);
    if (it != keymapping.end()) {
        keys[it->second] = false;
    }
}

