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
	void Loop() {
		// Wait until user close the window
		while (terminal_read() != TK_CLOSE) {
			// terminal_refresh();
		}
	}
	void Close() {
		terminal_close();
	}
};

int main(int argc, char const *argv[])
{
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

	BearTerminal terminal;

	terminal.Open();
	terminal.Print();
	terminal.GameScreenFrame();
	terminal.Refresh();
	terminal.Loop();
	terminal.Close();

	return 0;
}