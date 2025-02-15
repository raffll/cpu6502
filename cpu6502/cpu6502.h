#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "i_bus.h"
#include "i_clock.h"
#include "types.h"

namespace emulator {

class cpu6502 {
public:
    cpu6502(i_clock& clock, i_bus& bus);

    i_clock& clock;
    i_bus& bus;

    uint8_t A {}; // accumulator
    uint8_t X {}; // X register
    uint8_t Y {}; // Y register

    uint16_t PC {}; // program counter

    uint8_t S {}; // stack pointer

    struct status {
        uint8_t C : 1; // carry flag
        uint8_t Z : 1; // zero flag
        uint8_t I : 1; // interrupt disable
        uint8_t D : 1; // decimal mode
        uint8_t B : 1; // break command
        uint8_t _ : 1;
        uint8_t V : 1; // overflow flag
        uint8_t N : 1; // negative flag
    };

    status P {}; // processor status register

    uint16_t address {}; // address bus

    void reset();
    void execute();
    void run();
    void cycle();
    void log();

private:
    // adressing modes
    void ACC(); // accumulator
    void IMP(); // implied
    void IMM(); // immediate
    void ZPG(); // zero page
    void ZPX(); // zero page, X
    void ZPY(); // zero page, Y
    void ABS(); // absolute
    void ABX(); // absolute, X
    void ABY(); // absolute, Y
    void IND(); // indirect
    void IDX(); // indexed indirect
    void IDY(); // indirect indexed
    void REL() {}; // relative

public:
    // load operations
    void LDA();
    void LDX();
    void LDY();

    // store operations
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

    // logical
    void AND();
    void EOR();
    void ORA();
    void BIT();

    // arithmetic
    void ADC();
    void SBC();
    void CMP();
    void CPX();
    void CPY();

    // increments & decrements
    void INC();
    void INX();
    void INY();
    void DEC();
    void DEX();
    void DEY();

    // shifts
    void ASL();
    void LSR();
    void ROL();
    void ROR();

    // jumps & calls
    void JMP();
    void JSR();
    void RTS();

    // branches
    void BCC();
    void BCS();
    void BNE();
    void BEQ();
    void BPL();
    void BMI();
    void BVC();
    void BVS();

    // status flag changes
    void CLC();
    void CLD();
    void CLI();
    void CLV();
    void SEC();
    void SED();
    void SEI();

    // system functions
    void BRK();
    void RTI();
    void NOP() {};

    void ___() {};

