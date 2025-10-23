#include "../../include/lilygo/LilyGoDisplay.h"

LilyGoDisplay::LilyGoDisplay(int scale) : scale(scale) {
    for (auto& row : pixels) {
        row.fill(false);
    }
}

LilyGoDisplay::~LilyGoDisplay() {
    shutdown();
}

bool LilyGoDisplay::init() {
    tft.init();
    tft.setRotation(1);  // Ajustar según orientación deseada
    tft.fillScreen(TFT_BLACK);
    return true;
}

void LilyGoDisplay::shutdown() {
    // No hay mucho que limpiar con TFT_eSPI
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
    // Renderizar todos los píxeles
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            uint16_t color = pixels[y][x] ? TFT_WHITE : TFT_BLACK;
            tft.fillRect(x * scale, y * scale, scale, scale, color);
        }
    }
}

