#include "cpu6502.h"

namespace emulator
{
	const std::vector<cpu6502::instruction_t> cpu6502::instructions
	{
		{ &cpu6502::BRK, &cpu6502::___, 7 },
		{ &cpu6502::ORA, &cpu6502::IDX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::ORA, &cpu6502::ZPG, 3 },
		{ &cpu6502::ASL, &cpu6502::ZPG, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::PHP, &cpu6502::___, 3 },
		{ &cpu6502::ORA, &cpu6502::IMM, 2 },
		{ &cpu6502::ASL, &cpu6502::___, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::ORA, &cpu6502::ABS, 4 },
		{ &cpu6502::ASL, &cpu6502::ABS, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::BPL, &cpu6502::REL, 2 },
		{ &cpu6502::ORA, &cpu6502::IDY, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::ORA, &cpu6502::ZPX, 4 },
		{ &cpu6502::ASL, &cpu6502::ZPX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::CLC, &cpu6502::___, 2 },
		{ &cpu6502::ORA, &cpu6502::ABY, 4 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::ORA, &cpu6502::ABX, 4 },
		{ &cpu6502::ASL, &cpu6502::ABX, 7 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::JSR, &cpu6502::ABS, 6 },
		{ &cpu6502::AND, &cpu6502::IDX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::BIT, &cpu6502::ZPG, 3 },
		{ &cpu6502::AND, &cpu6502::ZPG, 3 },
		{ &cpu6502::ROL, &cpu6502::ZPG, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::PLP, &cpu6502::___, 4 },
		{ &cpu6502::AND, &cpu6502::IMM, 2 },
		{ &cpu6502::ROL, &cpu6502::___, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::BIT, &cpu6502::ABS, 4 },
		{ &cpu6502::AND, &cpu6502::ABS, 4 },
		{ &cpu6502::ROL, &cpu6502::ABS, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::BMI, &cpu6502::REL, 2 },
		{ &cpu6502::AND, &cpu6502::IDY, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::AND, &cpu6502::ZPX, 4 },
		{ &cpu6502::ROL, &cpu6502::ZPX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::SEC, &cpu6502::___, 2 },
		{ &cpu6502::AND, &cpu6502::ABY, 4 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::AND, &cpu6502::ABX, 4 },
		{ &cpu6502::ROL, &cpu6502::ABX, 7 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::RTI, &cpu6502::___, 6 },
		{ &cpu6502::EOR, &cpu6502::IDX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::EOR, &cpu6502::ZPG, 3 },
		{ &cpu6502::LSR, &cpu6502::ZPG, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::PHA, &cpu6502::___, 3 },
		{ &cpu6502::EOR, &cpu6502::IMM, 2 },
		{ &cpu6502::LSR, &cpu6502::___, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::JMP, &cpu6502::ABS, 3 },
		{ &cpu6502::EOR, &cpu6502::ABS, 4 },
		{ &cpu6502::LSR, &cpu6502::ABS, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::BVC, &cpu6502::REL, 2 },
		{ &cpu6502::EOR, &cpu6502::IDY, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::EOR, &cpu6502::ZPX, 4 },
		{ &cpu6502::LSR, &cpu6502::ZPX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::CLI, &cpu6502::___, 2 },
		{ &cpu6502::EOR, &cpu6502::ABY, 4 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::EOR, &cpu6502::ABX, 4 },
		{ &cpu6502::LSR, &cpu6502::ABX, 7 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::RTS, &cpu6502::___, 6 },
		{ &cpu6502::ADC, &cpu6502::IDX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::ADC, &cpu6502::ZPG, 3 },
		{ &cpu6502::ROR, &cpu6502::ZPG, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::PLA, &cpu6502::___, 4 },
		{ &cpu6502::ADC, &cpu6502::IMM, 2 },
		{ &cpu6502::ROR, &cpu6502::___, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::JMP, &cpu6502::IND, 5 },
		{ &cpu6502::ADC, &cpu6502::ABS, 4 },
		{ &cpu6502::ROR, &cpu6502::ABS, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::BVS, &cpu6502::REL, 2 },
		{ &cpu6502::ADC, &cpu6502::IDY, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::ADC, &cpu6502::ZPX, 4 },
		{ &cpu6502::ROR, &cpu6502::ZPX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::SEI, &cpu6502::___, 2 },
		{ &cpu6502::ADC, &cpu6502::ABY, 4 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::ADC, &cpu6502::ABX, 4 },
		{ &cpu6502::ROR, &cpu6502::ABX, 7 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::STA, &cpu6502::IDX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::STY, &cpu6502::ZPG, 3 },
		{ &cpu6502::STA, &cpu6502::ZPG, 3 },
		{ &cpu6502::STX, &cpu6502::ZPG, 3 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::DEY, &cpu6502::___, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::TXA, &cpu6502::___, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::STY, &cpu6502::ABS, 4 },
		{ &cpu6502::STA, &cpu6502::ABS, 4 },
		{ &cpu6502::STX, &cpu6502::ABS, 4 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::BCC, &cpu6502::REL, 2 },
		{ &cpu6502::STA, &cpu6502::IDY, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::STY, &cpu6502::ZPX, 4 },
		{ &cpu6502::STA, &cpu6502::ZPX, 4 },
		{ &cpu6502::STX, &cpu6502::ZPY, 4 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::TYA, &cpu6502::___, 2 },
		{ &cpu6502::STA, &cpu6502::ABY, 5 },
		{ &cpu6502::TXS, &cpu6502::___, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::STA, &cpu6502::ABX, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::LDY, &cpu6502::IMM, 2 },
		{ &cpu6502::LDA, &cpu6502::IDX, 6 },
		{ &cpu6502::LDX, &cpu6502::IMM, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::LDY, &cpu6502::ZPG, 3 },
		{ &cpu6502::LDA, &cpu6502::ZPG, 3 },
		{ &cpu6502::LDX, &cpu6502::ZPG, 3 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::TAY, &cpu6502::___, 2 },
		{ &cpu6502::LDA, &cpu6502::IMM, 2 },
		{ &cpu6502::TAX, &cpu6502::___, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::LDY, &cpu6502::ABS, 4 },
		{ &cpu6502::LDA, &cpu6502::ABS, 4 },
		{ &cpu6502::LDX, &cpu6502::ABS, 4 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::BCS, &cpu6502::REL, 2 },
		{ &cpu6502::LDA, &cpu6502::IDY, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::LDY, &cpu6502::ZPX, 4 },
		{ &cpu6502::LDA, &cpu6502::ZPX, 4 },
		{ &cpu6502::LDX, &cpu6502::ZPY, 4 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::CLV, &cpu6502::___, 2 },
		{ &cpu6502::LDA, &cpu6502::ABY, 4 },
		{ &cpu6502::TSX, &cpu6502::___, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::LDY, &cpu6502::ABX, 4 },
		{ &cpu6502::LDA, &cpu6502::ABX, 4 },
		{ &cpu6502::LDX, &cpu6502::ABY, 4 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::CPY, &cpu6502::IMM, 2 },
		{ &cpu6502::CMP, &cpu6502::IDX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::CPY, &cpu6502::ZPG, 3 },
		{ &cpu6502::CMP, &cpu6502::ZPG, 3 },
		{ &cpu6502::DEC, &cpu6502::ZPG, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::INY, &cpu6502::___, 2 },
		{ &cpu6502::CMP, &cpu6502::IMM, 2 },
		{ &cpu6502::DEX, &cpu6502::___, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::CPY, &cpu6502::ABS, 4 },
		{ &cpu6502::CMP, &cpu6502::ABS, 4 },
		{ &cpu6502::DEC, &cpu6502::ABS, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::BNE, &cpu6502::REL, 2 },
		{ &cpu6502::CMP, &cpu6502::IDY, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::CMP, &cpu6502::ZPX, 4 },
		{ &cpu6502::DEC, &cpu6502::ZPX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::CLD, &cpu6502::___, 2 },
		{ &cpu6502::CMP, &cpu6502::ABY, 4 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::CMP, &cpu6502::ABX, 4 },
		{ &cpu6502::DEC, &cpu6502::ABX, 7 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::CPX, &cpu6502::IMM, 2 },
		{ &cpu6502::SBC, &cpu6502::IDX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::CPX, &cpu6502::ZPG, 3 },
		{ &cpu6502::SBC, &cpu6502::ZPG, 3 },
		{ &cpu6502::INC, &cpu6502::ZPG, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::INX, &cpu6502::___, 2 },
		{ &cpu6502::SBC, &cpu6502::IMM, 2 },
		{ &cpu6502::NOP, &cpu6502::___, 2 },
		{ &cpu6502::SBC, &cpu6502::___, 2 },
		{ &cpu6502::CPX, &cpu6502::ABS, 4 },
		{ &cpu6502::SBC, &cpu6502::ABS, 4 },
		{ &cpu6502::INC, &cpu6502::ABS, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::BEQ, &cpu6502::REL, 2 },
		{ &cpu6502::SBC, &cpu6502::IDY, 5 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::SBC, &cpu6502::ZPX, 4 },
		{ &cpu6502::INC, &cpu6502::ZPX, 6 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::SED, &cpu6502::___, 2 },
		{ &cpu6502::SBC, &cpu6502::ABY, 4 },
		{ &cpu6502::___, &cpu6502::___, 2 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::___, &cpu6502::___, 0 },
		{ &cpu6502::SBC, &cpu6502::ABX, 4 },
		{ &cpu6502::INC, &cpu6502::ABX, 7 },
		{ &cpu6502::___, &cpu6502::___, 0 },
	};

