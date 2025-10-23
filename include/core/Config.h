#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

class Config {
public:
    static Config& getInstance();
    
    bool loadKeymap(const std::string& filename);
    std::string getKeyForChip8Key(uint8_t chip8Key) const;
    uint8_t getChip8KeyForSDLKey(const std::string& sdlKey) const;
    
private:
    Config() = default;
    std::map<uint8_t, std::string> chip8ToSDL;
    std::map<std::string, uint8_t> sdlToChip8;
    
    void setDefaultKeymap();
};

#endif // CONFIG_H