    enum opcode : uint8_t {
        BRK_IMP = 0x00,
        ORA_IDX = 0x01,
        _0x02__ = 0x02,
        _0x03__ = 0x03,
        _0x04__ = 0x04,
        ORA_ZPG = 0x05,
        ASL_ZPG = 0x06,
        _0x07__ = 0x07,
        PHP_IMP = 0x08,
        ORA_IMM = 0x09,
        ASL_ACC = 0x0A,
        _0x0B__ = 0x0B,
        _0x0C__ = 0x0C,
        ORA_ABS = 0x0D,
        ASL_ABS = 0x0E,
        _0x0F__ = 0x0F,
        BPL_REL = 0x10,
        ORA_IDY = 0x11,
        _0x12__ = 0x12,
        _0x13__ = 0x13,
        _0x14__ = 0x14,
        ORA_ZPX = 0x15,
        ASL_ZPX = 0x16,
        _0x17__ = 0x17,
        CLC____ = 0x18,
        ORA_ABY = 0x19,
        _0x1A__ = 0x1A,
        _0x1B__ = 0x1B,
        _0x1C__ = 0x1C,
        ORA_ABX = 0x1D,
        ASL_ABX = 0x1E,
        _0x1F__ = 0x1F,
        JSR_ABS = 0x20,
        AND_IDX = 0x21,
        _0x22__ = 0x22,
        _0x23__ = 0x23,
        BIT_ZPG = 0x24,
        AND_ZPG = 0x25,
        ROL_ZPG = 0x26,
        _0x27__ = 0x27,
        PLP_IMP = 0x28,
        AND_IMM = 0x29,
        ROL_ACC = 0x2A,
        _0x2B__ = 0x2B,
        BIT_ABS = 0x2C,
        AND_ABS = 0x2D,
        ROL_ABS = 0x2E,
        _0x2F__ = 0x2F,
        BMI_REL = 0x30,
        AND_IDY = 0x31,
        _0x32__ = 0x32,
        _0x33__ = 0x33,
        _0x34__ = 0x34,
        AND_ZPX = 0x35,
        ROL_ZPX = 0x36,
        _0x37__ = 0x37,
        SEC_IMP = 0x38,
        AND_ABY = 0x39,
        _0x3A__ = 0x3A,
        _0x3B__ = 0x3B,
        _0x3C__ = 0x3C,
        AND_ABX = 0x3D,
        ROL_ABX = 0x3E,
        _0x3F__ = 0x3F,
        RTI_IMP = 0x40,
        EOR_IDX = 0x41,
        _0x42__ = 0x42,
        _0x43__ = 0x43,
        _0x44__ = 0x44,
        EOR_ZPG = 0x45,
        LSR_ZPG = 0x46,
        _0x47__ = 0x47,
        PHA_IMP = 0x48,
        EOR_IMM = 0x49,
        LSR_ACC = 0x4A,
        _0x4B__ = 0x4B,
        JMP_ABS = 0x4C,
        EOR_ABS = 0x4D,
        LSR_ABS = 0x4E,
        _0x4F__ = 0x4F,
        BVC_REL = 0x50,
        EOR_IDY = 0x51,
        _0x52__ = 0x52,
        _0x53__ = 0x53,
        _0x54__ = 0x54,
        EOR_ZPX = 0x55,
        LSR_ZPX = 0x56,
        _0x57__ = 0x57,
        CLI_IMP = 0x58,
        EOR_ABY = 0x59,
        _0x5A__ = 0x5A,
        _0x5B__ = 0x5B,
        _0x5C__ = 0x5C,
        EOR_ABX = 0x5D,
        LSR_ABX = 0x5E,
        _0x5F__ = 0x5F,
        RTS_IMP = 0x60,
        ADC_IDX = 0x61,
        _0x62__ = 0x62,
        _0x63__ = 0x63,
        _0x64__ = 0x64,
        ADC_ZPG = 0x65,
        ROR_ZPG = 0x66,
        _0x67__ = 0x67,
        PLA_IMP = 0x68,
        ADC_IMM = 0x69,
        ROR_ACC = 0x6A,
        _0x6B__ = 0x6B,
        JMP_IND = 0x6C,
        ADC_ABS = 0x6D,
        ROR_ABS = 0x6E,
        _0x6F__ = 0x6F,
        BVS_REL = 0x70,
        ADC_IDY = 0x71,
        _0x72__ = 0x72,
        _0x73__ = 0x73,
        _0x74__ = 0x74,
        ADC_ZPX = 0x75,
        ROR_ZPX = 0x76,
        _0x77__ = 0x77,
        SEI_IMP = 0x78,
        ADC_ABY = 0x79,
        _0x7A__ = 0x7A,
        _0x7B__ = 0x7B,
        _0x7C__ = 0x7C,
        ADC_ABX = 0x7D,
        ROR_ABX = 0x7E,
        _0x7F__ = 0x7F,
        _0x80__ = 0x80,
        STA_IDX = 0x81,
        _0x82__ = 0x82,
        _0x83__ = 0x83,
        STY_ZPG = 0x84,
        STA_ZPG = 0x85,
        STX_ZPG = 0x86,
        _0x87__ = 0x87,
        DEY_IMP = 0x88,
        _0x89__ = 0x89,
        TXA_IMP = 0x8A,
        _0x8B__ = 0x8B,
        STY_ABS = 0x8C,
        STA_ABS = 0x8D,
        STX_ABS = 0x8E,
        _0x8F__ = 0x8F,
        BCC_REL = 0x90,
        STA_IDY = 0x91,
        _0x92__ = 0x92,
        _0x93__ = 0x93,
        STY_ZPX = 0x94,
        STA_ZPX = 0x95,
        STX_ZPY = 0x96,
        _0x97__ = 0x97,
        TYA_IMP = 0x98,
        STA_ABY = 0x99,
        TXS_IMP = 0x9A,
        _0x9B__ = 0x9B,
        _0x9C__ = 0x9C,
        STA_ABX = 0x9D,
        _0x9E__ = 0x9E,
        _0x9F__ = 0x9F,
        LDY_IMM = 0xA0,
        LDA_IDX = 0xA1,
        LDX_IMM = 0xA2,
        _0xA3__ = 0xA3,
        LDY_ZPG = 0xA4,
        LDA_ZPG = 0xA5,
        LDX_ZPG = 0xA6,
        _0xA7__ = 0xA7,
        TAY_IMP = 0xA8,
        LDA_IMM = 0xA9,
        TAX_IMP = 0xAA,
        _0xAB__ = 0xAB,
        LDY_ABS = 0xAC,
        LDA_ABS = 0xAD,
        LDX_ABS = 0xAE,
        _0xAF__ = 0xAF,
        BCS_REL = 0xB0,
        LDA_IDY = 0xB1,
        _0xB2__ = 0xB2,
        _0xB3__ = 0xB3,
        LDY_ZPX = 0xB4,
        LDA_ZPX = 0xB5,
        LDX_ZPY = 0xB6,
        _0xB7__ = 0xB7,
        CLV_IMP = 0xB8,
        LDA_ABY = 0xB9,
        TSX_IMP = 0xBA,
        _0xBB__ = 0xBB,
        LDY_ABX = 0xBC,
        LDA_ABX = 0xBD,
        LDX_ABY = 0xBE,
        _0xBF__ = 0xBF,
        CPY_IMM = 0xC0,
        CMP_IDX = 0xC1,
        _0xC2__ = 0xC2,
        _0xC3__ = 0xC3,
        CPY_ZPG = 0xC4,
        CMP_ZPG = 0xC5,
        DEC_ZPG = 0xC6,
        _0xC7__ = 0xC7,
        INY_IMP = 0xC8,
        CMP_IMM = 0xC9,
        DEX_IMP = 0xCA,
        _0xCB__ = 0xCB,
        CPY_ABS = 0xCC,
        CMP_ABS = 0xCD,
        DEC_ABS = 0xCE,
        _0xCF__ = 0xCF,
        BNE_REL = 0xD0,
        CMP_IDY = 0xD1,
        _0xD2__ = 0xD2,
        _0xD3__ = 0xD3,
        _0xD4__ = 0xD4,
        CMP_ZPX = 0xD5,
        DEC_ZPX = 0xD6,
        _0xD7__ = 0xD7,
        CLD_IMP = 0xD8,
        CMP_ABY = 0xD9,
        _0xDA__ = 0xDA,
        _0xDB__ = 0xDB,
        _0xDC__ = 0xDC,
        CMP_ABX = 0xDD,
        DEC_ABX = 0xDE,
        _0xDF__ = 0xDF,
        CPX_IMM = 0xE0,
        SBC_IDX = 0xE1,
        _0xE2__ = 0xE2,
        _0xE3__ = 0xE3,
        CPX_ZPG = 0xE4,
        SBC_ZPG = 0xE5,
        INC_ZPG = 0xE6,
        _0xE7__ = 0xE7,
        INX_IMP = 0xE8,
        SBC_IMM = 0xE9,
        NOP____ = 0xEA,
        _0xEB__ = 0xEB,
        CPX_ABS = 0xEC,
        SBC_ABS = 0xED,
        INC_ABS = 0xEE,
        _0xEF__ = 0xEF,
        BEQ_REL = 0xF0,
        SBC_IDY = 0xF1,
        _0xF2__ = 0xF2,
        _0xF3__ = 0xF3,
        _0xF4__ = 0xF4,
        SBC_ZPX = 0xF5,
        INC_ZPX = 0xF6,
        _0xF7__ = 0xF7,
        SED_IMP = 0xF8,
        SBC_ABY = 0xF9,
        _0xFA__ = 0xFA,
        _0xFB__ = 0xFB,
        _0xFC__ = 0xFC,
        SBC_ABX = 0xFD,
        INC_ABX = 0xFE,
        _0xFF__ = 0xFF,
    };