	cpu6502::cpu6502(i_clock & clock, i_bus & bus)
		: clock(clock)
		, bus(bus)
	{

	}

	void cpu6502::reset()
	{
		A = {};
		X = {};
		Y = {};

		PC = 0x0200;

		S = 0xFF;
		P = {};

		address = {};
	}

	static bool add_cycle = false;
	static bool add_carry = false;
	static bool acc_addressing = false;

	void cpu6502::execute()
	{
		add_cycle = false;
		add_carry = false;
		acc_addressing = false;

		address = PC;
		PC++;
		uint8_t opcode = bus.read(address);
		clock.cycle();

		instructions[opcode].addressing(*this);
		instructions[opcode].operation(*this);
	}

	void cpu6502::run()
	{
		try
		{
			while (true)
			{
				execute();
			}
		}
		catch (const std::exception &)
		{

		}
	}

	static uint8_t add_register(
		uint8_t byte,
		uint8_t reg,
		bool do_carry = false)
	{
		byte = (byte + reg) & 0xFF;
		add_cycle = do_carry;
		add_carry = do_carry && byte < reg;
		return byte;
	}

	void cpu6502::ACC()
	{
		acc_addressing = true;
	}

	void cpu6502::IMM()
	{
		address = PC;
		PC++;
	}

