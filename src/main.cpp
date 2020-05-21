#include <chrono>
#include <thread>
#include <fstream>
#include <iterator>
#include <vector>
#include <time.h>

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

	void PrintDebugInfo(unsigned char * V, unsigned short pc, unsigned short I, unsigned char * memory) {

		for (int i = 0; i < 0x10; ++i)
		{
			terminal_printf(67, 1 + i, "[color=orange]V%01X:[/color] 0x%04X [color=gray]%3d", i, V[i], V[i]);
		}

		terminal_printf(67, 16, "[color=orange]%s:[/color] 0x%04X%s", "VF", V[15], " [color=gray]" "(carry flag)");
		
		terminal_printf(67, 18, "[color=orange]%s:[/color] 0x%04X%s", "PC", pc, " [color=gray]" "(program counter)");
		terminal_printf(67, 19, "[color=orange]%s:[/color] 0x%04X%s", " I",  I, " [color=gray]" "nop");
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
		for (int i = 0; i < 64 * 32; ++i)
		{
			gfx[i] = 0;
		}

		// Clear stack
		for (int i = 0; i < 0x10; ++i)
		{
			stack[i] = 0;
		}

		// Clear registers V0-VF
		for (int i = 0; i < 0x10; ++i)
		{
			V[i] = 0x00;
		}
		// Clear memory
		
		// Load fontset
		for(int i = 0; i < 80; ++i) {
			memory[i] = chip8_fontset[i];		
		}

		// Reset timers
		delay_timer = 0;
		sound_timer = 0;

		srand (time(NULL));
	}

	void LoadGame(const char* gameName) {
		// load the program into the memory (use fopen in binary mode)
		
		std::ifstream input( gameName, std::ios::binary );
		// copies all data into buffer
		std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

		for (int i = 0; i < buffer.size(); ++i)
		{
			// 0x200 == 512
			memory[i + 512] = buffer[i];
		}

		terminal_printf(67, 24, "File %s loaded.", gameName);
	}

	void EmulateCycle() {
		// Fetch Opcode
		// Fetch one opcode from the memory at the location
		// specified by the program counter (pc).
		opcode = memory[pc] << 8 | memory[pc + 1];

		terminal_printf(67, 26, "Current opcode: 0x%04X", opcode);

		// Decode Opcode
		switch(opcode & 0xF000)
		{
			case 0x0000:
				switch(opcode & 0x000F)
				{
					case 0x0000: // 0x00E0: Clears the screen
						for (int i = 0; i < 64 * 32; ++i)
						{
							gfx[i] = 0;
						}
						pc += 2;
					break;
					case 0x000E: // 0x00EE: Returns from subroutine
						--sp;
						pc = stack[sp];
						pc += 2;
					break;
					default:
						terminal_printf(67, 26, "[color=red]ERR Unknown opcode: 0x%04X\n", opcode);
					break;
				}
			break;
			case 0x1000:
				pc = opcode & 0x0FFF;
			break;
			case 0x2000: // 0x2NNN: Calls the subroutine at address NNN
				stack[sp] = pc;
				++sp;
				pc = opcode & 0x0FFF;
			break;
			case 0x3000: // 0x3XNN: Skips the next instruction if VX equals NN.
				if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
					pc += 4;
				}
				else
				{
					pc += 2;
				}
			break;
			case 0x4000:
				// 0x4XNN: Skips the next instruction if VX doesn't equal NN.
				if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				{
					pc += 4;
				}
				else {
					pc += 2;
				}
			break;
			case 0x5000:
				// 0x5XY0: Skips the next instruction if VX equals VY.
				if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
				{
					pc += 4;
				}
				else {
					pc += 2;
				}
			break;
			case 0x6000: // 0x6XNN: Sets VX to NN.
				V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
				pc += 2;
			break;
			case 0x7000: // 0x7XNN: Adds NN to VX (Carry flag is not changed)
				V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
				pc += 2;
			break;
			case 0x8000:
				switch(opcode & 0x000F)
				{
					case 0x0000:
						// 0x8XY0: Sets VX to the value of VY
						V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
						pc += 2;
					break;
					case 0x0001:
						// 0x8XY1: Sets VX to VX or VY. (Bitwise OR operation)
						V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
						pc += 2;
					break;
					case 0x0002:
						// 0x8XY2: Sets VX to VX and VY. (Bitwise AND operation)
						V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
						pc += 2;
					break;
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
					case 0x0005: // 0x8XY5: VY is subtracted from VX. 
						// VF is set to 0 when there's a borrow, and 1 when there isn't.
						if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
						{
							V[0xF] = 0; // Borrow
						}
						else
						{
							V[0xF] = 1;
						}
						V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
						pc += 2;
						break;
					default:
						terminal_printf(67, 26, "[color=red]ERR Unknown opcode: 0x%04X\n", opcode);
					break;
				}
			break;
			case 0xA000: // 0xANNN: Sets I to the address NNN
				I = opcode & 0x0FFF;
				pc += 2;
			break;
			case 0xC000: // 0xCXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
				// Vx=rand()&NN
				V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
				pc += 2;
			break;
			case 0xD000:
				x = V[(opcode & 0x0F00) >> 8];
				y = V[(opcode & 0x00F0) >> 4];
				height = opcode & 0x000F;
				// pixel;

				V[0xF] = 0;

				for (int yline = 0; yline < height; yline++)
				{
					pixel = memory[I + yline];

					for (int xline = 0; xline < 8; xline++)
					{
						if ((pixel & (0x80 >> xline)) != 0)
						{
							if (gfx[(x + xline + ((y + yline) * 64))] == 1)
							{
								V[0xF] = 1;
							}
							// gfx[(x + xline + ((y + yline) * 64))] ^= 1;
							gfx[(x + xline + ((y + yline) * 64)) % (64 * 32)] ^= 1;
						}
					}
				}

				drawFlag = true;
				pc += 2;
			break;
			case 0xE000:
				switch(opcode & 0x00FF)
				{
					case 0x009E:
						// 0xEX9E: Skips the next instruction
						// if the key stored in VX is pressed
						if (key[V[(opcode & 0x0F00) >> 8]] != 0)
						{
							pc += 4;
						}
						else
						{
							pc += 2;
						}
					break;
					case 0x00A1:
						// 0xEXA1: Skips the next instruction
						// if the key stored in VX isn't pressed
						if (key[V[(opcode & 0x0F00) >> 8]] == 0)
						{
							pc += 4;
						}
						else
						{
							pc += 2;
						}
					break;
				}
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
					case 0x0018: // 0xFX18: Sets the sound timer to VX.
						// delay_timer(Vx)
						sound_timer = V[(opcode & 0x0F00) >> 8];
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
					case 0x0065:
						// 0xFX65: 
						// Fills V0 to VX (including VX) with values from memory 
						// starting at address I. The offset from I is increased 
						// by 1 for each value written, but I itself 
						// is left unmodified.
						for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
						{
							V[i] = memory[I+i];
						}
						pc += 2;
					break;
					case 0x0029:
						I = V[((opcode & 0x0F00) >> 8)];
						pc += 2;
					break;
					default:
						terminal_printf(67, 26, "[color=red]ERR Unknown opcode: 0x%X\n", opcode);
					break;
				}
			break;
			default:
				terminal_printf(67, 26, "[color=red]ERR Unknown opcode: 0x%X\n", opcode);
		}

		// Update timers
		if(delay_timer > 0)
		{
			--delay_timer;

			if(delay_timer == 0) {
				terminal_print(67,21,"↕ delay fired!");
			}
		}

		if(sound_timer > 0)
		{
			if(sound_timer == 1)
			{
				terminal_print(67,22,"♪ sound fired!");
			}

			--sound_timer;
		}
	}

	void SetKeys() {
		// Keypad                   Keyboard
		// +-+-+-+-+                +-+-+-+-+
		// |1|2|3|C|                |1|2|3|4|
		// +-+-+-+-+                +-+-+-+-+
		// |4|5|6|D|                |Q|W|E|R|
		// +-+-+-+-+       =>       +-+-+-+-+
		// |7|8|9|E|                |A|S|D|F|
		// +-+-+-+-+                +-+-+-+-+
		// |A|0|B|F|                |Z|X|C|V|
		// +-+-+-+-+                +-+-+-+-+
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

	unsigned short x = 0;
	unsigned short y = 0;
	unsigned short height = 0;
	unsigned short pixel = 0;
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
	terminal.GameScreenFrame();
	terminal.Refresh();

	// Clear the memory, registers and screen
	chip8.Initialize();
	// Copy the program into the memory
	chip8.LoadGame("./roms/pong.rom");

	// while (terminal_read() != TK_CLOSE);
	int terminal_input = -1;

	// emulation loop
	while(true)
	{
		// Maintain designated frequency of 60 Hz (16.(6) ms per frame)
		a = std::chrono::system_clock::now();

		// emulate one cycle of the system
		chip8.EmulateCycle();
		// terminal_delay(50);
		if (chip8.drawFlag)
		{
			// drawGraphics();
			for (int i = 0; i < 64 * 32 ; ++i)
			{
				if (chip8.gfx[i] == 1)
				{
					terminal_printf(i%64+1,i/64+1,"█");
				}
				else
				{
					terminal_printf(i%64+1,i/64+1," ");
				}
			}
		}

		chip8.SetKeys();

		b = std::chrono::system_clock::now();
		std::chrono::duration<double, std::milli> work_time = b - a;
		
		if (work_time.count() < 16.666)
		{
			std::chrono::duration<double, std::milli> delta_ms(16.666 - work_time.count());
			auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
			// std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
			terminal_delay(delta_ms_duration.count());
		}

		a = std::chrono::system_clock::now();
		std::chrono::duration<double, std::milli> sleep_time = a - b;

		terminal_clear_area(67, 28, 30, 5);

		terminal_printf(67, 28, "Delay timer: %d", chip8.delay_timer);
		terminal_printf(67, 29, "Sound timer: %d", chip8.sound_timer);

		terminal_printf(67, 31, "Sleep [color=gray]%f[/color] msc per frame", (sleep_time).count());
		terminal_printf(67, 32, "Total [color=gray]%f[/color] msc per frame", (sleep_time + work_time).count());
		
		terminal.PrintDebugInfo(chip8.V, chip8.pc, chip8.I, chip8.memory);

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