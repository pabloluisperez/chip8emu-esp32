#ifndef LILYGOAUDIO_H
#define LILYGOAUDIO_H

#include "../interfaces/IAudio.h"

class LilyGoAudio : public IAudio {
public:
    LilyGoAudio(int buzzerPin = 25);
    ~LilyGoAudio() override;
    
    bool init();
    void shutdown();
    
    void playBeep() override;
    void stopBeep() override;
    
private:
    int buzzerPin;
    bool isPlaying;
    static const int FREQUENCY = 440;  // Hz (La4)
};

#endif // LILYGOAUDIO_H