	void cpu6502::ZPG()
	{
		address = PC;
		PC++;
		uint8_t lo = bus.read(address);
		clock.cycle();

		address = lo;
	}

	void cpu6502::zero_page(uint8_t reg)
	{
		address = PC;
		PC++;
		uint8_t lo = bus.read(address);
		clock.cycle();

		address = lo;
		lo = add_register(lo, reg);
		clock.cycle();

		address = lo;
	}

	void cpu6502::ZPX()
	{
		zero_page(X);
	}

	void cpu6502::ZPY()
	{
		zero_page(Y);
	}

	void cpu6502::ABS()
	{
		address = PC;
		PC++;
		uint8_t lo = bus.read(address);
		clock.cycle();

		address = PC;
		PC++;
		uint8_t hi = bus.read(address);
		clock.cycle();

		address = (hi << 8) | lo;
	}

	void cpu6502::absolute(uint8_t reg)
	{
		address = PC;
		PC++;
		uint8_t lo = bus.read(address);
		clock.cycle();

		address = PC;
		PC++;
		uint8_t hi = bus.read(address);
		lo = add_register(lo, reg, true);
		clock.cycle();

		address = (hi << 8) | lo;
	}

	void cpu6502::ABX()
	{
		absolute(X);
	}

	void cpu6502::ABY()
	{
		absolute(Y);
	}

	void cpu6502::IDX()
	{
		address = PC;
		PC++;
		uint8_t ptr = bus.read(address);
		clock.cycle();

		address = ptr;
		ptr = add_register(ptr, X);
		clock.cycle();

		address = ptr;
		uint8_t lo = bus.read(address);
		clock.cycle();

		address = ptr + 1;
		uint8_t hi = bus.read(address);
		clock.cycle();

		address = (hi << 8) | lo;
	}

	void cpu6502::IDY()
	{
		address = PC;
		PC++;
		uint8_t ptr = bus.read(address);
		clock.cycle();

		address = ptr;
		uint8_t lo = bus.read(address);
		clock.cycle();

		address = ptr + 1;
		uint8_t hi = bus.read(address);
		lo = add_register(lo, Y, true);
		clock.cycle();

		address = (hi << 8) | lo;
	}

	uint8_t cpu6502::load()
	{
		uint8_t data = bus.read(address);
		clock.cycle();

		if (add_carry)
		{
			address = address + 0x0100;
			data = bus.read(address);
			clock.cycle();
		}

		return data;
	}

	static bool check_Z(uint8_t data)
	{
		return (data == 0);
	}

	static bool check_N(uint8_t data)
	{
		return ((data & (1 << 7)) != 0);
	}

	void cpu6502::LDA()
	{
		A = load();
		P.Z = check_Z(A);
		P.N = check_N(A);
	}

	void cpu6502::LDX()
	{
		X = load();
		P.Z = check_Z(X);
		P.N = check_N(X);
	}

	void cpu6502::LDY()
	{
		Y = load();
		P.Z = check_Z(Y);
		P.N = check_N(Y);
	}

	void cpu6502::store(uint8_t & reg)
	{
		if (add_carry)
			address = address + 0x0100;

		if (add_cycle)
			clock.cycle();

		bus.write(address, reg);
		clock.cycle();
	}

	void cpu6502::STA()
	{
		store(A);
	}

	void cpu6502::STX()
	{
		store(X);
	}

