#ifndef SDLDISPLAY_H
#define SDLDISPLAY_H

#include "../interfaces/IDisplay.h"
#include <SDL3/SDL.h>
#include <array>

class SDLDisplay : public IDisplay {
public:
    SDLDisplay(int scale = 10);
    ~SDLDisplay() override;
    
    bool init();
    void shutdown();
    
    void clear() override;
    void setPixel(int x, int y, bool on) override;
    void render() override;
    bool getPixel(int x, int y) const override;
    
private:
    static const int WIDTH = 64;
    static const int HEIGHT = 32;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    int scale;
    
    std::array<std::array<bool, WIDTH>, HEIGHT> pixels;
};

#endif // SDLDISPLAY_H

