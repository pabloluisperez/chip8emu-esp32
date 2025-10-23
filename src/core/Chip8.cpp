#include "../../include/core/Chip8.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <random>

// Fontset de CHIP-8 (0-F)
static const uint8_t CHIP8_FONTSET[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8(IDisplay& display, IInput& input, IAudio& audio)
    : display(display), input(input), audio(audio) {
    reset();
}

void Chip8::reset() {
    // Limpiar memoria
    memory.fill(0);
    
    // Limpiar registros
    V.fill(0);
    I = 0;
    pc = 0x200;  // Los programas empiezan en 0x200
    
    // Limpiar stack
    stack.fill(0);
    sp = 0;
    
    // Limpiar timers
    delayTimer = 0;
    soundTimer = 0;
    
    // Estado
    drawFlag = false;
    waitingForKey = false;
    keyRegister = 0;
    
    // Cargar fontset
    initializeFontset();
    
    // Limpiar display
    display.clear();
}

void Chip8::initializeFontset() {
    for (int i = 0; i < 80; ++i) {
        memory[i] = CHIP8_FONTSET[i];
    }
}

bool Chip8::loadROM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo ROM: " << filename << std::endl;
        return false;
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    if (size > (4096 - 512)) {
        std::cerr << "Error: La ROM es demasiado grande" << std::endl;
        return false;
    }
    
    file.read(reinterpret_cast<char*>(&memory[512]), size);
    file.close();
    
    std::cout << "ROM cargada: " << filename << " (" << size << " bytes)" << std::endl;
    return true;
}

bool Chip8::loadROMFromMemory(const uint8_t* data, size_t size) {
    if (size > (4096 - 512)) {
        std::cerr << "Error: La ROM es demasiado grande" << std::endl;
        return false;
    }
    
    std::memcpy(&memory[512], data, size);
    std::cout << "ROM cargada desde memoria (" << size << " bytes)" << std::endl;
    return true;
}

void Chip8::emulateCycle() {
    if (waitingForKey) {
        input.update();
        for (uint8_t i = 0; i < 16; ++i) {
            if (input.isKeyPressed(i)) {
                V[keyRegister] = i;
                waitingForKey = false;
                break;
            }
        }
        return;
    }
    
    // Fetch
    uint16_t opcode = memory[pc] << 8 | memory[pc + 1];
    
    // Decode & Execute
    executeOpcode(opcode);
}

void Chip8::updateTimers() {
    if (delayTimer > 0) {
        --delayTimer;
    }
    
    if (soundTimer > 0) {
        if (soundTimer == 1) {
            audio.playBeep();
        }
        --soundTimer;
    } else {
        audio.stopBeep();
    }
}

void Chip8::executeOpcode(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = opcode & 0x000F;
    uint8_t nn = opcode & 0x00FF;
    uint16_t nnn = opcode & 0x0FFF;
    
    switch (opcode & 0xF000) {
        case 0x0000:
            execute0xxx(opcode);
            break;
            
        case 0x1000: // 1NNN - Jump a dirección NNN
            pc = nnn;
            break;
            
        case 0x2000: // 2NNN - Call subroutine en NNN
            stack[sp] = pc;
            ++sp;
            pc = nnn;
            break;
            
        case 0x3000: // 3XNN - Skip si VX == NN
            pc += (V[x] == nn) ? 4 : 2;
            break;
            
        case 0x4000: // 4XNN - Skip si VX != NN
            pc += (V[x] != nn) ? 4 : 2;
            break;
            
        case 0x5000: // 5XY0 - Skip si VX == VY
            pc += (V[x] == V[y]) ? 4 : 2;
            break;
            
        case 0x6000: // 6XNN - Set VX = NN
            V[x] = nn;
            pc += 2;
            break;
            
        case 0x7000: // 7XNN - Set VX = VX + NN
            V[x] += nn;
            pc += 2;
            break;
            
        case 0x8000:
            execute8xxx(opcode);
            break;
            
        case 0x9000: // 9XY0 - Skip si VX != VY
            pc += (V[x] != V[y]) ? 4 : 2;
            break;
            
        case 0xA000: // ANNN - Set I = NNN
            I = nnn;
            pc += 2;
            break;
            
        case 0xB000: // BNNN - Jump a V0 + NNN
            pc = V[0] + nnn;
            break;
            
        case 0xC000: { // CXNN - Set VX = random & NN
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> dis(0, 255);
            V[x] = dis(gen) & nn;
            pc += 2;
            break;
        }
            
        case 0xD000: { // DXYN - Draw sprite
            uint8_t xPos = V[x] % 64;
            uint8_t yPos = V[y] % 32;
            V[0xF] = 0;
            
            for (int row = 0; row < n; ++row) {
                if (yPos + row >= 32) break;
                
                uint8_t spriteData = memory[I + row];
                
                for (int col = 0; col < 8; ++col) {
                    if (xPos + col >= 64) break;
                    
                    if ((spriteData & (0x80 >> col)) != 0) {
                        bool currentPixel = display.getPixel(xPos + col, yPos + row);
                        if (currentPixel) {
                            V[0xF] = 1;
                        }
                        display.setPixel(xPos + col, yPos + row, !currentPixel);
                    }
                }
            }
            
            drawFlag = true;
            pc += 2;
            break;
        }
            
        case 0xE000:
            executeExxx(opcode);
            break;
            
        case 0xF000:
            executeFxxx(opcode);
            break;
            
        default:
            std::cerr << "Opcode desconocido: 0x" << std::hex << opcode << std::endl;
            pc += 2;
            break;
    }
}

