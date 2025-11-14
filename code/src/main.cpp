#include <stdio.h>
#include <stdlib.h>



struct CPU_internal_data_structure {
    using Byte = unsigned char;
    using Word = unsigned short;

    Word PC;        // Program Counter
    Byte SP;        // Stack Counter

    Byte AC;        // Accumulator
    Byte X, Y;      // Resgisters

    Byte C : 1;     // Status Flag : Carry Flag
    Byte Z : 1;     // Status Flag : Zero Flag
    Byte I : 1;     // Status Flag : Interrupt Disable Flag
    Byte D : 1;     // Status Flag : Decimal Mode Flag
    Byte B : 1;     // Status Flag : Break Command Flag
    Byte V : 1;     // Status Flag : Overflow Flag
    Byte N : 1;     // Status Flag : Negative Flag
};

int main(void) {

    CPU_internal_data_structure cpu;

    return 0;
}