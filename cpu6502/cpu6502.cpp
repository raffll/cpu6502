#include "cpu6502.h"

namespace emulator
{
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
		byte += offset;
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

	void cpu6502::IND()
	{
		address = PC;
		PC++;
		uint8_t p_lo = bus.read(address);
		clock.cycle();

		address = PC;
		PC++;
		uint8_t p_hi = bus.read(address);
		clock.cycle();

		address = (p_hi << 8) | p_lo;
		uint8_t lo = bus.read(address);
		clock.cycle();

		address = (p_hi << 8) | (p_lo + 1);
		uint8_t hi = bus.read(address);
		clock.cycle();

		address = (hi << 8) | lo;
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
			address += 0x0100;
			data = bus.read(address);
			clock.cycle();
		}

		return data;
	}

	uint8_t cpu6502::load_force_cycle()
	{
		uint8_t data = bus.read(address);
		clock.cycle();

		if (add_carry)
		{
			address += 0x0100;
			data = bus.read(address);
		}

		if (add_cycle)
			clock.cycle();

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
			address += 0x0100;

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

		address = 0x0100 + S;
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

		address = 0x0100 + S;
		S++;
		clock.cycle();

		address = 0x0100 + S;
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

	void cpu6502::INC()
	{
		uint8_t data = load_force_cycle();

		data++;
		clock.cycle();

		bus.write(address, data);
		clock.cycle();

		P.Z = is_zero(data);
		P.N = is_negative(data);
	};

	void cpu6502::increment(uint8_t & reg)
	{
		reg++;
		P.Z = is_zero(reg);
		P.N = is_negative(reg);
	}

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
		uint8_t data = load();
		clock.cycle();

		data--;
		clock.cycle();

		bus.write(address, data);
		clock.cycle();

		P.Z = is_zero(data);
		P.N = is_negative(data);
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
		auto asl = [&](auto & reg)
		{
			P.C = reg & (1 << 7);
			reg = reg << 1;
			P.Z = is_zero(reg);
			P.N = is_negative(reg);
		};

		shift(asl, acc_addressing);
	};

	void cpu6502::LSR()
	{
		auto lsr = [&](auto & reg)
		{
			P.C = reg & 1;
			reg = 1 >> reg;
			P.Z = is_zero(reg);
			P.N = is_negative(reg);
		};

		shift(lsr, acc_addressing);
	};

	void cpu6502::ROL()
	{
		auto rol = [&](auto & reg)
		{
			bool is_carry = reg & (1 << 7);
			reg = reg << 1;
			reg = reg | P.C;
			P.C = is_carry;
			P.Z = is_zero(reg);
			P.N = is_negative(reg);
		};

		shift(rol, acc_addressing);
	};

	void cpu6502::ROR()
	{
		auto ror = [&](auto & reg)
		{
			bool is_carry = reg & 1;
			reg = 1 >> reg;
			reg = reg | (P.C << 7);
			P.C = is_carry;
			P.Z = is_zero(reg);
			P.N = is_negative(reg);
		};

		shift(ror, acc_addressing);
	};

	void cpu6502::JMP()
	{
		PC = address;
	};

	static uint8_t lo_byte(uint16_t word)
	{
		return word & 0xFF;
	}

	static uint8_t hi_byte(uint16_t word)
	{
		return (word >> 8) & 0xFF;
	}

	void cpu6502::JSR()
	{
		address = S;
		bus.write(address, hi_byte(PC));
		S--;
		clock.cycle();

		address = S;
		bus.write(address, lo_byte(PC));
		S--;
		clock.cycle();
	};

	void cpu6502::RTS()
	{

	};
}