void Chip8::execute0xxx(uint16_t opcode) {
    switch (opcode & 0x00FF) {
        case 0x00E0: // 00E0 - Clear screen
            display.clear();
            drawFlag = true;
            pc += 2;
            break;
            
        case 0x00EE: // 00EE - Return from subroutine
            --sp;
            pc = stack[sp];
            pc += 2;
            break;
            
        default:
            std::cerr << "Opcode 0x0xxx desconocido: 0x" << std::hex << opcode << std::endl;
            pc += 2;
            break;
    }
}

void Chip8::execute8xxx(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    
    switch (opcode & 0x000F) {
        case 0x0000: // 8XY0 - Set VX = VY
            V[x] = V[y];
            pc += 2;
            break;
            
        case 0x0001: // 8XY1 - Set VX = VX OR VY
            V[x] |= V[y];
            pc += 2;
            break;
            
        case 0x0002: // 8XY2 - Set VX = VX AND VY
            V[x] &= V[y];
            pc += 2;
            break;
            
        case 0x0003: // 8XY3 - Set VX = VX XOR VY
            V[x] ^= V[y];
            pc += 2;
            break;
            
        case 0x0004: { // 8XY4 - Set VX = VX + VY, VF = carry
            uint16_t sum = V[x] + V[y];
            V[0xF] = (sum > 255) ? 1 : 0;
            V[x] = sum & 0xFF;
            pc += 2;
            break;
        }
            
        case 0x0005: // 8XY5 - Set VX = VX - VY, VF = NOT borrow
            V[0xF] = (V[x] > V[y]) ? 1 : 0;
            V[x] -= V[y];
            pc += 2;
            break;
            
        case 0x0006: // 8XY6 - Set VX = VX SHR 1
            V[0xF] = V[x] & 0x1;
            V[x] >>= 1;
            pc += 2;
            break;
            
        case 0x0007: // 8XY7 - Set VX = VY - VX, VF = NOT borrow
            V[0xF] = (V[y] > V[x]) ? 1 : 0;
            V[x] = V[y] - V[x];
            pc += 2;
            break;
            
        case 0x000E: // 8XYE - Set VX = VX SHL 1
            V[0xF] = (V[x] & 0x80) >> 7;
            V[x] <<= 1;
            pc += 2;
            break;
            
        default:
            std::cerr << "Opcode 0x8xxx desconocido: 0x" << std::hex << opcode << std::endl;
            pc += 2;
            break;
    }
}

void Chip8::executeExxx(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    
    switch (opcode & 0x00FF) {
        case 0x009E: // EX9E - Skip si key VX está presionada
            pc += input.isKeyPressed(V[x]) ? 4 : 2;
            break;
            
        case 0x00A1: // EXA1 - Skip si key VX no está presionada
            pc += !input.isKeyPressed(V[x]) ? 4 : 2;
            break;
            
        default:
            std::cerr << "Opcode 0xExxx desconocido: 0x" << std::hex << opcode << std::endl;
            pc += 2;
            break;
    }
}

void Chip8::executeFxxx(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    
    switch (opcode & 0x00FF) {
        case 0x0007: // FX07 - Set VX = delay timer
            V[x] = delayTimer;
            pc += 2;
            break;
            
        case 0x000A: // FX0A - Wait for key press
            waitingForKey = true;
            keyRegister = x;
            pc += 2;
            break;
            
        case 0x0015: // FX15 - Set delay timer = VX
            delayTimer = V[x];
            pc += 2;
            break;
            
        case 0x0018: // FX18 - Set sound timer = VX
            soundTimer = V[x];
            pc += 2;
            break;
            
        case 0x001E: // FX1E - Set I = I + VX
            I += V[x];
            pc += 2;
            break;
            
        case 0x0029: // FX29 - Set I = ubicación del sprite para dígito VX
            I = V[x] * 5;
            pc += 2;
            break;
            
        case 0x0033: // FX33 - Store BCD de VX en I, I+1, I+2
            memory[I] = V[x] / 100;
            memory[I + 1] = (V[x] / 10) % 10;
            memory[I + 2] = V[x] % 10;
            pc += 2;
            break;
            
        case 0x0055: // FX55 - Store V0 a VX en memoria empezando en I
            for (int i = 0; i <= x; ++i) {
                memory[I + i] = V[i];
            }
            pc += 2;
            break;
            
        case 0x0065: // FX65 - Load V0 a VX desde memoria empezando en I
            for (int i = 0; i <= x; ++i) {
                V[i] = memory[I + i];
            }
            pc += 2;
            break;
            
        default:
            std::cerr << "Opcode 0xFxxx desconocido: 0x" << std::hex << opcode << std::endl;
            pc += 2;
            break;
    }
}

