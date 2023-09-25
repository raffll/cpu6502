#include "cpu6502.h"

namespace cpu6502
{
	using a = cpu6502_t;

	const std::array<cpu6502_t::instruction_t, 256> cpu6502_t::instructions
	{ {
		{ "BRK", &a::BRK, &a::immediate, 7 },
		{ "ORA", &a::ORA, &a::indirect_x, 6 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "???", &a::NOP, &a::implicit, 3 },
		{ "ORA", &a::ORA, &a::zero_page, 3 },
		{ "ASL", &a::ASL, &a::zero_page, 5 },
		{ "???", &a::XXX, &a::implicit, 5 },
		{ "PHP", &a::PHP, &a::implicit, 3 },
		{ "ORA", &a::ORA, &a::immediate, 2 },
		{ "ASL", &a::ASL, &a::accumulator, 2 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "ORA", &a::ORA, &a::absolute, 4 },
		{ "ASL", &a::ASL, &a::absolute, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },

		{ "BPL", &a::BPL, &a::relative, 2 },
		{ "ORA", &a::ORA, &a::indirect_y, 5 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "ORA", &a::ORA, &a::zero_page_x, 4 },
		{ "ASL", &a::ASL, &a::zero_page_x, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },
		{ "CLC", &a::CLC, &a::implicit, 2 },
		{ "ORA", &a::ORA, &a::absolute_y, 4 },
		{ "???", &a::NOP, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 7 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "ORA", &a::ORA, &a::absolute_x, 4 },
		{ "ASL", &a::ASL, &a::absolute_x, 7 },
		{ "???", &a::XXX, &a::implicit, 7 },

		{ "JSR", &a::JSR, &a::absolute, 6 },
		{ "AND", &a::AND, &a::indirect_x, 6 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "BIT", &a::BIT, &a::zero_page, 3 },
		{ "AND", &a::AND, &a::zero_page, 3 },
		{ "ROL", &a::ROL, &a::zero_page, 5 },
		{ "???", &a::XXX, &a::implicit, 5 },
		{ "PLP", &a::PLP, &a::implicit, 4 },
		{ "AND", &a::AND, &a::immediate, 2 },
		{ "ROL", &a::ROL, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "BIT", &a::BIT, &a::absolute, 4 },
		{ "AND", &a::AND, &a::absolute, 4 },
		{ "ROL", &a::ROL, &a::absolute, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },

		{ "BMI", &a::BMI, &a::relative, 2 },
		{ "AND", &a::AND, &a::indirect_y, 5 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "AND", &a::AND, &a::zero_page_x, 4 },
		{ "ROL", &a::ROL, &a::zero_page_x, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },
		{ "SEC", &a::SEC, &a::implicit, 2 },
		{ "AND", &a::AND, &a::absolute_y, 4 },
		{ "???", &a::NOP, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 7 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "AND", &a::AND, &a::absolute_x, 4 },
		{ "ROL", &a::ROL, &a::absolute_x, 7 },
		{ "???", &a::XXX, &a::implicit, 7 },

		{ "RTI", &a::RTI, &a::implicit, 6 },
		{ "EOR", &a::EOR, &a::indirect_x, 6 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "???", &a::NOP, &a::implicit, 3 },
		{ "EOR", &a::EOR, &a::zero_page, 3 },
		{ "LSR", &a::LSR, &a::zero_page, 5 },
		{ "???", &a::XXX, &a::implicit, 5 },
		{ "PHA", &a::PHA, &a::implicit, 3 },
		{ "EOR", &a::EOR, &a::immediate, 2 },
		{ "LSR", &a::LSR, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "JMP", &a::JMP, &a::absolute, 3 },
		{ "EOR", &a::EOR, &a::absolute, 4 },
		{ "LSR", &a::LSR, &a::absolute, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },

		{ "BVC", &a::BVC, &a::relative, 2 },
		{ "EOR", &a::EOR, &a::indirect_y, 5 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "EOR", &a::EOR, &a::zero_page_x, 4 },
		{ "LSR", &a::LSR, &a::zero_page_x, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },
		{ "CLI", &a::CLI, &a::implicit, 2 },
		{ "EOR", &a::EOR, &a::absolute_y, 4 },
		{ "???", &a::NOP, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 7 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "EOR", &a::EOR, &a::absolute_x, 4 },
		{ "LSR", &a::LSR, &a::absolute_x, 7 },
		{ "???", &a::XXX, &a::implicit, 7 },

		{ "RTS", &a::RTS, &a::implicit, 6 },
		{ "ADC", &a::ADC, &a::indirect_x, 6 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "???", &a::NOP, &a::implicit, 3 },
		{ "ADC", &a::ADC, &a::zero_page, 3 },
		{ "ROR", &a::ROR, &a::zero_page, 5 },
		{ "???", &a::XXX, &a::implicit, 5 },
		{ "PLA", &a::PLA, &a::implicit, 4 },
		{ "ADC", &a::ADC, &a::immediate, 2 },
		{ "ROR", &a::ROR, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "JMP", &a::JMP, &a::indirect, 5 },
		{ "ADC", &a::ADC, &a::absolute, 4 },
		{ "ROR", &a::ROR, &a::absolute, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },

		{ "BVS", &a::BVS, &a::relative, 2 },
		{ "ADC", &a::ADC, &a::indirect_y, 5 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "ADC", &a::ADC, &a::zero_page_x, 4 },
		{ "ROR", &a::ROR, &a::zero_page_x, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },
		{ "SEI", &a::SEI, &a::implicit, 2 },
		{ "ADC", &a::ADC, &a::absolute_y, 4 },
		{ "???", &a::NOP, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 7 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "ADC", &a::ADC, &a::absolute_x, 4 },
		{ "ROR", &a::ROR, &a::absolute_x, 7 },
		{ "???", &a::XXX, &a::implicit, 7 },

		{ "???", &a::NOP, &a::implicit, 2 },
		{ "STA", &a::STA, &a::indirect_x, 6 },
		{ "???", &a::NOP, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 6 },
		{ "STY", &a::STY, &a::zero_page, 3 },
		{ "STA", &a::STA, &a::zero_page, 3 },
		{ "STX", &a::STX, &a::zero_page, 3 },
		{ "???", &a::XXX, &a::implicit, 3 },
		{ "DEY", &a::DEY, &a::implicit, 2 },
		{ "???", &a::NOP, &a::implicit, 2 },
		{ "TXA", &a::TXA, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "STY", &a::STY, &a::absolute, 4 },
		{ "STA", &a::STA, &a::absolute, 4 },
		{ "STX", &a::STX, &a::absolute, 4 },
		{ "???", &a::XXX, &a::implicit, 4 },

		{ "BCC", &a::BCC, &a::relative, 2 },
		{ "STA", &a::STA, &a::indirect_y, 6 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 6 },
		{ "STY", &a::STY, &a::zero_page_x, 4 },
		{ "STA", &a::STA, &a::zero_page_x, 4 },
		{ "STX", &a::STX, &a::zero_page_y, 4 },
		{ "???", &a::XXX, &a::implicit, 4 },
		{ "TYA", &a::TYA, &a::implicit, 2 },
		{ "STA", &a::STA, &a::absolute_y, 5 },
		{ "TXS", &a::TXS, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 5 },
		{ "???", &a::NOP, &a::implicit, 5 },
		{ "STA", &a::STA, &a::absolute_x, 5 },
		{ "???", &a::XXX, &a::implicit, 5 },
		{ "???", &a::XXX, &a::implicit, 5 },

		{ "LDY", &a::LDY, &a::immediate, 2 },
		{ "LDA", &a::LDA, &a::indirect_x, 6 },
		{ "LDX", &a::LDX, &a::immediate, 2 },
		{ "???", &a::XXX, &a::implicit, 6 },
		{ "LDY", &a::LDY, &a::zero_page, 3 },
		{ "LDA", &a::LDA, &a::zero_page, 3 },
		{ "LDX", &a::LDX, &a::zero_page, 3 },
		{ "???", &a::XXX, &a::implicit, 3 },
		{ "TAY", &a::TAY, &a::implicit, 2 },
		{ "LDA", &a::LDA, &a::immediate, 2 },
		{ "TAX", &a::TAX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "LDY", &a::LDY, &a::absolute, 4 },
		{ "LDA", &a::LDA, &a::absolute, 4 },
		{ "LDX", &a::LDX, &a::absolute, 4 },
		{ "???", &a::XXX, &a::implicit, 4 },

		{ "BCS", &a::BCS, &a::relative, 2 },
		{ "LDA", &a::LDA, &a::indirect_y, 5 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 5 },
		{ "LDY", &a::LDY, &a::zero_page_x, 4 },
		{ "LDA", &a::LDA, &a::zero_page_x, 4 },
		{ "LDX", &a::LDX, &a::zero_page_y, 4 },
		{ "???", &a::XXX, &a::implicit, 4 },
		{ "CLV", &a::CLV, &a::implicit, 2 },
		{ "LDA", &a::LDA, &a::absolute_y, 4 },
		{ "TSX", &a::TSX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 4 },
		{ "LDY", &a::LDY, &a::absolute_x, 4 },
		{ "LDA", &a::LDA, &a::absolute_x, 4 },
		{ "LDX", &a::LDX, &a::absolute_y, 4 },
		{ "???", &a::XXX, &a::implicit, 4 },

		{ "CPY", &a::CPY, &a::immediate, 2 },
		{ "CMP", &a::CMP, &a::indirect_x, 6 },
		{ "???", &a::NOP, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "CPY", &a::CPY, &a::zero_page, 3 },
		{ "CMP", &a::CMP, &a::zero_page, 3 },
		{ "DEC", &a::DEC, &a::zero_page, 5 },
		{ "???", &a::XXX, &a::implicit, 5 },
		{ "INY", &a::INY, &a::implicit, 2 },
		{ "CMP", &a::CMP, &a::immediate, 2 },
		{ "DEX", &a::DEX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "CPY", &a::CPY, &a::absolute, 4 },
		{ "CMP", &a::CMP, &a::absolute, 4 },
		{ "DEC", &a::DEC, &a::absolute, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },

		{ "BNE", &a::BNE, &a::relative, 2 },
		{ "CMP", &a::CMP, &a::indirect_y, 5 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "CMP", &a::CMP, &a::zero_page_x, 4 },
		{ "DEC", &a::DEC, &a::zero_page_x, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },
		{ "CLD", &a::CLD, &a::implicit, 2 },
		{ "CMP", &a::CMP, &a::absolute_y, 4 },
		{ "NOP", &a::NOP, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 7 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "CMP", &a::CMP, &a::absolute_x, 4 },
		{ "DEC", &a::DEC, &a::absolute_x, 7 },
		{ "???", &a::XXX, &a::implicit, 7 },

		{ "CPX", &a::CPX, &a::immediate, 2 },
		{ "SBC", &a::SBC, &a::indirect_x, 6 },
		{ "???", &a::NOP, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "CPX", &a::CPX, &a::zero_page, 3 },
		{ "SBC", &a::SBC, &a::zero_page, 3 },
		{ "INC", &a::INC, &a::zero_page, 5 },
		{ "???", &a::XXX, &a::implicit, 5 },
		{ "INX", &a::INX, &a::implicit, 2 },
		{ "SBC", &a::SBC, &a::immediate, 2 },
		{ "NOP", &a::NOP, &a::implicit, 2 },
		{ "???", &a::SBC, &a::implicit, 2 },
		{ "CPX", &a::CPX, &a::absolute, 4 },
		{ "SBC", &a::SBC, &a::absolute, 4 },
		{ "INC", &a::INC, &a::absolute, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },

		{ "BEQ", &a::BEQ, &a::relative, 2 },
		{ "SBC", &a::SBC, &a::indirect_y, 5 },
		{ "???", &a::XXX, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 8 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "SBC", &a::SBC, &a::zero_page_x, 4 },
		{ "INC", &a::INC, &a::zero_page_x, 6 },
		{ "???", &a::XXX, &a::implicit, 6 },
		{ "SED", &a::SED, &a::implicit, 2 },
		{ "SBC", &a::SBC, &a::absolute_y, 4 },
		{ "NOP", &a::NOP, &a::implicit, 2 },
		{ "???", &a::XXX, &a::implicit, 7 },
		{ "???", &a::NOP, &a::implicit, 4 },
		{ "SBC", &a::SBC, &a::absolute_x, 4 },
		{ "INC", &a::INC, &a::absolute_x, 7 },
		{ "???", &a::XXX, &a::implicit, 7 },
	} };

