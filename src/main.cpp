#include <chrono>
#include <thread>

#include "BearLibTerminal.h"

std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

class BearTerminal
{
public:
	BearTerminal() = default;
	~BearTerminal() = default;
	void Open() {
		terminal_open();
		terminal_set("window: title='CHIP-8 Emulator', resizeable=true, minimum-size=66x34, size=100x34");
	}
	void PrintDebugInfo(unsigned char * V) {
		terminal_printf(67, 1,  "[color=orange]%s:[/color] %s%s", "V0", "0xA2F0", " [color=gray]" "mvi 2F0h");
		terminal_printf(67, 2,  "[color=orange]%s:[/color] 0x%04X%s", "V1", V[1], " [color=gray]" "nop");
		terminal_printf(67, 3,  "[color=orange]%s:[/color] %s%s", "V2", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 4,  "[color=orange]%s:[/color] %s%s", "V3", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 5,  "[color=orange]%s:[/color] %s%s", "V4", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 6,  "[color=orange]%s:[/color] %s%s", "V5", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 7,  "[color=orange]%s:[/color] %s%s", "V6", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 8,  "[color=orange]%s:[/color] %s%s", "V7", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 9,  "[color=orange]%s:[/color] %s%s", "V8", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 10, "[color=orange]%s:[/color] %s%s", "V9", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 11, "[color=orange]%s:[/color] %s%s", "VA", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 12, "[color=orange]%s:[/color] %s%s", "VB", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 13, "[color=orange]%s:[/color] %s%s", "VC", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 14, "[color=orange]%s:[/color] %s%s", "VD", "0x0000", " [color=gray]" "nop");
		terminal_printf(67, 15, "[color=orange]%s:[/color] %s%s", "VE", "0x0000", " [color=gray]" "nop");

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
		return (terminal_read() != TK_CLOSE);
	}
};

class Chip8
{
public:
	Chip8() = default;
	~Chip8() = default;

	void Initialize() {
		// Initialize registers and memory once
		pc = 0x200; // Program counter starts at 0x200
		opcode = 0; // Reset current opcode
		I      = 0; // Reset current register
		sp     = 0; // Reset stack pointer

		// Clear display	
		// Clear stack
		// Clear registers V0-VF
		// Clear memory
		
		// Load fontset
		for(int i = 0; i < 80; ++i) {
			memory[i] = chip8_fontset[i];		
		}

		// Reset timers
		delay_timer = 0;
		sound_timer = 0;
	}

	void LoadGame(const char* gameName) {
		// load the program into the memory (use fopen in binary mode)
		int bufferSize = 0;
		unsigned char buffer[4096];

		for (int i = 0; i < bufferSize; ++i)
		{
			// 0x200 == 512
			memory[i + 512] = buffer[i];
		}
	}

