#include "../../include/core/Config.h"
#include <fstream>
#include <sstream>
#include <iostream>

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

void Config::setDefaultKeymap() {
    // Mapeo por defecto: 1234, QWER, ASDF, ZXCV
    chip8ToSDL[0x1] = "1"; sdlToChip8["1"] = 0x1;
    chip8ToSDL[0x2] = "2"; sdlToChip8["2"] = 0x2;
    chip8ToSDL[0x3] = "3"; sdlToChip8["3"] = 0x3;
    chip8ToSDL[0xC] = "4"; sdlToChip8["4"] = 0xC;
    
    chip8ToSDL[0x4] = "Q"; sdlToChip8["Q"] = 0x4;
    chip8ToSDL[0x5] = "W"; sdlToChip8["W"] = 0x5;
    chip8ToSDL[0x6] = "E"; sdlToChip8["E"] = 0x6;
    chip8ToSDL[0xD] = "R"; sdlToChip8["R"] = 0xD;
    
    chip8ToSDL[0x7] = "A"; sdlToChip8["A"] = 0x7;
    chip8ToSDL[0x8] = "S"; sdlToChip8["S"] = 0x8;
    chip8ToSDL[0x9] = "D"; sdlToChip8["D"] = 0x9;
    chip8ToSDL[0xE] = "F"; sdlToChip8["F"] = 0xE;
    
    chip8ToSDL[0xA] = "Z"; sdlToChip8["Z"] = 0xA;
    chip8ToSDL[0x0] = "X"; sdlToChip8["X"] = 0x0;
    chip8ToSDL[0xB] = "C"; sdlToChip8["C"] = 0xB;
    chip8ToSDL[0xF] = "V"; sdlToChip8["V"] = 0xF;
}

bool Config::loadKeymap(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "No se pudo abrir " << filename << ", usando mapeo por defecto\n";
        setDefaultKeymap();
        return false;
    }
    
    chip8ToSDL.clear();
    sdlToChip8.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        // Ignorar comentarios y líneas vacías
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string chip8KeyStr, sdlKeyStr;
        
        if (std::getline(iss, chip8KeyStr, '=') && std::getline(iss, sdlKeyStr)) {
            // Convertir chip8KeyStr a número
            uint8_t chip8Key;
            if (chip8KeyStr.length() == 1 && std::isdigit(chip8KeyStr[0])) {
                chip8Key = chip8KeyStr[0] - '0';
            } else if (chip8KeyStr.length() == 1 && std::isxdigit(chip8KeyStr[0])) {
                chip8Key = std::toupper(chip8KeyStr[0]) - 'A' + 10;
            } else {
                continue;
            }
            
            // Almacenar mapeo
            chip8ToSDL[chip8Key] = sdlKeyStr;
            sdlToChip8[sdlKeyStr] = chip8Key;
        }
    }
    
    file.close();
    
    if (chip8ToSDL.empty()) {
        setDefaultKeymap();
        return false;
    }
    
    return true;
}

std::string Config::getKeyForChip8Key(uint8_t chip8Key) const {
    auto it = chip8ToSDL.find(chip8Key);
    return (it != chip8ToSDL.end()) ? it->second : "";
}

uint8_t Config::getChip8KeyForSDLKey(const std::string& sdlKey) const {
    auto it = sdlToChip8.find(sdlKey);
    return (it != sdlToChip8.end()) ? it->second : 0xFF;
}

