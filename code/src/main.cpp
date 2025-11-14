#include <stdio.h>
#include <stdlib.h>


using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

struct Internal_Memory {
    static constexpr u32 MAX_MEM = 1024 * 24;
    Byte Data[MAX_MEM];

    void Initialise() {
        for (u32 i = 0; i < MAX_MEM; i++) {
            Data[i] = 0;
        }
    }
};

struct CPU_internal_data_structure {

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

    void Reset(Internal_Memory* memory) {
        PC = 0xFFFC;
        SP = 0x0100;

        C = Z = I = D = B = V = N = 0;

        A = X = Y = 0;

        memory->Initialise();
    }
};

int main(void) {
    Internal_Memory mem;
    CPU_internal_data_structure cpu;
    cpu.Reset(&mem);

    return 0;
}