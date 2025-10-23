#ifndef IINPUT_H
#define IINPUT_H

#include <cstdint>

class IInput {
public:
    virtual ~IInput() = default;
    
    virtual bool isKeyPressed(uint8_t key) = 0;
    virtual uint8_t waitForKeyPress() = 0;
    virtual void update() = 0;
};

#endif // IINPUT_H