	cpu6502_t::cpu6502_t(i_clock & clock, i_bus & bus)
		: clock(clock)
		, bus(bus)
	{

	}

	void cpu6502_t::reset()
	{
		A = {};
		X = {};
		Y = {};

		PC = 0x0200;

		S = 0xFF;
		P = {};

		buffer = {};
	}

	uint8_t cpu6502_t::fetch_byte()
	{
		uint8_t byte = bus.read(PC);
		PC++;
		clock.cycle();
		return byte;
	}

	uint8_t cpu6502_t::read_byte(uint16_t address)
	{
		uint8_t byte = bus.read(address);
		clock.cycle();
		return byte;
	}

	uint8_t cpu6502_t::add_register(uint8_t address, uint8_t reg)
	{
		address += reg;
		clock.cycle();
		return address;
	}

	uint16_t cpu6502_t::add_register(uint16_t address, uint8_t reg)
	{
		address += reg;
		clock.cycle();

		// TODO: +1 if page crossed

		return address;
	}

	void cpu6502_t::execute()
	{
		uint8_t oppcode = fetch_byte();
		instructions[oppcode].addressing(*this);
		instructions[oppcode].operation(*this);
	}

	void cpu6502_t::immediate()
	{
		buffer = fetch_byte();
	}

	void cpu6502_t::zero_page()
	{
		uint8_t address = fetch_byte();
		buffer = read_byte(address);
	}

