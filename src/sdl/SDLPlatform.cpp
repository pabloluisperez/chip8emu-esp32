#include "../../include/sdl/SDLPlatform.h"
#include <iostream>

SDLPlatform::SDLPlatform(SDLDisplay& display, SDLInput& input, SDLAudio& audio)
    : display(display), input(input), audio(audio), quit(false) {
}

SDLPlatform::~SDLPlatform() {
    shutdown();
}

bool SDLPlatform::init() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if (!display.init()) {
        return false;
    }
    
    if (!audio.init()) {
        std::cerr << "Advertencia: No se pudo inicializar el audio" << std::endl;
        // Continuar sin audio
    }
    
    return true;
}

void SDLPlatform::shutdown() {
    audio.shutdown();
    display.shutdown();
    SDL_Quit();
}

bool SDLPlatform::shouldQuit() {
    return quit;
}

void SDLPlatform::delay(uint32_t ms) {
    SDL_Delay(ms);
}

uint32_t SDLPlatform::getTicks() {
    return SDL_GetTicks();
}

void SDLPlatform::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                quit = true;
                break;
                
            case SDL_EVENT_KEY_DOWN:
                if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = true;
                } else {
                    input.handleKeyDown(event.key.scancode);
                }
                break;
                
            case SDL_EVENT_KEY_UP:
                input.handleKeyUp(event.key.scancode);
                break;
        }
    }
}

