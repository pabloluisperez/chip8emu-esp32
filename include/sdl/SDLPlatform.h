#ifndef SDLPLATFORM_H
#define SDLPLATFORM_H

#include "../interfaces/IPlatform.h"
#include "SDLDisplay.h"
#include "SDLInput.h"
#include "SDLAudio.h"
#include <SDL3/SDL.h>

class SDLPlatform : public IPlatform {
public:
    SDLPlatform(SDLDisplay& display, SDLInput& input, SDLAudio& audio);
    ~SDLPlatform() override;
    
    bool init() override;
    void shutdown() override;
    bool shouldQuit() override;
    void delay(uint32_t ms) override;
    uint32_t getTicks() override;
    
    void processEvents();
    
private:
    SDLDisplay& display;
    SDLInput& input;
    SDLAudio& audio;
    bool quit;
};

#endif // SDLPLATFORM_H

