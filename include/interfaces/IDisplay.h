#ifndef IDISPLAY_H
#define IDISPLAY_H

#include <cstdint>

class IDisplay {
public:
    virtual ~IDisplay() = default;
    
    virtual void clear() = 0;
    virtual void setPixel(int x, int y, bool on) = 0;
    virtual void render() = 0;
    virtual bool getPixel(int x, int y) const = 0;
};

#endif // IDISPLAY_H

