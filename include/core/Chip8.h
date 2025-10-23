#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <array>
#include <string>
#include "../interfaces/IDisplay.h"
#include "../interfaces/IInput.h"
#include "../interfaces/IAudio.h"

class Chip8 {
public:
    Chip8(IDisplay& display, IInput& input, IAudio& audio);
    
    bool loadROM(const std::string& filename);
    bool loadROMFromMemory(const uint8_t* data, size_t size);
    void reset();
    void emulateCycle();
    void updateTimers();
    
    bool isDrawFlag() const { return drawFlag; }
    void clearDrawFlag() { drawFlag = false; }

private:
    // Interfaces
    IDisplay& display;
    IInput& input;
    IAudio& audio;
    
    // Memoria y registros
    std::array<uint8_t, 4096> memory;
    std::array<uint8_t, 16> V;  // Registros V0-VF
    uint16_t I;                  // Registro índice
    uint16_t pc;                 // Program counter
    
    // Stack
    std::array<uint16_t, 16> stack;
    uint8_t sp;                  // Stack pointer
    
    // Timers
    uint8_t delayTimer;
    uint8_t soundTimer;
    
    // Estado
    bool drawFlag;
    bool waitingForKey;
    uint8_t keyRegister;
    
    // Métodos para ejecutar opcodes
    void executeOpcode(uint16_t opcode);
    
    // Grupos de opcodes
    void execute0xxx(uint16_t opcode);
    void execute8xxx(uint16_t opcode);
    void executeExxx(uint16_t opcode);
    void executeFxxx(uint16_t opcode);
    
    // Inicialización
    void initializeFontset();
};

#endif // CHIP8_H

