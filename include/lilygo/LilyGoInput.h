#ifndef LILYGOINPUT_H
#define LILYGOINPUT_H

#include "../interfaces/IInput.h"
#include <array>

class LilyGoInput : public IInput {
public:
    LilyGoInput();
    ~LilyGoInput() override = default;
    
    bool isKeyPressed(uint8_t key) override;
    uint8_t waitForKeyPress() override;
    void update() override;
    
private:
    std::array<bool, 16> keys;
    std::array<int, 16> gpioMapping;
    
    void setupGPIO();
};

#endif // LILYGOINPUT_H

