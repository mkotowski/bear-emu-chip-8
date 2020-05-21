#include "BearLibTerminal.h"

class BearTerminal
{
public:
	BearTerminal() = default;
	~BearTerminal() = default;
	void Open() {
		terminal_open();
		terminal_set("window: title='CHIP-8 Emulator', resizeable=true, minimum-size=66x34, size=100x34");
	}
	void PrintDebugInfo() {
		terminal_printf(67, 1,  "[color=orange]%s:[/color] %s%s", "V0", "0xA2F0 " "[color=gray]", "mvi 2F0h");
		terminal_printf(67, 2,  "[color=orange]%s:[/color] %s%s", "V1", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 3,  "[color=orange]%s:[/color] %s%s", "V2", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 4,  "[color=orange]%s:[/color] %s%s", "V3", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 5,  "[color=orange]%s:[/color] %s%s", "V4", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 6,  "[color=orange]%s:[/color] %s%s", "V5", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 7,  "[color=orange]%s:[/color] %s%s", "V6", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 8,  "[color=orange]%s:[/color] %s%s", "V7", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 9,  "[color=orange]%s:[/color] %s%s", "V8", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 10, "[color=orange]%s:[/color] %s%s", "V9", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 11, "[color=orange]%s:[/color] %s%s", "VA", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 12, "[color=orange]%s:[/color] %s%s", "VB", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 13, "[color=orange]%s:[/color] %s%s", "VC", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 14, "[color=orange]%s:[/color] %s%s", "VD", "0x0000 " "[color=gray]", "nop");
		terminal_printf(67, 15, "[color=orange]%s:[/color] %s%s", "VE", "0x0000 " "[color=gray]", "nop");

		terminal_printf(67, 17, "[color=orange]%s:[/color] %s%s", "PC", "0x0000 " "[color=gray]", "nop");
	}
	void GameScreenFrame() {
		terminal_print //64 * 32
		(
			0, 0,
			L"┌────────────────────────────────────────────────────────────────┐\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"│                                                                │\n"
			L"└────────────────────────────────────────────────────────────────┘\n"
		);
	}

	void Refresh() {
		terminal_refresh();
	}

	void Close() {
		terminal_close();
	}

	bool ShouldClose() {
		return !(terminal_read() != TK_CLOSE);
	}
};

class Chip8
{
public:
	Chip8() = default;
	~Chip8() = default;

	void Initialize() {
		// Initialize registers and memory once
	}

	void LoadGame(const char* gameName) {

	}

	void EmulateCycle() {
		// Fetch Opcode
		// Fetch one opcode from the memory at the location
		// specofoed by the program counter (pc).
		opcode = memory[pc] << 8 | memory[pc + 1];

		// Decode Opcode
		// Execute Opcode

		// Update timers
	}

	void SetKeys() {

	}

	void Clear() {
		terminal_clear();
	}

	void ClearArea(int x, int y, int w, int h) {
		terminal_clear_area(x, y, w, h);
	}

	void ClearGameScreen() {
		ClearArea(1, 1, 64, 32);
	}

	bool drawFlag = false;

private:
	// the current opcode
	unsigned short opcode;

	// 4K memory
	// The system memory map:
	// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
	// 0x200-0xFFF - Program ROM and work RAM
	unsigned char memory[4096];

	// CPU registers
	// 15 8-bit general purpose registers V0 to VE
	// The 16th register is used for the "carry flag"
	unsigned char V[16];

	// index register
	unsigned short I;

	// program counter
	unsigned short pc;

	// the screen's pixel states
	// the screen has total of 2048 pixels
	unsigned char gfx[64 * 32];

	// time registers that count at 60 Hz
	// when set above zero they will count down to zero
	unsigned char delay_timer;
	unsigned char sound_timer;

	// the system's buzzer
	
	// stack
	unsigned short stack[16];
	// stack pointer
	unsigned short sp;

	// a hex based keypad
	// to store the current state of the key
	unsigned char key[16];
};

int main(int argc, char const *argv[])
{
	BearTerminal terminal;
	Chip8 chip8;

	// Set up render system and register input callbacks
	// Setup the graphics (window size, display mode, etc) and input system (bind callbacks)
	// setupGraphics();
	// setupInput();

	terminal.Open();
	terminal.GameScreenFrame();
	terminal.PrintDebugInfo();
	terminal.Refresh();

	// Clear the memory, registers and screen
	chip8.Initialize();
	// Copy the program into the memory
	chip8.LoadGame("pong");

	// while (terminal_read() != TK_CLOSE);

	// emulation loop
	while(!terminal.ShouldClose())
	{
		// emulate one cycle of the system
		chip8.EmulateCycle();

		if (chip8.drawFlag)
		{
			// drawGraphics();
			terminal.GameScreenFrame();
			terminal.PrintDebugInfo();
			terminal.Refresh();
		}

		chip8.SetKeys();

		// int key = terminal_read();

		// if (key == TK_CLOSE || key == TK_ESCAPE)
		// {
		// 	break;
		// }
	}

	terminal.Close();

	return 0;
}