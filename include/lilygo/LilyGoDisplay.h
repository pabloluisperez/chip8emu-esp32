#ifndef LILYGODISPLAY_H
#define LILYGODISPLAY_H

#include "../interfaces/IDisplay.h"
#include <TFT_eSPI.h>
#include <array>

class LilyGoDisplay : public IDisplay {
public:
    LilyGoDisplay(int scale = 4);
    ~LilyGoDisplay() override;
    
    bool init();
    void shutdown();
    
    void clear() override;
    void setPixel(int x, int y, bool on) override;
    void render() override;
    bool getPixel(int x, int y) const override;
    
private:
    static const int WIDTH = 64;
    static const int HEIGHT = 32;
    
    TFT_eSPI tft;
    int scale;
    
    std::array<std::array<bool, WIDTH>, HEIGHT> pixels;
};

#endif // LILYGODISPLAY_H

