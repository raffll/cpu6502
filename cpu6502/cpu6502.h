#pragma once

#include <cstdint>
#include <array>
#include <functional>
#include <string>
#include <bit>

#include "clock.h"
#include "bus.h"

namespace emulator
{
	class cpu6502
	{
	public:
		cpu6502(i_clock & clock, i_bus & bus);

		i_clock & clock;
		i_bus & bus;

		uint8_t A {};			// accumulator
		uint8_t X {};			// x register
		uint8_t Y {};			// y register

		uint16_t PC {};			// program counter

		uint8_t S = {};			// stack pointer

		struct status
		{
			uint8_t C : 1;		// carry flag
			uint8_t Z : 1;		// zero flag
			uint8_t I : 1;		// interrupt disable
			uint8_t D : 1;		// decimal mode
			uint8_t B : 1;		// break command
			uint8_t _ : 1;
			uint8_t V : 1;		// overflow flag
			uint8_t N : 1;		// negative flag
		};

		status P {};			// processor status register

		uint16_t address {};	// address bus

		void reset();
		void execute();
		uint8_t add_register(
			uint8_t byte,
			uint8_t reg,
			bool do_carry = false);

		// adressing modes
		void IMM();
		void ZPG();
		void ZP(uint8_t reg);
		void ZPX();
		void ZPY();
		void REL() {};
		void ABS();
		void AB(uint8_t reg);
		void ABX();
		void ABY();
		void IND() {};
		void IDX();
		void IDY();

		uint8_t read_byte();

		// load/store operations
		void LDA();
		void LDX();
		void LDY();
		void STA();
		void STX();
		void STY();

		// register transfers
		void TAX();
		void TAY();
		void TXA();
		void TYA();

		// stack operations
		void TSX();
		void TXS();
		void PHA();
		void PHP();
		void PLA();
		void PLP();

		void AND();
		void EOR();
		void ORA();
		void BIT();

		void ADC() {};
		void SBC() {};
		void CMP() {};
		void CPX() {};
		void CPY() {};

		void INC() {};
		void INX() {};
		void INY() {};
		void DEC() {};
		void DEX() {};
		void DEY() {};

		void ASL() {};
		void LSR() {};
		void ROL() {};
		void ROR() {};

		void JMP() {};
		void JSR() {};
		void RTS() {};

		void BCC() {};
		void BCS() {};
		void BEQ() {};
		void BMI() {};
		void BNE() {};
		void BPL() {};
		void BVC() {};
		void BVS() {};

		void CLC() {};
		void CLD() {};
		void CLI() {};
		void CLV() {};
		void SEC() {};
		void SED() {};
		void SEI() {};

		void BRK() {};
		void NOP() {};
		void RTI() {};

		void ___() {};

		struct instruction_t
		{
			uint8_t oppcode {};
			std::string name;
			std::function<void(cpu6502 &)> operation;
			std::function<void(cpu6502 &)> addressing;
			uint8_t cycles {};
		};

		static const std::vector<instruction_t> instructions;
	};
}