	void cpu6502_t::zero_page_x()
	{
		uint8_t address = fetch_byte();
		address = add_register(address, X);
		buffer = read_byte(address);
	}

	void cpu6502_t::absolute()
	{
		uint8_t lo = fetch_byte();
		uint8_t hi = fetch_byte();
		uint16_t address = (hi << 8) | lo;
		buffer = read_byte(address);
	}

	void cpu6502_t::absolute_x()
	{
		uint8_t lo = fetch_byte();
		uint8_t hi = fetch_byte();
		uint16_t address = (hi << 8) | lo;
		address = add_register(address, X);
		buffer = read_byte(address);
	}

	void cpu6502_t::absolute_y()
	{
		uint8_t lo = fetch_byte();
		uint8_t hi = fetch_byte();
		uint16_t address = (hi << 8) | lo;
		address = add_register(address, Y);
		buffer = read_byte(address);
	}

	void cpu6502_t::indirect_x()
	{
		uint8_t address = fetch_byte();
		uint8_t lo = read_byte(address);
		uint8_t hi = read_byte(address++);
		lo = add_register(lo, X);
		address = (hi << 8) | lo;
		buffer = read_byte(address);
	}

	void cpu6502_t::LDA()
	{
		A = buffer;
	}

	void cpu6502_t::LDX()
	{
		X = buffer;
	}

	void cpu6502_t::LDY()
	{
		Y = buffer;
	}

	void cpu6502_t::STA()
	{
		
	}

	void cpu6502_t::STX()
	{
		
	}

	void cpu6502_t::STY()
	{
		
	}
}