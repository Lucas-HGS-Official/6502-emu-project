#include "Bus.h"


Bus::Bus(/* args */) {
    for (auto &i: ram) i = 0x00;

    cpu.Connect_Bus(this);
}

Bus::~Bus() {
}


void Bus::Write(uint16_t addr, uint8_t data) {
    if (addr >= 0x0000 && addr <= 0xFFFF)
        ram[addr] = data;
}

uint8_t Bus::Read(uint16_t addr, bool is_read_only = false) {
    if (addr >= 0x0000 && addr <= 0xFFFF)
        return ram[addr];

    return 0x00;
}