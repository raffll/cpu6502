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
		uint8_t offset,
		bool do_carry = false)
	{
		byte = (byte + offset) & 0xFF;
		add_cycle = do_carry;
		add_carry = do_carry && byte < offset;
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

	void cpu6502::zero_page(uint8_t offset)
	{
		address = PC;
		PC++;
		uint8_t lo = bus.read(address);
		clock.cycle();

		address = lo;
		lo = add_register(lo, offset);
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

	void cpu6502::absolute(uint8_t offset)
	{
		address = PC;
		PC++;
		uint8_t lo = bus.read(address);
		clock.cycle();

		address = PC;
		PC++;
		uint8_t hi = bus.read(address);
		lo = add_register(lo, offset, true);
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

	static bool is_zero(uint8_t data)
	{
		return data == 0;
	}

	static bool is_negative(uint8_t data)
	{
		return (data & (1 << 7)) != 0;
	}

	void cpu6502::LDA()
	{
		A = load();
		P.Z = is_zero(A);
		P.N = is_negative(A);
	}

	void cpu6502::LDX()
	{
		X = load();
		P.Z = is_zero(X);
		P.N = is_negative(X);
	}

	void cpu6502::LDY()
	{
		Y = load();
		P.Z = is_zero(Y);
		P.N = is_negative(Y);
	}

	void cpu6502::store(uint8_t data)
	{
		if (add_carry)
			address = address + 0x0100;

		if (add_cycle)
			clock.cycle();

		bus.write(address, data);
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

	void cpu6502::transfer(uint8_t src, uint8_t & dst)
	{
		address = PC;
		clock.cycle();

		dst = src;
		P.Z = is_zero(dst);
		P.N = is_negative(dst);
	}

	void cpu6502::TAX()
	{
		transfer(A, X);
	};

	void cpu6502::TAY()
	{
		transfer(A, Y);
	};

	void cpu6502::TXA()
	{
		transfer(X, A);
	};

	void cpu6502::TYA()
	{
		transfer(Y, A);
	};

	void cpu6502::TSX()
	{
		transfer(S, X);
	};

	void cpu6502::TXS()
	{
		address = PC;
		clock.cycle();

		S = X;
	};

	void cpu6502::push(uint8_t data)
	{
		address = PC;
		clock.cycle();

		address = S;
		bus.write(address, data);
		S--;
		clock.cycle();
	}

	void cpu6502::PHA()
	{
		push(A);
	};

	void cpu6502::PHP()
	{
		push(std::bit_cast<uint8_t>(P));
	};

	uint8_t cpu6502::pull()
	{
		address = PC;
		clock.cycle();

		address = S;
		S++;
		clock.cycle();

		address = S;
		uint8_t data = bus.read(address);
		clock.cycle();

		return data;
	}

	void cpu6502::PLA()
	{
		A = pull();
	};

	void cpu6502::PLP()
	{
		P = std::bit_cast<status>(pull());
	};

	void cpu6502::AND()
	{
		A = A & load();
		P.Z = is_zero(A);
		P.N = is_negative(A);
	};

	void cpu6502::EOR()
	{
		A = A ^ load();
		P.Z = is_zero(A);
		P.N = is_negative(A);
	};

	void cpu6502::ORA()
	{
		A = A | load();
		P.Z = is_zero(A);
		P.N = is_negative(A);
	};

	void cpu6502::BIT()
	{
		uint8_t data = load();
		P.Z = is_zero(A & data);
		P.N = is_negative(data);
		P.V = (data & (1 << 6)) != 0;
	};

	void cpu6502::add_or_substract(uint8_t data)
	{
		uint16_t result = A + data + P.C;

		P.C = result > 0xFF;
		P.V = ((A ^ static_cast<uint8_t>(result)) &
			(data ^ static_cast<uint8_t>(result)) & 0x80) != 0;

		A = static_cast<uint8_t>(result);
		P.Z = is_zero(A);
		P.N = is_negative(A);
	}

	void cpu6502::ADC()
	{
		add_or_substract(load());
	};

	void cpu6502::SBC()
	{
		add_or_substract(~load());
	};

	void cpu6502::compare(uint8_t lhs)
	{
		uint8_t rhs = load();
		P.C = lhs >= rhs;
		P.Z = is_zero(lhs - rhs);
		P.N = is_negative(lhs - rhs);
	}

	void cpu6502::CMP()
	{
		compare(A);
	};

	void cpu6502::CPX()
	{
		compare(X);
	};

	void cpu6502::CPY()
	{
		compare(Y);
	};

	void cpu6502::increment(uint8_t & reg)
	{
		reg++;
		P.Z = is_zero(reg);
		P.N = is_negative(reg);
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
		P.Z = is_zero(reg);
		P.N = is_negative(reg);
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
		auto shift = [&](auto & reg)
		{
			P.C = reg & (1 << 7);
			reg = reg << 1;
			P.Z = is_zero(reg);
			P.N = is_negative(reg);
		};

		f(shift, acc_addressing);
	};

	void cpu6502::LSR()
	{
		auto shift = [&](auto & reg)
		{
			P.C = reg & 1;
			reg = 1 >> reg;
			P.Z = is_zero(reg);
			P.N = is_negative(reg);
		};

		f(shift, acc_addressing);
	};

	void cpu6502::ROL()
	{
		auto shift = [&](auto & reg)
		{
			bool is_carry = reg & (1 << 7);
			reg = reg << 1;
			reg = reg | P.C;
			P.C = is_carry;
			P.Z = is_zero(reg);
			P.N = is_negative(reg);
		};

		f(shift, acc_addressing);
	};

	void cpu6502::ROR()
	{
		auto shift = [&](auto & reg)
		{
			bool is_carry = reg & 1;
			reg = 1 >> reg;
			reg = reg | (P.C << 7);
			P.C = is_carry;
			P.Z = is_zero(reg);
			P.N = is_negative(reg);
		};

		f(shift, acc_addressing);
	};

	void cpu6502::JMP() {};
	void cpu6502::JSR() {};
	void cpu6502::RTS() {};
}