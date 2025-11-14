#include <stdio.h>
#include <stdlib.h>



struct CPU_internal_data_structure {
    using Byte = unsigned char;
    using Word = unsigned short;

    Word PC;        // Program Counter
    Byte SP;        // Stack Counter

    Byte A;         // Accumulator
    Byte X, Y;      // Resgisters

    Byte C : 1;     // Status Flag : Carry Flag
    Byte Z : 1;     // Status Flag : Zero Flag
    Byte I : 1;     // Status Flag : Interrupt Disable Flag
    Byte D : 1;     // Status Flag : Decimal Mode Flag
    Byte B : 1;     // Status Flag : Break Command Flag
    Byte V : 1;     // Status Flag : Overflow Flag
    Byte N : 1;     // Status Flag : Negative Flag

    void Reset() {
        PC = 0xFFFC;
        SP = 0x0100;
        
        C = Z = I = D = B = V = N = 0;

        A = X = Y = 0;
    }
};

int main(void) {
    CPU_internal_data_structure cpu;
    cpu.Reset();

    return 0;
}