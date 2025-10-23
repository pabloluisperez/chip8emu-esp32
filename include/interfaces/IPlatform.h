#ifndef IPLATFORM_H
#define IPLATFORM_H

#include <cstdint>

class IPlatform {
public:
    virtual ~IPlatform() = default;
    
    virtual bool init() = 0;
    virtual void shutdown() = 0;
    virtual bool shouldQuit() = 0;
    virtual void delay(uint32_t ms) = 0;
    virtual uint32_t getTicks() = 0;
};

#endif // IPLATFORM_H

