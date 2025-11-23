#include <iostream>
#include <sstream>

#include "Bus.h"
#include "CPU6502.h"

#include "raylib.h"


#define FONTSIZE 16
#define LETTER_SPACING_Y 15

class Demo_CPU6502 {
public:
    Demo_CPU6502() {}

    Bus nes;
    std::map<uint16_t, std::string> map_asm;

    Font myfont;
    void DrawTextB(const char *text, int posX, int posY, int fontSize, Color color) {
        DrawTextEx(myfont, text, (Vector2){ (float) posX, (float) posY }, fontSize, 1, color);
    }


    std::string Hex(uint32_t n, uint8_t d) {
        std::string s(d, '0');
        for(int i = d-1; i>=0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    };

    void Draw_Ram(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
        int n_ramX = x, n_ramY = y;
        for (int row = 0; row < nRows; row++) {
			std::string s_offset = "$" + Hex(nAddr, 4) + ":";
			for (int col = 0; col < nColumns; col++) {
				s_offset += " " + Hex(nes.Read(nAddr, true), 2);
				nAddr += 1;
			}
            DrawTextB(s_offset.c_str(), n_ramX, n_ramY, FONTSIZE, RAYWHITE);
			n_ramY += LETTER_SPACING_Y;
        }
    }

    void Draw_CPU(int x, int y) {
		std::string status = "STATUS: ";
        DrawTextB("STATUS:", x, y, FONTSIZE, RAYWHITE);
        DrawTextB("N", x + 72, y, FONTSIZE, nes.cpu.status & CPU6502::N ? GREEN : RED);
        DrawTextB("V", x + 88, y, FONTSIZE, nes.cpu.status & CPU6502::V ? GREEN : RED);
        DrawTextB("-", x + 104, y, FONTSIZE, nes.cpu.status & CPU6502::U ? GREEN : RED);
        DrawTextB("B", x + 120, y, FONTSIZE, nes.cpu.status & CPU6502::B ? GREEN : RED);
        DrawTextB("D", x + 136, y, FONTSIZE, nes.cpu.status & CPU6502::D ? GREEN : RED);
        DrawTextB("I", x + 152, y, FONTSIZE, nes.cpu.status & CPU6502::I ? GREEN : RED);
        DrawTextB("Z", x + 168, y, FONTSIZE, nes.cpu.status & CPU6502::Z ? GREEN : RED);
        DrawTextB("C", x + 184, y, FONTSIZE, nes.cpu.status & CPU6502::C ? GREEN : RED);
        DrawTextB(("PC: $" + Hex(nes.cpu.pc, 4)).c_str(), x, y + LETTER_SPACING_Y, FONTSIZE, RAYWHITE);
        DrawTextB(("A: $" + Hex(nes.cpu.a, 2) + " [" + std::to_string(nes.cpu.a) + "]").c_str(), x, y + (LETTER_SPACING_Y * 2), FONTSIZE, RAYWHITE);
        DrawTextB(("X: $" + Hex(nes.cpu.x, 2) + " [" + std::to_string(nes.cpu.x) + "]").c_str(), x, y + (LETTER_SPACING_Y * 3), FONTSIZE, RAYWHITE);
        DrawTextB(("y: $" + Hex(nes.cpu.y, 2) + " [" + std::to_string(nes.cpu.y) + "]").c_str(), x, y + (LETTER_SPACING_Y * 4), FONTSIZE, RAYWHITE);
        DrawTextB(("Stack P: $" + Hex(nes.cpu.stkp, 4)).c_str(), x, y + (LETTER_SPACING_Y * 5), FONTSIZE, RAYWHITE);
    }

    void Draw_Code(int x, int y, int n_lines) {
        auto it_a = map_asm.find(nes.cpu.pc);
        int n_lineY = (n_lines >> 1) * LETTER_SPACING_Y + y;
        if (it_a != map_asm.end()) {
            DrawTextB(((*it_a).second).c_str(), x, n_lineY, FONTSIZE, SKYBLUE);
            while (n_lineY < (n_lines * LETTER_SPACING_Y) + y) {
                n_lineY += LETTER_SPACING_Y;
                if (++it_a != map_asm.end()) {
                    DrawTextB(((*it_a).second).c_str(), x, n_lineY, FONTSIZE, RAYWHITE);
                }
            }
        }

        it_a = map_asm.find(nes.cpu.pc);
        n_lineY = (n_lines >> 1) * LETTER_SPACING_Y + y;
        if (it_a != map_asm.end()) {
            while (n_lineY > y) {
                n_lineY -= LETTER_SPACING_Y;
                if (--it_a != map_asm.end()) {
                    DrawTextB(((*it_a).second).c_str(), x, n_lineY, FONTSIZE, RAYWHITE);
                }
            }
        }
    }

    bool On_User_Create() {
        // Load Program (assembled at https://www.masswerk.at/6502/assembler.html)
		/*
			*=$8000
			LDX #10
			STX $0000
			LDX #3
			STX $0001
			LDY $0000
			LDA #0
			CLC
			loop
			ADC $0001
			DEY
			BNE loop
			STA $0002
			NOP
			NOP
			NOP
		*/
		
		// Convert hex string into bytes for RAM
		std::stringstream ss;
		ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
		uint16_t n_offset = 0x8000;
        while (!ss.eof()) {
            std::string b;
            ss >> b;
            nes.ram[n_offset++] = (uint8_t)std::stoul(b, nullptr, 16);
        }


		// Set Reset Vector
		nes.ram[0xFFFC] = 0x00;
		nes.ram[0xFFFD] = 0x80;

		// Dont forget to set IRQ and NMI vectors if you want to play with those
				
		// Extract dissassembly
		map_asm = nes.cpu.disassemble(0x0000, 0xFFFF);

		// Reset
		nes.cpu.Reset();

		return true;
    }

    bool On_User_Update(float f_elapsed_time) {
		ClearBackground(DARKBLUE);


		if (IsKeyPressed(KEY_SPACE)) {
			do {
				nes.cpu.Clock();
			} while (!nes.cpu.complete());
		}

		if (IsKeyPressed(KEY_R))
			nes.cpu.Reset();

		if (IsKeyPressed(KEY_I))
			nes.cpu.irq();

		if (IsKeyPressed(KEY_N))
			nes.cpu.nmi();

		// Draw Ram Page 0x00		
		Draw_Ram(2, 2, 0x0000, 16, 16);
		Draw_Ram(2, 252, 0x8000, 16, 16);
		Draw_CPU(448, 2);
		Draw_Code(448, 96, 26);


		DrawTextB("SPACE = Step Instruction    R = RESET    I = IRQ    N = NMI", 10, 500, FONTSIZE, RAYWHITE);

		return true;
	}
};

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    Demo_CPU6502 demo;
    demo.myfont = LoadFontEx("resources/fonts/JetBrainsMono-Bold.ttf", FONTSIZE, NULL, 0);
    demo.On_User_Create();
    const int screenWidth = 680 * 2;
    const int screenHeight = 480 * 2;

    InitWindow(screenWidth, screenHeight, "CPU 6502 Demonstration");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            demo.On_User_Update(1.f);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}