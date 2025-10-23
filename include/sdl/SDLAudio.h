#ifndef SDLAUDIO_H
#define SDLAUDIO_H

#include "../interfaces/IAudio.h"
#include <SDL3/SDL.h>

class SDLAudio : public IAudio {
public:
    SDLAudio();
    ~SDLAudio() override;
    
    bool init();
    void shutdown();
    
    void playBeep() override;
    void stopBeep() override;
    
private:
    SDL_AudioStream* audioStream;
    bool isPlaying;
    
    static const int FREQUENCY = 440; // Hz (La4)
    static const int SAMPLE_RATE = 44100;
    static const int AMPLITUDE = 28000;
};

#endif // SDLAUDIO_H

