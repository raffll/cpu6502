#include "cpu6502.h"

namespace emulator
{
	const std::vector<cpu6502::instruction_t> cpu6502::instructions
	{
		{ 0x00, "BRK", &cpu6502::BRK, &cpu6502::___, 7 },
		{ 0x01, "ORA", &cpu6502::ORA, &cpu6502::IDX, 6 },
		{ 0x02, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x03, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x04, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x05, "ORA", &cpu6502::ORA, &cpu6502::ZPG, 3 },
		{ 0x06, "ASL", &cpu6502::ASL, &cpu6502::ZPG, 5 },
		{ 0x07, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x08, "PHP", &cpu6502::PHP, &cpu6502::___, 3 },
		{ 0x09, "ORA", &cpu6502::ORA, &cpu6502::IMM, 2 },
		{ 0x0A, "ASL", &cpu6502::ASL, &cpu6502::___, 2 },
		{ 0x0B, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x0C, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x0D, "ORA", &cpu6502::ORA, &cpu6502::ABS, 4 },
		{ 0x0E, "ASL", &cpu6502::ASL, &cpu6502::ABS, 6 },
		{ 0x0F, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x10, "BPL", &cpu6502::BPL, &cpu6502::REL, 2 },
		{ 0x11, "ORA", &cpu6502::ORA, &cpu6502::IDY, 5 },
		{ 0x12, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x13, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x14, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x15, "ORA", &cpu6502::ORA, &cpu6502::ZPX, 4 },
		{ 0x16, "ASL", &cpu6502::ASL, &cpu6502::ZPX, 6 },
		{ 0x17, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x18, "CLC", &cpu6502::CLC, &cpu6502::___, 2 },
		{ 0x19, "ORA", &cpu6502::ORA, &cpu6502::ABY, 4 },
		{ 0x1A, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x1B, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x1C, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x1D, "ORA", &cpu6502::ORA, &cpu6502::ABX, 4 },
		{ 0x1E, "ASL", &cpu6502::ASL, &cpu6502::ABX, 7 },
		{ 0x1F, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0x20, "JSR", &cpu6502::JSR, &cpu6502::ABS, 6 },
		{ 0x21, "AND", &cpu6502::AND, &cpu6502::IDX, 6 },
		{ 0x22, "___", &cpu6502::___, &cpu6502::___, 2 },
		{ 0x23, "___", &cpu6502::___, &cpu6502::___, 8 },
		{ 0x24, "BIT", &cpu6502::BIT, &cpu6502::ZPG, 3 },
		{ 0x25, "AND", &cpu6502::AND, &cpu6502::ZPG, 3 },
		{ 0x26, "ROL", &cpu6502::ROL, &cpu6502::ZPG, 5 },
		{ 0x27, "___", &cpu6502::___, &cpu6502::___, 5 },
		{ 0x28, "PLP", &cpu6502::PLP, &cpu6502::___, 4 },
		{ 0x29, "AND", &cpu6502::AND, &cpu6502::IMM, 2 },
		{ 0x2A, "ROL", &cpu6502::ROL, &cpu6502::___, 2 },
		{ 0x2B, "___", &cpu6502::___, &cpu6502::___, 2 },
		{ 0x2C, "BIT", &cpu6502::BIT, &cpu6502::ABS, 4 },
		{ 0x2D, "AND", &cpu6502::AND, &cpu6502::ABS, 4 },
		{ 0x2E, "ROL", &cpu6502::ROL, &cpu6502::ABS, 6 },
		{ 0x2F, "___", &cpu6502::___, &cpu6502::___, 6 },
		{ 0x30, "BMI", &cpu6502::BMI, &cpu6502::REL, 2 },
		{ 0x31, "AND", &cpu6502::AND, &cpu6502::IDY, 5 },
		{ 0x32, "___", &cpu6502::___, &cpu6502::___, 2 },
		{ 0x33, "___", &cpu6502::___, &cpu6502::___, 8 },
		{ 0x34, "___", &cpu6502::NOP, &cpu6502::___, 4 },
		{ 0x35, "AND", &cpu6502::AND, &cpu6502::ZPX, 4 },
		{ 0x36, "ROL", &cpu6502::ROL, &cpu6502::ZPX, 6 },
		{ 0x37, "___", &cpu6502::___, &cpu6502::___, 6 },
		{ 0x38, "SEC", &cpu6502::SEC, &cpu6502::___, 2 },
		{ 0x39, "AND", &cpu6502::AND, &cpu6502::ABY, 4 },
		{ 0x3A, "___", &cpu6502::NOP, &cpu6502::___, 2 },
		{ 0x3B, "___", &cpu6502::___, &cpu6502::___, 7 },
		{ 0x3C, "___", &cpu6502::NOP, &cpu6502::___, 4 },
		{ 0x3D, "AND", &cpu6502::AND, &cpu6502::ABX, 4 },
		{ 0x3E, "ROL", &cpu6502::ROL, &cpu6502::ABX, 7 },
		{ 0x3F, "___", &cpu6502::___, &cpu6502::___, 7 },
		{ 0x40, "RTI", &cpu6502::RTI, &cpu6502::___, 6 },
		{ 0x41, "EOR", &cpu6502::EOR, &cpu6502::IDX, 6 },
		{ 0x42, "___", &cpu6502::___, &cpu6502::___, 2 },
		{ 0x43, "___", &cpu6502::___, &cpu6502::___, 8 },
		{ 0x44, "___", &cpu6502::NOP, &cpu6502::___, 3 },
		{ 0x45, "EOR", &cpu6502::EOR, &cpu6502::ZPG, 3 },
		{ 0x46, "LSR", &cpu6502::LSR, &cpu6502::ZPG, 5 },
		{ 0x47, "___", &cpu6502::___, &cpu6502::___, 5 },
		{ 0x48, "PHA", &cpu6502::PHA, &cpu6502::___, 3 },
		{ 0x49, "EOR", &cpu6502::EOR, &cpu6502::IMM, 2 },
		{ 0x4A, "LSR", &cpu6502::LSR, &cpu6502::___, 2 },
		{ 0x4B, "___", &cpu6502::___, &cpu6502::___, 2 },
		{ 0x4C, "JMP", &cpu6502::JMP, &cpu6502::ABS, 3 },
		{ 0x4D, "EOR", &cpu6502::EOR, &cpu6502::ABS, 4 },
		{ 0x4E, "LSR", &cpu6502::LSR, &cpu6502::ABS, 6 },
		{ 0x4F, "___", &cpu6502::___, &cpu6502::___, 6 },
		{ 0x50, "BVC", &cpu6502::BVC, &cpu6502::REL, 2 },
		{ 0x51, "EOR", &cpu6502::EOR, &cpu6502::IDY, 5 },
		{ 0x52, "___", &cpu6502::___, &cpu6502::___, 2 },
		{ 0x53, "___", &cpu6502::___, &cpu6502::___, 8 },
		{ 0x54, "___", &cpu6502::NOP, &cpu6502::___, 4 },
		{ 0x55, "EOR", &cpu6502::EOR, &cpu6502::ZPX, 4 },
		{ 0x56, "LSR", &cpu6502::LSR, &cpu6502::ZPX, 6 },
		{ 0x57, "___", &cpu6502::___, &cpu6502::___, 6 },
		{ 0x58, "CLI", &cpu6502::CLI, &cpu6502::___, 2 },
		{ 0x59, "EOR", &cpu6502::EOR, &cpu6502::ABY, 4 },
		{ 0x5A, "___", &cpu6502::NOP, &cpu6502::___, 2 },
		{ 0x5B, "___", &cpu6502::___, &cpu6502::___, 7 },
		{ 0x5C, "___", &cpu6502::NOP, &cpu6502::___, 4 },
		{ 0x5D, "EOR", &cpu6502::EOR, &cpu6502::ABX, 4 },
		{ 0x5E, "LSR", &cpu6502::LSR, &cpu6502::ABX, 7 },
		{ 0x5F, "___", &cpu6502::___, &cpu6502::___, 7 },
		{ 0x60, "RTS", &cpu6502::RTS, &cpu6502::___, 6 },
		{ 0x61, "ADC", &cpu6502::ADC, &cpu6502::IDX, 6 },
		{ 0x62, "___", &cpu6502::___, &cpu6502::___, 2 },
		{ 0x63, "___", &cpu6502::___, &cpu6502::___, 8 },
		{ 0x64, "___", &cpu6502::NOP, &cpu6502::___, 3 },
		{ 0x65, "ADC", &cpu6502::ADC, &cpu6502::ZPG, 3 },
		{ 0x66, "ROR", &cpu6502::ROR, &cpu6502::ZPG, 5 },
		{ 0x67, "___", &cpu6502::___, &cpu6502::___, 5 },
		{ 0x68, "PLA", &cpu6502::PLA, &cpu6502::___, 4 },
		{ 0x69, "ADC", &cpu6502::ADC, &cpu6502::IMM, 2 },
		{ 0x6A, "ROR", &cpu6502::ROR, &cpu6502::___, 2 },
		{ 0x6B, "___", &cpu6502::___, &cpu6502::___, 2 },
		{ 0x6C, "JMP", &cpu6502::JMP, &cpu6502::IND, 5 },
		{ 0x6D, "ADC", &cpu6502::ADC, &cpu6502::ABS, 4 },
		{ 0x6E, "ROR", &cpu6502::ROR, &cpu6502::ABS, 6 },
		{ 0x6F, "___", &cpu6502::___, &cpu6502::___, 6 },
		{ 0x70, "BVS", &cpu6502::BVS, &cpu6502::REL, 2 },
		{ 0x71, "ADC", &cpu6502::ADC, &cpu6502::IDY, 5 },
		{ 0x72, "___", &cpu6502::___, &cpu6502::___, 2 },
		{ 0x73, "___", &cpu6502::___, &cpu6502::___, 8 },
		{ 0x74, "___", &cpu6502::NOP, &cpu6502::___, 4 },
		{ 0x75, "ADC", &cpu6502::ADC, &cpu6502::ZPX, 4 },
		{ 0x76, "ROR", &cpu6502::ROR, &cpu6502::ZPX, 6 },
		{ 0x77, "___", &cpu6502::___, &cpu6502::___, 6 },
		{ 0x78, "SEI", &cpu6502::SEI, &cpu6502::___, 2 },
		{ 0x79, "ADC", &cpu6502::ADC, &cpu6502::ABY, 4 },
		{ 0x7A, "___", &cpu6502::NOP, &cpu6502::___, 2 },
		{ 0x7B, "___", &cpu6502::___, &cpu6502::___, 7 },
		{ 0x7C, "___", &cpu6502::NOP, &cpu6502::___, 4 },
		{ 0x7D, "ADC", &cpu6502::ADC, &cpu6502::ABX, 4 },
		{ 0x7E, "ROR", &cpu6502::ROR, &cpu6502::ABX, 7 },
		{ 0x7F, "___", &cpu6502::___, &cpu6502::___, 7 },
		{ 0x80, "___", &cpu6502::NOP, &cpu6502::___, 2 },
		{ 0x81, "STA", &cpu6502::STA, &cpu6502::IDX, 6 },
		{ 0x82, "___", &cpu6502::NOP, &cpu6502::___, 2 },
		{ 0x83, "___", &cpu6502::___, &cpu6502::___, 6 },
		{ 0x84, "STY", &cpu6502::STY, &cpu6502::ZPG, 3 },
		{ 0x85, "STA", &cpu6502::STA, &cpu6502::ZPG, 3 },
		{ 0x86, "STX", &cpu6502::STX, &cpu6502::ZPG, 3 },
		{ 0x87, "___", &cpu6502::___, &cpu6502::___, 3 },
		{ 0x88, "DEY", &cpu6502::DEY, &cpu6502::___, 2 },
		{ 0x89, "___", &cpu6502::NOP, &cpu6502::___, 2 },
		{ 0x8A, "TXA", &cpu6502::TXA, &cpu6502::___, 2 },
		{ 0x8B, "___", &cpu6502::___, &cpu6502::___, 2 },
		{ 0x8C, "STY", &cpu6502::STY, &cpu6502::ABS, 4 },
		{ 0x8D, "STA", &cpu6502::STA, &cpu6502::ABS, 4 },
		{ 0x8E, "STX", &cpu6502::STX, &cpu6502::ABS, 4 },
		{ 0x8F, "___", &cpu6502::___, &cpu6502::___, 4 },
		{ 0x90, "BCC", &cpu6502::BCC, &cpu6502::REL, 2 },
		{ 0x91, "STA", &cpu6502::STA, &cpu6502::IDY, 6 },
		{ 0x92, "___", &cpu6502::___, &cpu6502::___, 2 },
		{ 0x93, "___", &cpu6502::___, &cpu6502::___, 6 },
		{ 0x94, "STY", &cpu6502::STY, &cpu6502::ZPX, 4 },
		{ 0x95, "STA", &cpu6502::STA, &cpu6502::ZPX, 4 },
		{ 0x96, "STX", &cpu6502::STX, &cpu6502::ZPY, 4 },
		{ 0x97, "___", &cpu6502::___, &cpu6502::___, 4 },
		{ 0x98, "TYA", &cpu6502::TYA, &cpu6502::___, 2 },
		{ 0x99, "STA", &cpu6502::STA, &cpu6502::ABY, 5 },
		{ 0x9A, "TXS", &cpu6502::TXS, &cpu6502::___, 2 },
		{ 0x9B, "___", &cpu6502::___, &cpu6502::___, 5 },
		{ 0x9C, "___", &cpu6502::NOP, &cpu6502::___, 5 },
		{ 0x9D, "STA", &cpu6502::STA, &cpu6502::ABX, 5 },
		{ 0x9E, "___", &cpu6502::___, &cpu6502::___, 5 },
		{ 0x9F, "___", &cpu6502::___, &cpu6502::___, 5 },
		{ 0xA0, "LDY", &cpu6502::LDY, &cpu6502::IMM, 2 },
		{ 0xA1, "LDA", &cpu6502::LDA, &cpu6502::IDX, 6 },
		{ 0xA2, "LDX", &cpu6502::LDX, &cpu6502::IMM, 2 },
		{ 0xA3, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xA4, "LDY", &cpu6502::LDY, &cpu6502::ZPG, 3 },
		{ 0xA5, "LDA", &cpu6502::LDA, &cpu6502::ZPG, 3 },
		{ 0xA6, "LDX", &cpu6502::LDX, &cpu6502::ZPG, 3 },
		{ 0xA7, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xA8, "TAY", &cpu6502::TAY, &cpu6502::___, 2 },
		{ 0xA9, "LDA", &cpu6502::LDA, &cpu6502::IMM, 2 },
		{ 0xAA, "TAX", &cpu6502::TAX, &cpu6502::___, 2 },
		{ 0xAB, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xAC, "LDY", &cpu6502::LDY, &cpu6502::ABS, 4 },
		{ 0xAD, "LDA", &cpu6502::LDA, &cpu6502::ABS, 4 },
		{ 0xAE, "LDX", &cpu6502::LDX, &cpu6502::ABS, 4 },
		{ 0xAF, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xB0, "BCS", &cpu6502::BCS, &cpu6502::REL, 2 },
		{ 0xB1, "LDA", &cpu6502::LDA, &cpu6502::IDY, 5 },
		{ 0xB2, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xB3, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xB4, "LDY", &cpu6502::LDY, &cpu6502::ZPX, 4 },
		{ 0xB5, "LDA", &cpu6502::LDA, &cpu6502::ZPX, 4 },
		{ 0xB6, "LDX", &cpu6502::LDX, &cpu6502::ZPY, 4 },
		{ 0xB7, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xB8, "CLV", &cpu6502::CLV, &cpu6502::___, 2 },
		{ 0xB9, "LDA", &cpu6502::LDA, &cpu6502::ABY, 4 },
		{ 0xBA, "TSX", &cpu6502::TSX, &cpu6502::___, 2 },
		{ 0xBB, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xBC, "LDY", &cpu6502::LDY, &cpu6502::ABX, 4 },
		{ 0xBD, "LDA", &cpu6502::LDA, &cpu6502::ABX, 4 },
		{ 0xBE, "LDX", &cpu6502::LDX, &cpu6502::ABY, 4 },
		{ 0xBF, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xC0, "CPY", &cpu6502::CPY, &cpu6502::IMM, 2 },
		{ 0xC1, "CMP", &cpu6502::CMP, &cpu6502::IDX, 6 },
		{ 0xC2, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xC3, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xC4, "CPY", &cpu6502::CPY, &cpu6502::ZPG, 3 },
		{ 0xC5, "CMP", &cpu6502::CMP, &cpu6502::ZPG, 3 },
		{ 0xC6, "DEC", &cpu6502::DEC, &cpu6502::ZPG, 5 },
		{ 0xC7, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xC8, "INY", &cpu6502::INY, &cpu6502::___, 2 },
		{ 0xC9, "CMP", &cpu6502::CMP, &cpu6502::IMM, 2 },
		{ 0xCA, "DEX", &cpu6502::DEX, &cpu6502::___, 2 },
		{ 0xCB, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xCC, "CPY", &cpu6502::CPY, &cpu6502::ABS, 4 },
		{ 0xCD, "CMP", &cpu6502::CMP, &cpu6502::ABS, 4 },
		{ 0xCE, "DEC", &cpu6502::DEC, &cpu6502::ABS, 6 },
		{ 0xCF, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xD0, "BNE", &cpu6502::BNE, &cpu6502::REL, 2 },
		{ 0xD1, "CMP", &cpu6502::CMP, &cpu6502::IDY, 5 },
		{ 0xD2, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xD3, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xD4, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xD5, "CMP", &cpu6502::CMP, &cpu6502::ZPX, 4 },
		{ 0xD6, "DEC", &cpu6502::DEC, &cpu6502::ZPX, 6 },
		{ 0xD7, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xD8, "CLD", &cpu6502::CLD, &cpu6502::___, 2 },
		{ 0xD9, "CMP", &cpu6502::CMP, &cpu6502::ABY, 4 },
		{ 0xDA, "NOP", &cpu6502::NOP, &cpu6502::___, 2 },
		{ 0xDB, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xDC, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xDD, "CMP", &cpu6502::CMP, &cpu6502::ABX, 4 },
		{ 0xDE, "DEC", &cpu6502::DEC, &cpu6502::ABX, 7 },
		{ 0xDF, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xE0, "CPX", &cpu6502::CPX, &cpu6502::IMM, 2 },
		{ 0xE1, "SBC", &cpu6502::SBC, &cpu6502::IDX, 6 },
		{ 0xE2, "___", &cpu6502::NOP, &cpu6502::___, 2 },
		{ 0xE3, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xE4, "CPX", &cpu6502::CPX, &cpu6502::ZPG, 3 },
		{ 0xE5, "SBC", &cpu6502::SBC, &cpu6502::ZPG, 3 },
		{ 0xE6, "INC", &cpu6502::INC, &cpu6502::ZPG, 5 },
		{ 0xE7, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xE8, "INX", &cpu6502::INX, &cpu6502::___, 2 },
		{ 0xE9, "SBC", &cpu6502::SBC, &cpu6502::IMM, 2 },
		{ 0xEA, "NOP", &cpu6502::NOP, &cpu6502::___, 2 },
		{ 0xEB, "___", &cpu6502::SBC, &cpu6502::___, 2 },
		{ 0xEC, "CPX", &cpu6502::CPX, &cpu6502::ABS, 4 },
		{ 0xED, "SBC", &cpu6502::SBC, &cpu6502::ABS, 4 },
		{ 0xEE, "INC", &cpu6502::INC, &cpu6502::ABS, 6 },
		{ 0xEF, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xF0, "BEQ", &cpu6502::BEQ, &cpu6502::REL, 2 },
		{ 0xF1, "SBC", &cpu6502::SBC, &cpu6502::IDY, 5 },
		{ 0xF2, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xF3, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xF4, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xF5, "SBC", &cpu6502::SBC, &cpu6502::ZPX, 4 },
		{ 0xF6, "INC", &cpu6502::INC, &cpu6502::ZPX, 6 },
		{ 0xF7, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xF8, "SED", &cpu6502::SED, &cpu6502::___, 2 },
		{ 0xF9, "SBC", &cpu6502::SBC, &cpu6502::ABY, 4 },
		{ 0xFA, "NOP", &cpu6502::NOP, &cpu6502::___, 2 },
		{ 0xFB, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xFC, "___", &cpu6502::___, &cpu6502::___, 0 },
		{ 0xFD, "SBC", &cpu6502::SBC, &cpu6502::ABX, 4 },
		{ 0xFE, "INC", &cpu6502::INC, &cpu6502::ABX, 7 },
		{ 0xFF, "___", &cpu6502::___, &cpu6502::___, 0 },
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

	void cpu6502::execute()
	{
		address = PC;
		PC++;
		uint8_t opcode = bus.read(address);
		clock.cycle();

		instructions[opcode].addressing(*this);
		instructions[opcode].operation(*this);
	}

	static bool carry = false;

	uint8_t cpu6502::add_register(
		uint8_t byte,
		uint8_t reg,
		bool do_carry)
	{
		byte = (byte + reg) & 0xFF;
		if (do_carry && byte < reg)
			carry = true;
		return byte;
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

	void cpu6502::ZP(uint8_t reg)
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
		ZP(X);
	}

	void cpu6502::ZPY()
	{
		ZP(Y);
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

	void cpu6502::AB(uint8_t reg)
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
		AB(X);
	}

	void cpu6502::ABY()
	{
		AB(Y);
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

	void cpu6502::LDA()
	{
		uint8_t byte = bus.read(address);
		clock.cycle();

		if (carry)
		{
			address = address + 0x0100;
			byte = bus.read(address);
			clock.cycle();
			carry = false;
		}

		A = byte;
		P.Z = (A == 0);
		P.N = A & (1 << 7);
	}

	void cpu6502::LDX()
	{
		uint8_t byte = bus.read(address);
		clock.cycle();

		if (carry)
		{
			address = address + 0x0100;
			byte = bus.read(address);
			clock.cycle();
			carry = false;
		}

		X = byte;
		P.Z = (X == 0);
		P.N = X & (1 << 7);
	}

	void cpu6502::LDY()
	{
		uint8_t byte = bus.read(address);
		clock.cycle();

		if (carry)
		{
			address = address + 0x0100;
			byte = bus.read(address);
			clock.cycle();
			carry = false;
		}

		Y = byte;
		P.Z = (Y == 0);
		P.N = Y & (1 << 7);
	}

	void cpu6502::STA()
	{
		bus.write(address, A);
		clock.cycle();
	}

	void cpu6502::STX()
	{
		bus.write(address, X);
		clock.cycle();
	}

	void cpu6502::STY()
	{
		bus.write(address, Y);
		clock.cycle();
	}

	void cpu6502::TAX()
	{
		address = PC;
		clock.cycle();

		X = A;
		P.Z = (X == 0);
		P.N = X & (1 << 7);
	};

	void cpu6502::TAY()
	{
		address = PC;
		clock.cycle();

		Y = A;
		P.Z = (Y == 0);
		P.N = Y & (1 << 7);
	};

	void cpu6502::TXA() 
	{
		address = PC;
		clock.cycle();

		A = X;
		P.Z = (A == 0);
		P.N = A & (1 << 7);
	};

	void cpu6502::TYA()
	{
		address = PC;
		clock.cycle();

		A = Y;
		P.Z = (A == 0);
		P.N = A & (1 << 7);
	};

	void cpu6502::TSX()
	{
		address = PC;
		clock.cycle();

		X = S;
		P.Z = (X == 0);
		P.N = X & (1 << 7);
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
		S--;
		bus.write(address, A);
		clock.cycle();
	};

	void cpu6502::PHP()
	{
		address = PC;
		clock.cycle();

		address = S;
		bus.write(address, A);
		clock.cycle();
	};

	void cpu6502::PLA()
	{

	};

	void cpu6502::PLP() {};
}