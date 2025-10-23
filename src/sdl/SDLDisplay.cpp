#include "../../include/sdl/SDLDisplay.h"
#include <iostream>

SDLDisplay::SDLDisplay(int scale)
    : window(nullptr), renderer(nullptr), scale(scale) {
    for (auto& row : pixels) {
        row.fill(false);
    }
}

SDLDisplay::~SDLDisplay() {
    shutdown();
}

bool SDLDisplay::init() {
    window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        WIDTH * scale,
        HEIGHT * scale,
        0
    );
    
    if (!window) {
        std::cerr << "Error al crear ventana SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, nullptr);
    
    if (!renderer) {
        std::cerr << "Error al crear renderer SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    return true;
}

void SDLDisplay::shutdown() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void SDLDisplay::clear() {
    for (auto& row : pixels) {
        row.fill(false);
    }
}

void SDLDisplay::setPixel(int x, int y, bool on) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        pixels[y][x] = on;
    }
}

bool SDLDisplay::getPixel(int x, int y) const {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        return pixels[y][x];
    }
    return false;
}

void SDLDisplay::render() {
    // Fondo negro
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Píxeles blancos
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (pixels[y][x]) {
                SDL_FRect rect = {
                    static_cast<float>(x * scale),
                    static_cast<float>(y * scale),
                    static_cast<float>(scale),
                    static_cast<float>(scale)
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    
    SDL_RenderPresent(renderer);
}