	void cpu6502::STY()
	{
		store(Y);
	}

	void cpu6502::TAX()
	{
		address = PC;
		clock.cycle();

		X = A;
		P.Z = check_Z(X);
		P.N = check_N(X);
	};

	void cpu6502::TAY()
	{
		address = PC;
		clock.cycle();

		Y = A;
		P.Z = check_Z(Y);
		P.N = check_N(Y);
	};

	void cpu6502::TXA()
	{
		address = PC;
		clock.cycle();

		A = X;
		P.Z = check_Z(A);
		P.N = check_N(A);
	};

	void cpu6502::TYA()
	{
		address = PC;
		clock.cycle();

		A = Y;
		P.Z = check_Z(A);
		P.N = check_N(A);
	};

	void cpu6502::TSX()
	{
		address = PC;
		clock.cycle();

		X = S;
		P.Z = check_Z(X);
		P.N = check_N(X);
	};

	void cpu6502::TXS()
	{
		address = PC;
		clock.cycle();

		S = X;
	};

	void cpu6502::PHA()
	{
		address = PC;
		clock.cycle();

		address = S;
		bus.write(address, A);
		S--;
		clock.cycle();
	};

	void cpu6502::PHP()
	{
		address = PC;
		clock.cycle();

		address = S;
		bus.write(address, std::bit_cast<uint8_t>(P));
		S--;
		clock.cycle();
	};

	void cpu6502::PLA()
	{
		address = PC;
		clock.cycle();

		address = S;
		S++;
		clock.cycle();

		address = S;
		uint8_t data = bus.read(address);
		clock.cycle();

		A = data;
	};

	void cpu6502::PLP()
	{
		address = PC;
		clock.cycle();

		address = S;
		S++;
		clock.cycle();

		address = S;
		uint8_t data = bus.read(address);
		clock.cycle();

		P = std::bit_cast<status>(data);
	};

	void cpu6502::AND()
	{
		uint8_t data = load();
		A = A & data;
		P.Z = check_Z(A);
		P.N = check_N(A);
	};

	void cpu6502::EOR()
	{
		uint8_t data = load();
		A = A ^ data;
		P.Z = check_Z(A);
		P.N = check_N(A);
	};

	void cpu6502::ORA()
	{
		uint8_t data = load();
		A = A | data;
		P.Z = check_Z(A);
		P.N = check_N(A);
	};

	void cpu6502::BIT()
	{
		uint8_t data = load();
		P.Z = check_Z(A & data);
		P.N = check_N(data);
		P.V = ((data & (1 << 6)) != 0);
	};

	void cpu6502::add_or_substract(uint8_t data)
	{
		uint16_t result = A + data + P.C;

		P.C = result > 0xFF;
		P.V = ((A ^ static_cast<uint8_t>(result)) &
			(data ^ static_cast<uint8_t>(result)) & 0x80) != 0;

		A = static_cast<uint8_t>(result);
		P.Z = check_Z(A);
		P.N = check_N(A);
	}

	void cpu6502::ADC()
	{
		uint8_t data = load();
		add_or_substract(data);
	};

	void cpu6502::SBC()
	{
		uint8_t data = ~load();
		add_or_substract(data);
	};

	void cpu6502::CMP()
	{
		uint8_t data = load();

		P.C = A >= data;
		P.Z = check_Z(A - data);
		P.N = check_N(A - data);
	};

	void cpu6502::CPX()
	{
		uint8_t data = load();

		P.C = X >= data;
		P.Z = check_Z(X - data);
		P.N = check_N(X - data);
	};

	void cpu6502::CPY()
	{
		uint8_t data = load();

		P.C = Y >= data;
		P.Z = check_Z(Y - data);
		P.N = check_N(Y - data);
	};

	void cpu6502::increment(uint8_t & reg)
	{
		reg++;
		P.Z = check_Z(reg);
		P.N = check_N(reg);
	}

	void cpu6502::INC()
	{
		increment(A);
	};

	void cpu6502::INX()
	{
		increment(X);
	};

	void cpu6502::INY()
	{
		increment(Y);
	};

	void cpu6502::decrement(uint8_t & reg)
	{
		reg--;
		P.Z = check_Z(reg);
		P.N = check_N(reg);
	}

	void cpu6502::DEC()
	{
		decrement(A);
	};

	void cpu6502::DEX()
	{
		decrement(X);
	};

	void cpu6502::DEY()
	{
		decrement(Y);
	};

	void cpu6502::ASL()
	{
		if (acc_addressing)
		{
			A = A << 1;
		}
		else
		{
			auto data = load();
			data = data << 1;
			store(data);
		}
	};

	void cpu6502::LSR()
	{

	};
	void cpu6502::ROL()
	{

	};
	void cpu6502::ROR()
	{

	};
}