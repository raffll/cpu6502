#include "gtest/gtest.h"
#include "../cpu6502/cpu6502.h"

class cpu6502_test : public testing::Test
{
public:
	using oc = emulator::cpu6502::oppcode;

	emulator::clock clock;
	emulator::bus bus;
	emulator::cpu6502 cpu { clock, bus };
	uint16_t address = {};

	void SetUp() override
	{
		clock.reset();
		bus.reset();
		cpu.reset();
		address = 0x0200;
	}

	void addressing_IMM()
	{

	}

	void LD_IMM(oc oppcode, uint8_t & reg)
	{
		bus.write(address, oppcode);
		bus.write(++address, 0x0A);
		cpu.execute();

		EXPECT_EQ(reg, 0x0A);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);

		bus.write(++address, oppcode);
		bus.write(++address, 0x00);
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);

		bus.write(++address, oppcode);
		bus.write(++address, 0x80);
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	struct numbers
	{
		uint8_t a;
		uint8_t b;
		uint8_t c;

		uint8_t za;
		uint8_t zb;

		uint8_t na;
		uint8_t nb;
	};

	void logic_IMM(oc oppcode, numbers n)
	{
		bus.write(address, oppcode);
		bus.write(++address, n.a);
		cpu.A = n.b;
		cpu.execute();

		EXPECT_EQ(cpu.A, n.c);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);

		bus.write(++address, oppcode);
		bus.write(++address, n.za);
		cpu.A = n.zb;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);

		bus.write(++address, oppcode);
		bus.write(++address, n.na);
		cpu.A = n.nb;
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	void LD_ZPG(oc oppcode, uint8_t & reg)
	{
		bus.write(address, oppcode);
		bus.write(++address, 0x0A);
		bus.write(0x000A, 0x0B);
		cpu.execute();

		EXPECT_EQ(reg, 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	void ST_ZPG(oc oppcode, uint8_t & reg)
	{
		bus.write(address, oppcode);
		bus.write(++address, 0x0A);
		reg = 0x0B;
		cpu.execute();

		EXPECT_EQ(bus.read(0x000A), 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	void LD_ZP(oc oppcode, uint8_t & reg, uint8_t & offset)
	{
		offset = 0x0F;
		bus.write(address, oppcode);
		bus.write(++address, 0x80);
		bus.write(0x008F, 0x0C);
		cpu.execute();

		EXPECT_EQ(reg, 0x0C);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	void ST_ZP(oc oppcode, uint8_t & reg, uint8_t & offset)
	{
		offset = 0x0F;
		bus.write(address, oppcode);
		bus.write(++address, 0x80);
		reg = 0x0C;
		cpu.execute();

		EXPECT_EQ(bus.read(0x008F), 0x0C);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	void LD_ABS(oc oppcode, uint8_t & reg)
	{
		bus.write(address, oppcode);
		bus.write(++address, 0x80);
		bus.write(++address, 0x40);
		bus.write(0x4080, 0x0D);
		cpu.execute();

		EXPECT_EQ(reg, 0x0D);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	void ST_ABS(oc oppcode, uint8_t & reg)
	{
		bus.write(address, oppcode);
		bus.write(++address, 0x80);
		bus.write(++address, 0x40);
		reg = 0x0D;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080), 0x0D);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	void AB(oc oppcode, uint8_t & offset, bool carry)
	{
		if (carry)
			offset = 0xFF;
		else
			offset = 0x0F;

		bus.write(address, oppcode);
		bus.write(++address, 0x80);
		bus.write(++address, 0x40);
	}

	void LD_AB(oc oppcode, uint8_t & reg, uint8_t & offset, bool carry = false)
	{
		AB(oppcode, offset, carry);
		bus.write(0x4080 + offset, 0x0E);
		cpu.execute();

		EXPECT_EQ(reg, 0x0E);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles + carry);
	}

	void ST_AB(oc oppcode, uint8_t & reg, uint8_t & offset, bool carry = false)
	{
		AB(oppcode, offset, carry);
		reg = 0x0E;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080 + offset), 0x0E);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	void IDX(oc oppcode)
	{
		cpu.X = 0x0F;
		bus.write(address, oppcode);
		bus.write(++address, 0x20);
		bus.write(0x002F, 0x80);
		bus.write(0x0030, 0x40);
	}

	void LD_IDX(oc oppcode, uint8_t & reg)
	{
		IDX(oppcode);
		bus.write(0x4080, 0x0A);
		cpu.execute();

		EXPECT_EQ(reg, 0x0A);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	void ST_IDX(oc oppcode, uint8_t & reg)
	{
		IDX(oppcode);
		reg = 0x0A;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080), 0x0A);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	void IDY(oc oppcode, bool carry)
	{
		if (carry)
			cpu.Y = 0xFF;
		else
			cpu.Y = 0x0F;

		bus.write(address, oppcode);
		bus.write(++address, 0x20);
		bus.write(0x0020, 0x80);
		bus.write(0x0021, 0x40);
	}

	void LD_IDY(oc oppcode, uint8_t & reg, bool carry = false)
	{
		IDY(oppcode, carry);
		bus.write(0x4080 + cpu.Y, 0x0B);
		cpu.execute();

		EXPECT_EQ(reg, 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles + carry);
	}

	void ST_IDY(oc oppcode, uint8_t & reg, bool carry = false)
	{
		IDY(oppcode, carry);
		reg = 0x0B;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080 + cpu.Y), 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	void T(oc oppcode, uint8_t & src, uint8_t & dst, bool check = true)
	{
		bus.write(address, oppcode);
		src = 0x0A;
		cpu.execute();

		EXPECT_EQ(src, dst);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);

		if (check)
		{
			bus.write(++address, oppcode);
			src = 0x00;
			cpu.execute();

			EXPECT_EQ(cpu.P.Z, 1);

			bus.write(++address, oppcode);
			src = 0x80;
			cpu.execute();

			EXPECT_EQ(cpu.P.N, 1);
		}
	}
};

TEST_F(cpu6502_test, LDA_IMM)
{
	LD_IMM(oc::LDA_IMM, cpu.A);
}

TEST_F(cpu6502_test, LDX_IMM)
{
	LD_IMM(oc::LDX_IMM, cpu.X);
}

TEST_F(cpu6502_test, LDY_IMM)
{
	LD_IMM(oc::LDY_IMM, cpu.Y);
}

TEST_F(cpu6502_test, LDA_ZPG)
{
	LD_ZPG(oc::LDA_ZPG, cpu.A);
}

TEST_F(cpu6502_test, LDX_ZPG)
{
	LD_ZPG(oc::LDX_ZPG, cpu.X);
}

TEST_F(cpu6502_test, LDY_ZPG)
{
	LD_ZPG(oc::LDY_ZPG, cpu.Y);
}

TEST_F(cpu6502_test, LDA_ZPX)
{
	LD_ZP(oc::LDA_ZPX, cpu.A, cpu.X);
}

TEST_F(cpu6502_test, LDX_ZPY)
{
	LD_ZP(oc::LDX_ZPY, cpu.X, cpu.Y);
}

TEST_F(cpu6502_test, LDY_ZPX)
{
	LD_ZP(oc::LDY_ZPX, cpu.Y, cpu.X);
}

TEST_F(cpu6502_test, LDA_ABS)
{
	LD_ABS(oc::LDA_ABS, cpu.A);
}

TEST_F(cpu6502_test, LDX_ABS)
{
	LD_ABS(oc::LDX_ABS, cpu.X);
}

TEST_F(cpu6502_test, LDY_ABS)
{
	LD_ABS(oc::LDY_ABS, cpu.Y);
}

TEST_F(cpu6502_test, LDA_ABX)
{
	LD_AB(oc::LDA_ABX, cpu.A, cpu.X);
}

TEST_F(cpu6502_test, LDA_ABX_C)
{
	LD_AB(oc::LDA_ABX, cpu.A, cpu.X, true);
}

TEST_F(cpu6502_test, LDA_ABY)
{
	LD_AB(oc::LDA_ABY, cpu.A, cpu.Y);
}

TEST_F(cpu6502_test, LDA_ABY_C)
{
	LD_AB(oc::LDA_ABY, cpu.A, cpu.Y, true);
}

TEST_F(cpu6502_test, LDX_ABY)
{
	LD_AB(oc::LDX_ABY, cpu.X, cpu.Y);
}

TEST_F(cpu6502_test, LDX_ABY_C)
{
	LD_AB(oc::LDX_ABY, cpu.X, cpu.Y, true);
}

TEST_F(cpu6502_test, LDY_ABX)
{
	LD_AB(oc::LDY_ABX, cpu.Y, cpu.X);
}

TEST_F(cpu6502_test, LDY_ABX_C)
{
	LD_AB(oc::LDY_ABX, cpu.Y, cpu.X, true);
}

TEST_F(cpu6502_test, LDA_IDX)
{
	LD_IDX(oc::LDA_IDX, cpu.A);
}

TEST_F(cpu6502_test, LDA_IDY)
{
	LD_IDY(oc::LDA_IDY, cpu.A);
}

TEST_F(cpu6502_test, LDA_IDY_C)
{
	LD_IDY(oc::LDA_IDY, cpu.A, true);
}

TEST_F(cpu6502_test, STA_ZPG)
{
	ST_ZPG(oc::STA_ZPG, cpu.A);
}

TEST_F(cpu6502_test, STX_ZPG)
{
	ST_ZPG(oc::STX_ZPG, cpu.X);
}

TEST_F(cpu6502_test, STY_ZPG)
{
	ST_ZPG(oc::STY_ZPG, cpu.Y);
}

TEST_F(cpu6502_test, STA_ZPX)
{
	ST_ZP(oc::STA_ZPX, cpu.A, cpu.X);
}

TEST_F(cpu6502_test, STX_ZPY)
{
	ST_ZP(oc::STX_ZPY, cpu.X, cpu.Y);
}

TEST_F(cpu6502_test, STY_ZPX)
{
	ST_ZP(oc::STY_ZPX, cpu.Y, cpu.X);
}

TEST_F(cpu6502_test, STA_ABS)
{
	ST_ABS(oc::STA_ABS, cpu.A);
}

TEST_F(cpu6502_test, STX_ABS)
{
	ST_ABS(oc::STX_ABS, cpu.X);
}

TEST_F(cpu6502_test, STY_ABS)
{
	ST_ABS(oc::STY_ABS, cpu.Y);
}

TEST_F(cpu6502_test, STA_ABX)
{
	ST_AB(oc::STA_ABX, cpu.A, cpu.X);
}

TEST_F(cpu6502_test, STA_ABX_C)
{
	ST_AB(oc::STA_ABX, cpu.A, cpu.X, true);
}

TEST_F(cpu6502_test, STA_ABY)
{
	ST_AB(oc::STA_ABY, cpu.A, cpu.Y);
}

TEST_F(cpu6502_test, STA_ABY_C)
{
	ST_AB(oc::STA_ABY, cpu.A, cpu.Y, true);
}

TEST_F(cpu6502_test, STA_IDX)
{
	ST_IDX(oc::STA_IDX, cpu.A);
}

TEST_F(cpu6502_test, STA_IDY)
{
	ST_IDY(oc::STA_IDY, cpu.A);
}

TEST_F(cpu6502_test, STA_IDY_C)
{
	ST_IDY(oc::STA_IDY, cpu.A, true);
}

TEST_F(cpu6502_test, TAX)
{
	T(oc::TAX____, cpu.A, cpu.X);
}

TEST_F(cpu6502_test, TAY)
{
	T(oc::TAY____, cpu.A, cpu.Y);
}

TEST_F(cpu6502_test, TXA)
{
	T(oc::TXA____, cpu.X, cpu.A);
}

TEST_F(cpu6502_test, TYA)
{
	T(oc::TYA____, cpu.Y, cpu.A);
}

TEST_F(cpu6502_test, TSX)
{
	T(oc::TSX____, cpu.S, cpu.X);
}

TEST_F(cpu6502_test, TXS)
{
	T(oc::TXS____, cpu.X, cpu.S, false);
}

TEST_F(cpu6502_test, PHA)
{
	auto oppcode = oc::PHA____;

	bus.write(address, oppcode);
	cpu.A = 0x0B;
	cpu.execute();

	EXPECT_EQ(cpu.S, 0xFE);
	EXPECT_EQ(bus.read(0x00FF), 0x0B);
	EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
}

TEST_F(cpu6502_test, PHP)
{
	auto oppcode = oc::PHP____;

	bus.write(address, oppcode);
	cpu.P.N = 1;
	cpu.execute();

	EXPECT_EQ(cpu.S, 0xFE);
	EXPECT_EQ(bus.read(0x00FF), 0x80);
	EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
}

TEST_F(cpu6502_test, PLA)
{
	auto oppcode = oc::PLA____;

	bus.write(address, oppcode);
	bus.write(0x00FF, 0x0C);
	cpu.S = 0xFE;
	cpu.execute();

	EXPECT_EQ(cpu.S, 0xFF);
	EXPECT_EQ(cpu.A, 0x0C);
	EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
}

TEST_F(cpu6502_test, PLP)
{
	auto oppcode = oc::PLP____;

	bus.write(address, oppcode);
	bus.write(0x00FF, 0x80);
	cpu.S = 0xFE;
	cpu.execute();

	EXPECT_EQ(cpu.S, 0xFF);
	EXPECT_EQ(cpu.P.N, 1);
	EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
}

TEST_F(cpu6502_test, AND)
{
	// 000000110 &
	// 000000011
	// ---------
	// 000000010

	logic_IMM(oc::AND_IMM, { 0x06, 0x03, 0x02, 0, 0, 0x80, 0x80 });
}

TEST_F(cpu6502_test, EOR)
{
	// 000000110 ^
	// 000000011
	// ---------
	// 000000101

	logic_IMM(oc::EOR_IMM, { 0x06, 0x03, 0x05, 0, 0, 0x80, 0 });
}

TEST_F(cpu6502_test, ORA)
{
	// 000000110 |
	// 000000011
	// ---------
	// 000000111

	logic_IMM(oc::ORA_IMM, { 0x06, 0x03, 0x07, 0, 0, 0x80, 0 });
}

TEST_F(cpu6502_test, BIT)
{
	auto oppcode = oc::BIT_ZPG;

	bus.write(address, oppcode);
	bus.write(++address, 0x0A);
	bus.write(0x000A, 0x00);
	cpu.A = 0x00;
	cpu.execute();

	EXPECT_EQ(cpu.P.Z, 1);
	EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);

	bus.write(++address, oppcode);
	bus.write(++address, 0x0A);
	bus.write(0x000A, 0xFE);
	cpu.A = 0xFF;
	cpu.execute();

	EXPECT_EQ(cpu.P.Z, 0);
	EXPECT_EQ(cpu.P.V, 1);
	EXPECT_EQ(cpu.P.N, 1);
	EXPECT_EQ(cpu.A, 0xFF);
}