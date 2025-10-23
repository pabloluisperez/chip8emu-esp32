#ifndef SDLINPUT_H
#define SDLINPUT_H

#include "../interfaces/IInput.h"
#include <SDL3/SDL.h>
#include <array>
#include <map>
#include <string>

class SDLInput : public IInput {
public:
    SDLInput();
    ~SDLInput() override = default;
    
    bool isKeyPressed(uint8_t key) override;
    uint8_t waitForKeyPress() override;
    void update() override;
    
    void handleKeyDown(SDL_Scancode scancode);
    void handleKeyUp(SDL_Scancode scancode);
    
private:
    std::array<bool, 16> keys;
    std::map<SDL_Scancode, uint8_t> keymapping;
    
    void setupKeyMapping();
    std::string scancodeToString(SDL_Scancode scancode);
};

#endif // SDLINPUT_H

