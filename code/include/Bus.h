#pragma once

#include <cstdint>
#include <array>

#include "CPU6502.h"

class Bus {
private:
    /* data */

public:
    Bus();
    ~Bus();

public:     // Dispositivos
    CPU6502 cpu;
    std::array<uint8_t, 64*1024> ram;

public:     // Bus: Ler & Escrever
    void Write(uint16_t addr, uint8_t data);
    uint8_t Read(uint16_t addr, bool is_read_only = false);
};
