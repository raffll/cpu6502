#include "gtest/gtest.h"
#include "../cpu6502/cpu6502.h"

TEST(cpu6502, LDA_IMM)
{
	emulator::clock clock;
	emulator::bus bus;
	emulator::cpu6502 cpu(clock, bus);

	clock.reset();
	bus.reset();
	cpu.reset();

	bus.write(0x0200, 0xA9);
	bus.write(0x0201, 0x0A);
	cpu.execute();

	EXPECT_EQ(cpu.A, 0x0A);
	EXPECT_EQ(clock.get_cycles(), cpu.instructions[0xA9].cycles);
}

TEST(cpu6502, LDA_ZPG)
{
	emulator::clock clock;
	emulator::bus bus;
	emulator::cpu6502 cpu(clock, bus);

	clock.reset();
	bus.reset();
	cpu.reset();

	bus.write(0x0200, 0xA5);
	bus.write(0x0201, 0x0A);
	bus.write(0x000A, 0x0B);
	cpu.execute();

	EXPECT_EQ(0x0B, cpu.A);
	EXPECT_EQ(clock.get_cycles(), cpu.instructions[0xA5].cycles);
}

TEST(cpu6502, LDA_ZPX)
{
	emulator::clock clock;
	emulator::bus bus;
	emulator::cpu6502 cpu(clock, bus);

	clock.reset();
	bus.reset();
	cpu.reset();

	cpu.X = 0x0F;
	bus.write(0x0200, 0xB5);
	bus.write(0x0201, 0x80);
	bus.write(0x008F, 0x0C);
	cpu.execute();

	EXPECT_EQ(0x0C, cpu.A);
	EXPECT_EQ(clock.get_cycles(), cpu.instructions[0xB5].cycles);
}