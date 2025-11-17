#include "CPU6502.h"

#include "Bus.h"


CPU6502::CPU6502(/* args */) {
}

CPU6502::~CPU6502() {
}


void CPU6502::Write(uint16_t addr, uint8_t data) {
    bus->Write(addr, data);
}

uint8_t CPU6502::Read(uint16_t addr, bool is_read_only = false) {
    return bus->Read(addr, false);
}