    struct instruction_t {
        opcode op;
        std::function<void(cpu6502&)> operation;
        std::function<void(cpu6502&)> addressing;
        int cycles {};
    };

    const std::array<instruction_t, 256> instructions { {
        { BRK_IMP, &cpu6502::BRK, &cpu6502::IMP, 7 }, // 0x00
        { ORA_IDX, &cpu6502::ORA, &cpu6502::IDX, 6 }, // 0x01
        { _0x02__, &cpu6502::___, &cpu6502::___, 0 }, // 0x02
        { _0x03__, &cpu6502::___, &cpu6502::___, 0 }, // 0x03
        { _0x04__, &cpu6502::___, &cpu6502::___, 0 }, // 0x04
        { ORA_ZPG, &cpu6502::ORA, &cpu6502::ZPG, 3 }, // 0x05
        { ASL_ZPG, &cpu6502::ASL, &cpu6502::ZPG, 5 }, // 0x06
        { _0x07__, &cpu6502::___, &cpu6502::___, 0 }, // 0x07
        { PHP_IMP, &cpu6502::PHP, &cpu6502::IMP, 3 }, // 0x08
        { ORA_IMM, &cpu6502::ORA, &cpu6502::IMM, 2 }, // 0x09
        { ASL_ACC, &cpu6502::ASL, &cpu6502::ACC, 2 }, // 0x0A
        { _0x0B__, &cpu6502::___, &cpu6502::___, 0 }, // 0x0B
        { _0x0C__, &cpu6502::___, &cpu6502::___, 0 }, // 0x0C
        { ORA_ABS, &cpu6502::ORA, &cpu6502::ABS, 4 }, // 0x0D
        { ASL_ABS, &cpu6502::ASL, &cpu6502::ABS, 6 }, // 0x0E
        { _0x0F__, &cpu6502::___, &cpu6502::___, 0 }, // 0x0F
        { BPL_REL, &cpu6502::BPL, &cpu6502::REL, 2 }, // 0x10
        { ORA_IDY, &cpu6502::ORA, &cpu6502::IDY, 5 }, // 0x11
        { _0x12__, &cpu6502::___, &cpu6502::___, 0 }, // 0x12
        { _0x13__, &cpu6502::___, &cpu6502::___, 0 }, // 0x13
        { _0x14__, &cpu6502::___, &cpu6502::___, 0 }, // 0x14
        { ORA_ZPX, &cpu6502::ORA, &cpu6502::ZPX, 4 }, // 0x15
        { ASL_ZPX, &cpu6502::ASL, &cpu6502::ZPX, 6 }, // 0x16
        { _0x17__, &cpu6502::___, &cpu6502::___, 0 }, // 0x17
        { CLC____, &cpu6502::CLC, &cpu6502::IMP, 2 }, // 0x18
        { ORA_ABY, &cpu6502::ORA, &cpu6502::ABY, 4 }, // 0x19
        { _0x1A__, &cpu6502::___, &cpu6502::___, 0 }, // 0x1A
        { _0x1B__, &cpu6502::___, &cpu6502::___, 0 }, // 0x1B
        { _0x1C__, &cpu6502::___, &cpu6502::___, 0 }, // 0x1C
        { ORA_ABX, &cpu6502::ORA, &cpu6502::ABX, 4 }, // 0x1D
        { ASL_ABX, &cpu6502::ASL, &cpu6502::ABX, 7 }, // 0x1E
        { _0x1F__, &cpu6502::___, &cpu6502::___, 0 }, // 0x1F
        { JSR_ABS, &cpu6502::JSR, &cpu6502::ABS, 6 }, // 0x20
        { AND_IDX, &cpu6502::AND, &cpu6502::IDX, 6 }, // 0x21
        { _0x22__, &cpu6502::___, &cpu6502::___, 0 }, // 0x22
        { _0x23__, &cpu6502::___, &cpu6502::___, 0 }, // 0x23
        { BIT_ZPG, &cpu6502::BIT, &cpu6502::ZPG, 3 }, // 0x24
        { AND_ZPG, &cpu6502::AND, &cpu6502::ZPG, 3 }, // 0x25
        { ROL_ZPG, &cpu6502::ROL, &cpu6502::ZPG, 5 }, // 0x26
        { _0x27__, &cpu6502::___, &cpu6502::___, 0 }, // 0x27
        { PLP_IMP, &cpu6502::PLP, &cpu6502::IMP, 4 }, // 0x28
        { AND_IMM, &cpu6502::AND, &cpu6502::IMM, 2 }, // 0x29
        { ROL_ACC, &cpu6502::ROL, &cpu6502::ACC, 2 }, // 0x2A
        { _0x2B__, &cpu6502::___, &cpu6502::___, 0 }, // 0x2B
        { BIT_ABS, &cpu6502::BIT, &cpu6502::ABS, 4 }, // 0x2C
        { AND_ABS, &cpu6502::AND, &cpu6502::ABS, 4 }, // 0x2D
        { ROL_ABS, &cpu6502::ROL, &cpu6502::ABS, 6 }, // 0x2E
        { _0x2F__, &cpu6502::___, &cpu6502::___, 0 }, // 0x2F
        { BMI_REL, &cpu6502::BMI, &cpu6502::REL, 2 }, // 0x30
        { AND_IDY, &cpu6502::AND, &cpu6502::IDY, 5 }, // 0x31
        { _0x32__, &cpu6502::___, &cpu6502::___, 0 }, // 0x32
        { _0x33__, &cpu6502::___, &cpu6502::___, 0 }, // 0x33
        { _0x34__, &cpu6502::___, &cpu6502::___, 0 }, // 0x34
        { AND_ZPX, &cpu6502::AND, &cpu6502::ZPX, 4 }, // 0x35
        { ROL_ZPX, &cpu6502::ROL, &cpu6502::ZPX, 6 }, // 0x36
        { _0x37__, &cpu6502::___, &cpu6502::___, 0 }, // 0x37
        { SEC_IMP, &cpu6502::SEC, &cpu6502::IMP, 2 }, // 0x38
        { AND_ABY, &cpu6502::AND, &cpu6502::ABY, 4 }, // 0x39
        { _0x3A__, &cpu6502::___, &cpu6502::___, 0 }, // 0x3A
        { _0x3B__, &cpu6502::___, &cpu6502::___, 0 }, // 0x3B
        { _0x3C__, &cpu6502::___, &cpu6502::___, 0 }, // 0x3C
        { AND_ABX, &cpu6502::AND, &cpu6502::ABX, 4 }, // 0x3D
        { ROL_ABX, &cpu6502::ROL, &cpu6502::ABX, 7 }, // 0x3E
        { _0x3F__, &cpu6502::___, &cpu6502::___, 0 }, // 0x3F
        { RTI_IMP, &cpu6502::RTI, &cpu6502::IMP, 6 }, // 0x40
        { EOR_IDX, &cpu6502::EOR, &cpu6502::IDX, 6 }, // 0x41
        { _0x42__, &cpu6502::___, &cpu6502::___, 0 }, // 0x42
        { _0x43__, &cpu6502::___, &cpu6502::___, 0 }, // 0x43
        { _0x44__, &cpu6502::___, &cpu6502::___, 0 }, // 0x44
        { EOR_ZPG, &cpu6502::EOR, &cpu6502::ZPG, 3 }, // 0x45
        { LSR_ZPG, &cpu6502::LSR, &cpu6502::ZPG, 5 }, // 0x46
        { _0x47__, &cpu6502::___, &cpu6502::___, 0 }, // 0x47
        { PHA_IMP, &cpu6502::PHA, &cpu6502::IMP, 3 }, // 0x48
        { EOR_IMM, &cpu6502::EOR, &cpu6502::IMM, 2 }, // 0x49
        { LSR_ACC, &cpu6502::LSR, &cpu6502::ACC, 2 }, // 0x4A
        { _0x4B__, &cpu6502::___, &cpu6502::___, 0 }, // 0x4B
        { JMP_ABS, &cpu6502::JMP, &cpu6502::ABS, 3 }, // 0x4C
        { EOR_ABS, &cpu6502::EOR, &cpu6502::ABS, 4 }, // 0x4D
        { LSR_ABS, &cpu6502::LSR, &cpu6502::ABS, 6 }, // 0x4E
        { _0x4F__, &cpu6502::___, &cpu6502::___, 0 }, // 0x4F
        { BVC_REL, &cpu6502::BVC, &cpu6502::REL, 2 }, // 0x50
        { EOR_IDY, &cpu6502::EOR, &cpu6502::IDY, 5 }, // 0x51
        { _0x52__, &cpu6502::___, &cpu6502::___, 0 }, // 0x52
        { _0x53__, &cpu6502::___, &cpu6502::___, 0 }, // 0x53
        { _0x54__, &cpu6502::___, &cpu6502::___, 0 }, // 0x54
        { EOR_ZPX, &cpu6502::EOR, &cpu6502::ZPX, 4 }, // 0x55
        { LSR_ZPX, &cpu6502::LSR, &cpu6502::ZPX, 6 }, // 0x56
        { _0x57__, &cpu6502::___, &cpu6502::___, 0 }, // 0x57
        { CLI_IMP, &cpu6502::CLI, &cpu6502::IMP, 2 }, // 0x58
        { EOR_ABY, &cpu6502::EOR, &cpu6502::ABY, 4 }, // 0x59
        { _0x5A__, &cpu6502::___, &cpu6502::___, 0 }, // 0x5A
        { _0x5B__, &cpu6502::___, &cpu6502::___, 0 }, // 0x5B
        { _0x5C__, &cpu6502::___, &cpu6502::___, 0 }, // 0x5C
        { EOR_ABX, &cpu6502::EOR, &cpu6502::ABX, 4 }, // 0x5D
        { LSR_ABX, &cpu6502::LSR, &cpu6502::ABX, 7 }, // 0x5E
        { _0x5F__, &cpu6502::___, &cpu6502::___, 0 }, // 0x5F
        { RTS_IMP, &cpu6502::RTS, &cpu6502::IMP, 6 }, // 0x60
        { ADC_IDX, &cpu6502::ADC, &cpu6502::IDX, 6 }, // 0x61
        { _0x62__, &cpu6502::___, &cpu6502::___, 0 }, // 0x62
        { _0x63__, &cpu6502::___, &cpu6502::___, 0 }, // 0x63
        { _0x64__, &cpu6502::___, &cpu6502::___, 0 }, // 0x64
        { ADC_ZPG, &cpu6502::ADC, &cpu6502::ZPG, 3 }, // 0x65
        { ROR_ZPG, &cpu6502::ROR, &cpu6502::ZPG, 5 }, // 0x66
        { _0x67__, &cpu6502::___, &cpu6502::___, 0 }, // 0x67
        { PLA_IMP, &cpu6502::PLA, &cpu6502::IMP, 4 }, // 0x68
        { ADC_IMM, &cpu6502::ADC, &cpu6502::IMM, 2 }, // 0x69
        { ROR_ACC, &cpu6502::ROR, &cpu6502::ACC, 2 }, // 0x6A
        { _0x6B__, &cpu6502::___, &cpu6502::___, 0 }, // 0x6B
        { JMP_IND, &cpu6502::JMP, &cpu6502::IND, 5 }, // 0x6C
        { ADC_ABS, &cpu6502::ADC, &cpu6502::ABS, 4 }, // 0x6D
        { ROR_ABS, &cpu6502::ROR, &cpu6502::ABS, 6 }, // 0x6E
        { _0x6F__, &cpu6502::___, &cpu6502::___, 0 }, // 0x6F
        { BVS_REL, &cpu6502::BVS, &cpu6502::REL, 2 }, // 0x70
        { ADC_IDY, &cpu6502::ADC, &cpu6502::IDY, 5 }, // 0x71
        { _0x72__, &cpu6502::___, &cpu6502::___, 0 }, // 0x72
        { _0x73__, &cpu6502::___, &cpu6502::___, 0 }, // 0x73
        { _0x74__, &cpu6502::___, &cpu6502::___, 0 }, // 0x74
        { ADC_ZPX, &cpu6502::ADC, &cpu6502::ZPX, 4 }, // 0x75
        { ROR_ZPX, &cpu6502::ROR, &cpu6502::ZPX, 6 }, // 0x76
        { _0x77__, &cpu6502::___, &cpu6502::___, 0 }, // 0x77
        { SEI_IMP, &cpu6502::SEI, &cpu6502::IMP, 2 }, // 0x78
        { ADC_ABY, &cpu6502::ADC, &cpu6502::ABY, 4 }, // 0x79
        { _0x7A__, &cpu6502::___, &cpu6502::___, 0 }, // 0x7A
        { _0x7B__, &cpu6502::___, &cpu6502::___, 0 }, // 0x7B
        { _0x7C__, &cpu6502::___, &cpu6502::___, 0 }, // 0x7C
        { ADC_ABX, &cpu6502::ADC, &cpu6502::ABX, 4 }, // 0x7D
        { ROR_ABX, &cpu6502::ROR, &cpu6502::ABX, 7 }, // 0x7E
        { _0x7F__, &cpu6502::___, &cpu6502::___, 0 }, // 0x7F
        { _0x80__, &cpu6502::___, &cpu6502::___, 0 }, // 0x80
        { STA_IDX, &cpu6502::STA, &cpu6502::IDX, 6 }, // 0x81
        { _0x82__, &cpu6502::___, &cpu6502::___, 0 }, // 0x82
        { _0x83__, &cpu6502::___, &cpu6502::___, 0 }, // 0x83
        { STY_ZPG, &cpu6502::STY, &cpu6502::ZPG, 3 }, // 0x84
        { STA_ZPG, &cpu6502::STA, &cpu6502::ZPG, 3 }, // 0x85
        { STX_ZPG, &cpu6502::STX, &cpu6502::ZPG, 3 }, // 0x86
        { _0x87__, &cpu6502::___, &cpu6502::___, 0 }, // 0x87
        { DEY_IMP, &cpu6502::DEY, &cpu6502::IMP, 2 }, // 0x88
        { _0x89__, &cpu6502::___, &cpu6502::___, 0 }, // 0x89
        { TXA_IMP, &cpu6502::TXA, &cpu6502::IMP, 2 }, // 0x8A
        { _0x8B__, &cpu6502::___, &cpu6502::___, 0 }, // 0x8B
        { STY_ABS, &cpu6502::STY, &cpu6502::ABS, 4 }, // 0x8C
        { STA_ABS, &cpu6502::STA, &cpu6502::ABS, 4 }, // 0x8D
        { STX_ABS, &cpu6502::STX, &cpu6502::ABS, 4 }, // 0x8E
        { _0x8F__, &cpu6502::___, &cpu6502::___, 0 }, // 0x8F
        { BCC_REL, &cpu6502::BCC, &cpu6502::REL, 2 }, // 0x90
        { STA_IDY, &cpu6502::STA, &cpu6502::IDY, 6 }, // 0x91
        { _0x92__, &cpu6502::___, &cpu6502::___, 0 }, // 0x92
        { _0x93__, &cpu6502::___, &cpu6502::___, 0 }, // 0x93
        { STY_ZPX, &cpu6502::STY, &cpu6502::ZPX, 4 }, // 0x94
        { STA_ZPX, &cpu6502::STA, &cpu6502::ZPX, 4 }, // 0x95
        { STX_ZPY, &cpu6502::STX, &cpu6502::ZPY, 4 }, // 0x96
        { _0x97__, &cpu6502::___, &cpu6502::___, 0 }, // 0x97
        { TYA_IMP, &cpu6502::TYA, &cpu6502::IMP, 2 }, // 0x98
        { STA_ABY, &cpu6502::STA, &cpu6502::ABY, 5 }, // 0x99
        { TXS_IMP, &cpu6502::TXS, &cpu6502::IMP, 2 }, // 0x9A
        { _0x9B__, &cpu6502::___, &cpu6502::___, 0 }, // 0x9B
        { _0x9C__, &cpu6502::___, &cpu6502::___, 0 }, // 0x9C
        { STA_ABX, &cpu6502::STA, &cpu6502::ABX, 5 }, // 0x9D
        { _0x9E__, &cpu6502::___, &cpu6502::___, 0 }, // 0x9E
        { _0x9F__, &cpu6502::___, &cpu6502::___, 0 }, // 0x9F
        { LDY_IMM, &cpu6502::LDY, &cpu6502::IMM, 2 }, // 0xA0
        { LDA_IDX, &cpu6502::LDA, &cpu6502::IDX, 6 }, // 0xA1
        { LDX_IMM, &cpu6502::LDX, &cpu6502::IMM, 2 }, // 0xA2
        { _0xA3__, &cpu6502::___, &cpu6502::___, 0 }, // 0xA3
        { LDY_ZPG, &cpu6502::LDY, &cpu6502::ZPG, 3 }, // 0xA4
        { LDA_ZPG, &cpu6502::LDA, &cpu6502::ZPG, 3 }, // 0xA5
        { LDX_ZPG, &cpu6502::LDX, &cpu6502::ZPG, 3 }, // 0xA6
        { _0xA7__, &cpu6502::___, &cpu6502::___, 0 }, // 0xA7
        { TAY_IMP, &cpu6502::TAY, &cpu6502::IMP, 2 }, // 0xA8
        { LDA_IMM, &cpu6502::LDA, &cpu6502::IMM, 2 }, // 0xA9
        { TAX_IMP, &cpu6502::TAX, &cpu6502::IMP, 2 }, // 0xAA
        { _0xAB__, &cpu6502::___, &cpu6502::___, 0 }, // 0xAB
        { LDY_ABS, &cpu6502::LDY, &cpu6502::ABS, 4 }, // 0xAC
        { LDA_ABS, &cpu6502::LDA, &cpu6502::ABS, 4 }, // 0xAD
        { LDX_ABS, &cpu6502::LDX, &cpu6502::ABS, 4 }, // 0xAE
        { _0xAF__, &cpu6502::___, &cpu6502::___, 0 }, // 0xAF
        { BCS_REL, &cpu6502::BCS, &cpu6502::REL, 2 }, // 0xB0
        { LDA_IDY, &cpu6502::LDA, &cpu6502::IDY, 5 }, // 0xB1
        { _0xB2__, &cpu6502::___, &cpu6502::___, 0 }, // 0xB2
        { _0xB3__, &cpu6502::___, &cpu6502::___, 0 }, // 0xB3
        { LDY_ZPX, &cpu6502::LDY, &cpu6502::ZPX, 4 }, // 0xB4
        { LDA_ZPX, &cpu6502::LDA, &cpu6502::ZPX, 4 }, // 0xB5
        { LDX_ZPY, &cpu6502::LDX, &cpu6502::ZPY, 4 }, // 0xB6
        { _0xB7__, &cpu6502::___, &cpu6502::___, 0 }, // 0xB7
        { CLV_IMP, &cpu6502::CLV, &cpu6502::IMP, 2 }, // 0xB8
        { LDA_ABY, &cpu6502::LDA, &cpu6502::ABY, 4 }, // 0xB9
        { TSX_IMP, &cpu6502::TSX, &cpu6502::IMP, 2 }, // 0xBA
        { _0xBB__, &cpu6502::___, &cpu6502::___, 0 }, // 0xBB
        { LDY_ABX, &cpu6502::LDY, &cpu6502::ABX, 4 }, // 0xBC
        { LDA_ABX, &cpu6502::LDA, &cpu6502::ABX, 4 }, // 0xBD
        { LDX_ABY, &cpu6502::LDX, &cpu6502::ABY, 4 }, // 0xBE
        { _0xBF__, &cpu6502::___, &cpu6502::___, 0 }, // 0xBF
        { CPY_IMM, &cpu6502::CPY, &cpu6502::IMM, 2 }, // 0xC0
        { CMP_IDX, &cpu6502::CMP, &cpu6502::IDX, 6 }, // 0xC1
        { _0xC2__, &cpu6502::___, &cpu6502::___, 0 }, // 0xC2
        { _0xC3__, &cpu6502::___, &cpu6502::___, 0 }, // 0xC3
        { CPY_ZPG, &cpu6502::CPY, &cpu6502::ZPG, 3 }, // 0xC4
        { CMP_ZPG, &cpu6502::CMP, &cpu6502::ZPG, 3 }, // 0xC5
        { DEC_ZPG, &cpu6502::DEC, &cpu6502::ZPG, 5 }, // 0xC6
        { _0xC7__, &cpu6502::___, &cpu6502::___, 0 }, // 0xC7
        { INY_IMP, &cpu6502::INY, &cpu6502::IMP, 2 }, // 0xC8
        { CMP_IMM, &cpu6502::CMP, &cpu6502::IMM, 2 }, // 0xC9
        { DEX_IMP, &cpu6502::DEX, &cpu6502::IMP, 2 }, // 0xCA
        { _0xCB__, &cpu6502::___, &cpu6502::___, 0 }, // 0xCB
        { CPY_ABS, &cpu6502::CPY, &cpu6502::ABS, 4 }, // 0xCC
        { CMP_ABS, &cpu6502::CMP, &cpu6502::ABS, 4 }, // 0xCD
        { DEC_ABS, &cpu6502::DEC, &cpu6502::ABS, 6 }, // 0xCE
        { _0xCF__, &cpu6502::___, &cpu6502::___, 0 }, // 0xCF
        { BNE_REL, &cpu6502::BNE, &cpu6502::REL, 2 }, // 0xD0
        { CMP_IDY, &cpu6502::CMP, &cpu6502::IDY, 5 }, // 0xD1
        { _0xD2__, &cpu6502::___, &cpu6502::___, 0 }, // 0xD2
        { _0xD3__, &cpu6502::___, &cpu6502::___, 0 }, // 0xD3
        { _0xD4__, &cpu6502::___, &cpu6502::___, 0 }, // 0xD4
        { CMP_ZPX, &cpu6502::CMP, &cpu6502::ZPX, 4 }, // 0xD5
        { DEC_ZPX, &cpu6502::DEC, &cpu6502::ZPX, 6 }, // 0xD6
        { _0xD7__, &cpu6502::___, &cpu6502::___, 0 }, // 0xD7
        { CLD_IMP, &cpu6502::CLD, &cpu6502::IMP, 2 }, // 0xD8
        { CMP_ABY, &cpu6502::CMP, &cpu6502::ABY, 4 }, // 0xD9
        { _0xDA__, &cpu6502::___, &cpu6502::___, 0 }, // 0xDA
        { _0xDB__, &cpu6502::___, &cpu6502::___, 0 }, // 0xDB
        { _0xDC__, &cpu6502::___, &cpu6502::___, 0 }, // 0xDC
        { CMP_ABX, &cpu6502::CMP, &cpu6502::ABX, 4 }, // 0xDD
        { DEC_ABX, &cpu6502::DEC, &cpu6502::ABX, 7 }, // 0xDE
        { _0xDF__, &cpu6502::___, &cpu6502::___, 0 }, // 0xDF
        { CPX_IMM, &cpu6502::CPX, &cpu6502::IMM, 2 }, // 0xE0
        { SBC_IDX, &cpu6502::SBC, &cpu6502::IDX, 6 }, // 0xE1
        { _0xE2__, &cpu6502::___, &cpu6502::___, 0 }, // 0xE2
        { _0xE3__, &cpu6502::___, &cpu6502::___, 0 }, // 0xE3
        { CPX_ZPG, &cpu6502::CPX, &cpu6502::ZPG, 3 }, // 0xE4
        { SBC_ZPG, &cpu6502::SBC, &cpu6502::ZPG, 3 }, // 0xE5
        { INC_ZPG, &cpu6502::INC, &cpu6502::ZPG, 5 }, // 0xE6
        { _0xE7__, &cpu6502::___, &cpu6502::___, 0 }, // 0xE7
        { INX_IMP, &cpu6502::INX, &cpu6502::IMP, 2 }, // 0xE8
        { SBC_IMM, &cpu6502::SBC, &cpu6502::IMM, 2 }, // 0xE9
        { NOP____, &cpu6502::NOP, &cpu6502::___, 2 }, // 0xEA
        { _0xEB__, &cpu6502::___, &cpu6502::___, 2 }, // 0xEB
        { CPX_ABS, &cpu6502::CPX, &cpu6502::ABS, 4 }, // 0xEC
        { SBC_ABS, &cpu6502::SBC, &cpu6502::ABS, 4 }, // 0xED
        { INC_ABS, &cpu6502::INC, &cpu6502::ABS, 6 }, // 0xEE
        { _0xEF__, &cpu6502::___, &cpu6502::___, 0 }, // 0xEF
        { BEQ_REL, &cpu6502::BEQ, &cpu6502::REL, 2 }, // 0xF0
        { SBC_IDY, &cpu6502::SBC, &cpu6502::IDY, 5 }, // 0xF1
        { _0xF2__, &cpu6502::___, &cpu6502::___, 0 }, // 0xF2
        { _0xF3__, &cpu6502::___, &cpu6502::___, 0 }, // 0xF3
        { _0xF4__, &cpu6502::___, &cpu6502::___, 0 }, // 0xF4
        { SBC_ZPX, &cpu6502::SBC, &cpu6502::ZPX, 4 }, // 0xF5
        { INC_ZPX, &cpu6502::INC, &cpu6502::ZPX, 6 }, // 0xF6
        { _0xF7__, &cpu6502::___, &cpu6502::___, 0 }, // 0xF7
        { SED_IMP, &cpu6502::SED, &cpu6502::IMP, 2 }, // 0xF8
        { SBC_ABY, &cpu6502::SBC, &cpu6502::ABY, 4 }, // 0xF9
        { _0xFA__, &cpu6502::___, &cpu6502::___, 2 }, // 0xFA
        { _0xFB__, &cpu6502::___, &cpu6502::___, 0 }, // 0xFB
        { _0xFC__, &cpu6502::___, &cpu6502::___, 0 }, // 0xFC
        { SBC_ABX, &cpu6502::SBC, &cpu6502::ABX, 4 }, // 0xFD
        { INC_ABX, &cpu6502::INC, &cpu6502::ABX, 7 }, // 0xFE
        { _0xFF__, &cpu6502::___, &cpu6502::___, 0 }, // 0xFF
    } };

private:
    enum class extra_cycle {
        if_carry,
        if_carry_possible,
        always
    };

    void zero_page(uint8_t offset);
    void absolute(uint8_t offset);
    auto load(extra_cycle e = extra_cycle::if_carry) -> uint8_t;
    void store(uint8_t data, extra_cycle e = extra_cycle::if_carry_possible);
    void transfer(uint8_t src, uint8_t& dst);
    void push(uint8_t data);
    auto pull() -> uint8_t;
    void add_or_substract(uint8_t data);
    void increment(uint8_t& reg);
    void decrement(uint8_t& reg);
    void compare(uint8_t lhs);
    void shift(auto&& f, bool acc_addressing)
    {
        if (acc_addressing) {
            f(A);
        } else {
            uint8_t data = load(extra_cycle::if_carry_possible);
            f(data);
            store(data, extra_cycle::always);
        }
    }
    void branch(bool is_branch);
};

}