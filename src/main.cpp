#include "BearLibTerminal.h"

class BearTerminal
{
public:
	BearTerminal() = default;
	~BearTerminal() = default;
	void Open() {
		terminal_open();
		terminal_set("window: title='CHIP-8 Emulator', resizeable=true, minimum-size=66x34, size=66x34");
	}
	void Print() {
		terminal_print(1, 1, "Hello, world!");
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
		// Decode Opcode
		// Execute Opcode

		// Update timers
	}

	void SetKeys() {

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

	terminal.Open();
	terminal.GameScreenFrame();
	terminal.Print();
	terminal.Refresh();

	chip8.Initialize();
	chip8.LoadGame("pong");

	// while (terminal_read() != TK_CLOSE);

	// emulation loop
	while(!terminal.ShouldClose())
	{
		// emulate one cycle
		chip8.EmulateCycle();

		if (chip8.drawFlag)
		{
			// drawGraphics();
		}

		chip8.SetKeys();

		int key = terminal_read();

		if (key == TK_CLOSE || key == TK_ESCAPE)
		{
			break;
		}

		// terminal.GameScreenFrame();
		// terminal.Print();
		// terminal.Refresh();
	}

	terminal.Close();

	return 0;
}