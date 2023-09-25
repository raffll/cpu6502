#pragma once

#include <cstdint>
#include <array>
#include <functional>
#include <string>

#include "clock.h"
#include "bus.h"

namespace cpu6502
{
	class cpu6502_t
	{
	public:
		cpu6502_t(i_clock & clock, i_bus & bus);

		i_clock & clock;
		i_bus & bus;

		uint8_t A {}; // accumulator
		uint8_t X {}; // x register
		uint8_t Y {}; // y register

		uint16_t PC {}; // program counter

		uint8_t S = {}; // stack pointer
		uint8_t P {}; // processor status register

		uint8_t buffer {};

		void reset();
		void execute();
		uint8_t fetch_byte();
		uint8_t read_byte(uint16_t address);

		uint8_t add_register(uint8_t address, uint8_t reg);
		uint16_t add_register(uint16_t address, uint8_t reg);

		// adressing modes
		void implicit() {}
		void accumulator() {}
		void immediate();
		void zero_page();
		void zero_page_x();
		void zero_page_y() {};
		void relative() {};
		void absolute();
		void absolute_x();
		void absolute_y();
		void indirect() {};
		void indirect_x();
		void indirect_y() {};

		// load/store operations
		void LDA();
		void LDX();
		void LDY();
		void STA();
		void STX();
		void STY();

		void TAX() {};
		void TAY() {};
		void TXA() {};
		void TYA() {};

		void TSX() {};
		void TXS() {};
		void PHA() {};
		void PHP() {};
		void PLA() {};
		void PLP() {};

		void AND() {};
		void EOR() {};
		void ORA() {};
		void BIT() {};

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
		void XXX() {};

		struct instruction_t
		{
			std::string name;
			std::function<void(cpu6502_t &)> operation;
			std::function<void(cpu6502_t &)> addressing;
			uint8_t cycles {};
		};

		static const std::array<instruction_t, 256> instructions;

	};
}