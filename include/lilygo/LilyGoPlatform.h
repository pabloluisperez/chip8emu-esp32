#ifndef LILYGOPLATFORM_H
#define LILYGOPLATFORM_H

#include "../interfaces/IPlatform.h"
#include "LilyGoDisplay.h"
#include "LilyGoInput.h"
#include "LilyGoAudio.h"

class LilyGoPlatform : public IPlatform {
public:
    LilyGoPlatform(LilyGoDisplay& display, LilyGoInput& input, LilyGoAudio& audio);
    ~LilyGoPlatform() override;
    
    bool init() override;
    void shutdown() override;
    bool shouldQuit() override;
    void delay(uint32_t ms) override;
    uint32_t getTicks() override;
    
private:
    LilyGoDisplay& display;
    LilyGoInput& input;
    LilyGoAudio& audio;
};

#endif // LILYGOPLATFORM_H