	void EmulateCycle() {
		// Fetch Opcode
		// Fetch one opcode from the memory at the location
		// specified by the program counter (pc).
		opcode = memory[pc] << 8 | memory[pc + 1];

		// Decode Opcode
		switch(opcode & 0xF00)
		{
			case 0x0000:
				switch(opcode & 0x000F)
				{
					case 0x0000: // 0x00E0: Clears the screen
					break;

					case 0x000E: // 0x00EE: Returns from subroutine
					break;
				}
			case 0x2000: // 0x2NNN: Calls the subroutine at address NNN
				stack[sp] = pc;
				++sp;
				pc = opcode & 0x0FFF;
			break;
			case 0x6000: // 0x6XNN: Sets VX to NN.
				V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
				pc += 2;
			break;
			case 0x8000:
				switch(opcode & 0x000F)
				{
					case 0x0004: // 0x8XY4: Adds the value of VY to VX
						if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
						{
							V[0xF] = 1; // Carry
						}
						else
						{
							V[0xF] = 0;
						}

						V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
						pc += 2;
					break;
				}
			break;
			case 0xA000: // 0xANNN: Sets I to the address NNN
				I = opcode & 0x0FFF;
				pc += 2;
			break;
			case 0xD000:
				// 0xDXYN: Draws a sprite at coordinate (VX, VY) that has 
				//         a width of 8 pixels and a height of N pixels.

			break;
			case 0xF000:
				switch(opcode & 0x00FF)
				{
					case 0x0007: // 0xFX07: Sets VX to the value of the delay timer
						// Vx = get_delay()
						V[(opcode & 0x0F00) >> 8] = delay_timer;
						pc += 2;
					break;
					case 0x0015: // 0xFX15: Sets the delay timer to VX.
						// delay_timer(Vx)
						delay_timer = V[(opcode & 0x0F00) >> 8];
						pc += 2;
					break;
					case 0x0033:
						// 0xFX33: Stores the binary-coded decimal representation of VX
						//         at the addresses I, I plus 1, and I plus 2
						memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
						memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
						memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
						pc += 2;
					break;
				}
			break;
			default:
				terminal_printf(70, 19, "Unknown opcode: 0x%X\n", opcode);
		}

		// Update timers
		if(delay_timer > 0)
		{
			--delay_timer;

			// if(delay_timer == 0) {
			// 	terminal_print(70,21,"delay fired!");
			// }
		}

		if(sound_timer > 0)
		{
			if(sound_timer == 1)
			{
				terminal_print(70,20,"sound fired!");
			}

			--sound_timer;
		}
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

// private:
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
	unsigned char delay_timer = 100;
	unsigned char sound_timer = 0;

	// the system's buzzer
	
	// stack
	unsigned short stack[16];
	// stack pointer
	unsigned short sp;

	// a hex based keypad
	// to store the current state of the key
	unsigned char key[16];

	// Each number or character is 4 pixels wide and 5 pixel high.
	unsigned char chip8_fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
};

// using frames = duration<int64_t, ratio<1, 60>>;

int main(int argc, char const *argv[])
{
	BearTerminal terminal;
	Chip8 chip8;

	// Set up render system and register input callbacks
	// Setup the graphics (window size, display mode, etc) and input system (bind callbacks)
	// setupGraphics();
	// setupInput();

	terminal.Open();
	// terminal.GameScreenFrame();
	// terminal.PrintDebugInfo();
	terminal.Refresh();

	// Clear the memory, registers and screen
	chip8.Initialize();
	// Copy the program into the memory
	chip8.LoadGame("pong");

	// while (terminal_read() != TK_CLOSE);
	int terminal_input = -1;

	// emulation loop
	while(true)
	{
		// Maintain designated frequency of 10 Hz (200 ms per frame)
		a = std::chrono::system_clock::now();

		chip8.drawFlag = true;

		// printf("Time: %f \n", (work_time + sleep_time).count());

		// emulate one cycle of the system
		chip8.EmulateCycle();

		if (chip8.drawFlag)
		{
			// drawGraphics();
			terminal.GameScreenFrame();
			terminal.PrintDebugInfo(chip8.V);
			terminal.Refresh();
		}

		// terminal_delay(20);

		chip8.SetKeys();

		b = std::chrono::system_clock::now();
		std::chrono::duration<double, std::milli> work_time = b - a;
		
		if (work_time.count() < 16.666)
		{
			if(chip8.V[1] > 200) chip8.V[1] = 0;
			chip8.V[1]++;
			std::chrono::duration<double, std::milli> delta_ms(16.666 - work_time.count());
			auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
			// std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
			terminal_delay(delta_ms_duration.count());
		}

		a = std::chrono::system_clock::now();
		std::chrono::duration<double, std::milli> sleep_time = a - b;

		// chip8.ClearArea(67, 31, 20, 1);
		terminal_printf(67, 31, "Sleep  [color=gray]%f[/color] msc per frame", (sleep_time).count());
		terminal_printf(67, 32, "Total [color=gray]%f[/color] msc per frame", (sleep_time + work_time).count());
		terminal_printf(67,29, "Delay timer: %d", chip8.delay_timer);
		terminal.Refresh();

		if(terminal_has_input())
		{
			terminal_input = terminal_read();

			if (terminal_input == TK_CLOSE || terminal_input == TK_ESCAPE)
			{
				break;
			}
		}
	}

	terminal.Close();

	return 0;
}