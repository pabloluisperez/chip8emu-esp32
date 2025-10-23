#ifndef IAUDIO_H
#define IAUDIO_H

class IAudio {
public:
    virtual ~IAudio() = default;
    
    virtual void playBeep() = 0;
    virtual void stopBeep() = 0;
};

#endif // IAUDIO_H

