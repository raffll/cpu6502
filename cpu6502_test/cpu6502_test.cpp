#include "cpu6502_test.h"

namespace emulator {

void cpu6502_test::SetUp()
{
    clock.reset();
    bus.reset();
    cpu.reset();
    counter = 0x0200;
    carry = false;
}

void cpu6502_test::create_IMM(oc opcode, uint8_t data)
{
    bus.write(counter++, opcode);
    bus.write(counter++, data);
}

uint16_t cpu6502_test::create_ZPG(oc opcode)
{
    bus.write(counter++, opcode);
    bus.write(counter++, 0x80);

    return 0x80;
}

uint16_t cpu6502_test::create_ZP(oc opcode, uint8_t& offset_reg)
{
    offset_reg = 0x0F;
    bus.write(counter++, opcode);
    bus.write(counter++, 0x80);

    return 0x80 + offset_reg;
}

uint16_t cpu6502_test::create_ABS(oc opcode)
{
    bus.write(counter++, opcode);
    bus.write(counter++, 0x80);
    bus.write(counter++, 0x40);

    return 0x4080;
}

uint16_t cpu6502_test::create_AB(oc opcode, uint8_t& offset_reg)
{
    if (carry)
        offset_reg = 0xFF;
    else
        offset_reg = 0x0F;

    bus.write(counter++, opcode);
    bus.write(counter++, 0x80);
    bus.write(counter++, 0x40);

    return 0x4080 + offset_reg;
}

uint16_t cpu6502_test::create_IND(oc opcode)
{
    bus.write(counter++, opcode);
    bus.write(counter++, 0x20);
    bus.write(counter++, 0x30);
    bus.write(0x3020, 0x80);
    bus.write(0x3021, 0x40);

    return 0x4080;
}

uint16_t cpu6502_test::create_IDX(oc opcode)
{
    cpu.X = 0x0F;
    bus.write(counter++, opcode);
    bus.write(counter++, 0x20);
    bus.write(0x002F, 0x80);
    bus.write(0x0030, 0x40);

    return 0x4080;
}

uint16_t cpu6502_test::create_IDY(oc opcode)
{
    if (carry)
        cpu.Y = 0xFF;
    else
        cpu.Y = 0x0F;

    bus.write(counter++, opcode);
    bus.write(counter++, 0x20);
    bus.write(0x0020, 0x80);
    bus.write(0x0021, 0x40);

    return 0x4080 + cpu.Y;
}

void cpu6502_test::load_IMM(oc opcode, uint8_t& reg)
{
    create_IMM(opcode, 0x0A);
    cpu.execute();

    EXPECT_EQ(reg, 0x0A);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

    create_IMM(opcode, 0x00);
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 1);

    create_IMM(opcode, 0x80);
    cpu.execute();

    EXPECT_EQ(cpu.P.N, 1);
}

TEST_F(cpu6502_test, LDA_IMM)
{
    load_IMM(oc::LDA_IMM, cpu.A);
}

TEST_F(cpu6502_test, LDX_IMM)
{
    load_IMM(oc::LDX_IMM, cpu.X);
}

TEST_F(cpu6502_test, LDY_IMM)
{
    load_IMM(oc::LDY_IMM, cpu.Y);
}

void cpu6502_test::load_ZPG(oc opcode, uint8_t& reg)
{
    bus.write(create_ZPG(opcode), 0x0B);
    cpu.execute();

    EXPECT_EQ(reg, 0x0B);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, LDA_ZPG)
{
    load_ZPG(oc::LDA_ZPG, cpu.A);
}

TEST_F(cpu6502_test, LDX_ZPG)
{
    load_ZPG(oc::LDX_ZPG, cpu.X);
}

TEST_F(cpu6502_test, LDY_ZPG)
{
    load_ZPG(oc::LDY_ZPG, cpu.Y);
}

void cpu6502_test::load_ZP(oc opcode, uint8_t& reg, uint8_t& offset_reg)
{
    bus.write(create_ZP(opcode, offset_reg), 0x0C);
    cpu.execute();

    EXPECT_EQ(reg, 0x0C);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, LDA_ZPX)
{
    load_ZP(oc::LDA_ZPX, cpu.A, cpu.X);
}

TEST_F(cpu6502_test, LDX_ZPY)
{
    load_ZP(oc::LDX_ZPY, cpu.X, cpu.Y);
}

TEST_F(cpu6502_test, LDY_ZPX)
{
    load_ZP(oc::LDY_ZPX, cpu.Y, cpu.X);
}

void cpu6502_test::load_ABS(oc opcode, uint8_t& reg)
{
    bus.write(create_ABS(opcode), 0x0D);
    cpu.execute();

    EXPECT_EQ(reg, 0x0D);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, LDA_ABS)
{
    load_ABS(oc::LDA_ABS, cpu.A);
}

TEST_F(cpu6502_test, LDX_ABS)
{
    load_ABS(oc::LDX_ABS, cpu.X);
}

TEST_F(cpu6502_test, LDY_ABS)
{
    load_ABS(oc::LDY_ABS, cpu.Y);
}

void cpu6502_test::load_AB(oc opcode, uint8_t& reg, uint8_t& offset_reg)
{
    bus.write(create_AB(opcode, offset_reg), 0x0E);
    cpu.execute();

    EXPECT_EQ(reg, 0x0E);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
}

TEST_F(cpu6502_test, LDA_ABX)
{
    load_AB(oc::LDA_ABX, cpu.A, cpu.X);
}

TEST_F(cpu6502_test, LDA_ABX_with_carry)
{
    carry = true;
    load_AB(oc::LDA_ABX, cpu.A, cpu.X);
}

TEST_F(cpu6502_test, LDA_ABY)
{
    load_AB(oc::LDA_ABY, cpu.A, cpu.Y);
}

TEST_F(cpu6502_test, LDA_ABY_with_carry)
{
    carry = true;
    load_AB(oc::LDA_ABY, cpu.A, cpu.Y);
}

TEST_F(cpu6502_test, LDX_ABY)
{
    load_AB(oc::LDX_ABY, cpu.X, cpu.Y);
}

TEST_F(cpu6502_test, LDX_ABY_with_carry)
{
    carry = true;
    load_AB(oc::LDX_ABY, cpu.X, cpu.Y);
}

TEST_F(cpu6502_test, LDY_ABX)
{
    load_AB(oc::LDY_ABX, cpu.Y, cpu.X);
}

TEST_F(cpu6502_test, LDY_ABX_with_carry)
{
    carry = true;
    load_AB(oc::LDY_ABX, cpu.Y, cpu.X);
}

void cpu6502_test::load_IDX(oc opcode, uint8_t& reg)
{
    bus.write(create_IDX(opcode), 0x0A);
    cpu.execute();

    EXPECT_EQ(reg, 0x0A);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, LDA_IDX)
{
    load_IDX(oc::LDA_IDX, cpu.A);
}

void cpu6502_test::load_IDY(oc opcode, uint8_t& reg)
{
    bus.write(create_IDY(opcode), 0x0B);
    cpu.execute();

    EXPECT_EQ(reg, 0x0B);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
}

TEST_F(cpu6502_test, LDA_IDY)
{
    load_IDY(oc::LDA_IDY, cpu.A);
}

TEST_F(cpu6502_test, LDA_IDY_with_carry)
{
    carry = true;
    load_IDY(oc::LDA_IDY, cpu.A);
}

void cpu6502_test::store_ZPG(oc opcode, uint8_t& reg)
{
    auto address = create_ZPG(opcode);
    reg = 0x0B;
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0x0B);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, STA_ZPG)
{
    store_ZPG(oc::STA_ZPG, cpu.A);
}

TEST_F(cpu6502_test, STX_ZPG)
{
    store_ZPG(oc::STX_ZPG, cpu.X);
}

TEST_F(cpu6502_test, STY_ZPG)
{
    store_ZPG(oc::STY_ZPG, cpu.Y);
}

void cpu6502_test::store_ZP(oc opcode, uint8_t& reg, uint8_t& offset_reg)
{
    auto address = create_ZP(opcode, offset_reg);
    reg = 0x0C;
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0x0C);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, STA_ZPX)
{
    store_ZP(oc::STA_ZPX, cpu.A, cpu.X);
}

TEST_F(cpu6502_test, STX_ZPY)
{
    store_ZP(oc::STX_ZPY, cpu.X, cpu.Y);
}

TEST_F(cpu6502_test, STY_ZPX)
{
    store_ZP(oc::STY_ZPX, cpu.Y, cpu.X);
}

void cpu6502_test::store_ABS(oc opcode, uint8_t& reg)
{
    auto address = create_ABS(opcode);
    reg = 0x0D;
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0x0D);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, STA_ABS)
{
    store_ABS(oc::STA_ABS, cpu.A);
}

TEST_F(cpu6502_test, STX_ABS)
{
    store_ABS(oc::STX_ABS, cpu.X);
}

TEST_F(cpu6502_test, STY_ABS)
{
    store_ABS(oc::STY_ABS, cpu.Y);
}

void cpu6502_test::store_AB(oc opcode, uint8_t& reg, uint8_t& offset_reg)
{
    auto address = create_AB(opcode, offset_reg);
    reg = 0x0E;
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0x0E);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, STA_ABX)
{
    store_AB(oc::STA_ABX, cpu.A, cpu.X);
}

TEST_F(cpu6502_test, STA_ABX_with_carry)
{
    carry = true;
    store_AB(oc::STA_ABX, cpu.A, cpu.X);
}

TEST_F(cpu6502_test, STA_ABY)
{
    store_AB(oc::STA_ABY, cpu.A, cpu.Y);
}

TEST_F(cpu6502_test, STA_ABY_with_carry)
{
    carry = true;
    store_AB(oc::STA_ABY, cpu.A, cpu.Y);
}

void cpu6502_test::store_IDX(oc opcode, uint8_t& reg)
{
    auto address = create_IDX(opcode);
    reg = 0x0A;
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0x0A);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, STA_IDX)
{
    store_IDX(oc::STA_IDX, cpu.A);
}

void cpu6502_test::store_IDY(oc opcode, uint8_t& reg)
{
    auto address = create_IDY(opcode);
    reg = 0x0B;
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0x0B);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, STA_IDY)
{
    store_IDY(oc::STA_IDY, cpu.A);
}

TEST_F(cpu6502_test, STA_IDY_with_carry)
{
    carry = true;
    store_IDY(oc::STA_IDY, cpu.A);
}

void cpu6502_test::transfer(oc opcode, uint8_t& src, uint8_t& dst, bool check_Z_N)
{
    bus.write(counter++, opcode);
    src = 0x0A;
    cpu.execute();

    EXPECT_EQ(src, dst);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

    if (check_Z_N) {
        bus.write(counter++, opcode);
        src = 0x00;
        cpu.execute();

        EXPECT_EQ(cpu.P.Z, 1);

        bus.write(counter++, opcode);
        src = 0x80;
        cpu.execute();

        EXPECT_EQ(cpu.P.N, 1);
    }
}

TEST_F(cpu6502_test, TAX_IMP)
{
    transfer(oc::TAX_IMP, cpu.A, cpu.X, true);
}

TEST_F(cpu6502_test, TAY_IMP)
{
    transfer(oc::TAY_IMP, cpu.A, cpu.Y, true);
}

TEST_F(cpu6502_test, TXA_IMP)
{
    transfer(oc::TXA_IMP, cpu.X, cpu.A, true);
}

TEST_F(cpu6502_test, TYA_IMP)
{
    transfer(oc::TYA_IMP, cpu.Y, cpu.A, true);
}

TEST_F(cpu6502_test, TSX_IMP)
{
    transfer(oc::TSX_IMP, cpu.S, cpu.X, true);
}

TEST_F(cpu6502_test, TXS_IMP)
{
    transfer(oc::TXS_IMP, cpu.X, cpu.S, false);
}

TEST_F(cpu6502_test, PHA_IMP)
{
    auto opcode = oc::PHA_IMP;

    bus.write(counter++, opcode);
    cpu.A = 0x0B;
    cpu.execute();

    EXPECT_EQ(cpu.S, 0xFE);
    EXPECT_EQ(bus.read(0x01FF), 0x0B);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, PHP_IMP)
{
    auto opcode = oc::PHP_IMP;

    bus.write(counter++, opcode);
    cpu.P.N = 1;
    cpu.execute();

    EXPECT_EQ(cpu.S, 0xFE);
    EXPECT_EQ(bus.read(0x01FF), 0b10110000);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, PLA_IMP)
{
    auto opcode = oc::PLA_IMP;

    bus.write(counter++, opcode);
    bus.write(0x01FF, 0x0C);
    cpu.S = 0xFE;
    cpu.execute();

    EXPECT_EQ(cpu.S, 0xFF);
    EXPECT_EQ(cpu.A, 0x0C);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, PLP_IMP)
{
    auto opcode = oc::PLP_IMP;

    bus.write(counter++, opcode);
    bus.write(0x01FF, 0x80);
    cpu.S = 0xFE;
    cpu.execute();

    EXPECT_EQ(cpu.S, 0xFF);
    EXPECT_EQ(cpu.P.N, 1);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

void cpu6502_test::logic(oc opcode, uint8_t a, uint8_t b)
{
    cpu.A = a;
    cpu.execute();

    EXPECT_EQ(cpu.A, b);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
}

TEST_F(cpu6502_test, AND_IMM)
{
    // 000000110 & 6
    // 000000011   3
    // ---------
    // 000000010   2

    auto opcode = oc::AND_IMM;

    create_IMM(opcode, 0x06);
    logic(opcode, 0x03, 0x02);

    create_IMM(opcode, 0x00);
    cpu.A = 0x00;
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 1);

    create_IMM(opcode, 0x80);
    cpu.A = 0x80;
    cpu.execute();

    EXPECT_EQ(cpu.P.N, 1);
}

TEST_F(cpu6502_test, AND_ZPG)
{
    auto opcode = oc::AND_ZPG;

    bus.write(create_ZPG(opcode), 0x06);
    logic(opcode, 0x03, 0x02);
}

TEST_F(cpu6502_test, AND_ZPX)
{
    auto opcode = oc::AND_ZPX;

    bus.write(create_ZP(opcode, cpu.X), 0x06);
    logic(opcode, 0x03, 0x02);
}

TEST_F(cpu6502_test, AND_ABS)
{
    auto opcode = oc::AND_ABS;

    bus.write(create_ABS(opcode), 0x06);
    logic(opcode, 0x03, 0x02);
}

TEST_F(cpu6502_test, AND_ABX)
{
    auto opcode = oc::AND_ABX;

    bus.write(create_AB(opcode, cpu.X), 0x06);
    logic(opcode, 0x03, 0x02);
}

TEST_F(cpu6502_test, AND_ABX_with_carry)
{
    auto opcode = oc::AND_ABX;

    carry = true;
    bus.write(create_AB(opcode, cpu.X), 0x06);
    logic(opcode, 0x03, 0x02);
}

TEST_F(cpu6502_test, AND_ABY)
{
    auto opcode = oc::AND_ABY;

    bus.write(create_AB(opcode, cpu.Y), 0x06);
    logic(opcode, 0x03, 0x02);
}

TEST_F(cpu6502_test, AND_ABY_with_carry)
{
    auto opcode = oc::AND_ABY;

    carry = true;
    bus.write(create_AB(opcode, cpu.Y), 0x06);
    logic(opcode, 0x03, 0x02);
}

TEST_F(cpu6502_test, AND_IDX)
{
    auto opcode = oc::AND_IDX;

    bus.write(create_IDX(opcode), 0x06);
    logic(opcode, 0x03, 0x02);
}

TEST_F(cpu6502_test, AND_IDY)
{
    auto opcode = oc::AND_IDY;

    bus.write(create_IDY(opcode), 0x06);
    logic(opcode, 0x03, 0x02);
}

TEST_F(cpu6502_test, AND_IDY_with_carry)
{
    auto opcode = oc::AND_IDY;

    carry = true;
    bus.write(create_IDY(opcode), 0x06);
    logic(opcode, 0x03, 0x02);
}

TEST_F(cpu6502_test, EOR_IMM)
{
    // 000000110 ^
    // 000000011
    // ---------
    // 000000101

    auto opcode = oc::EOR_IMM;

    create_IMM(opcode, 0x06);
    logic(opcode, 0x03, 0x05);

    create_IMM(opcode, 0x00);
    cpu.A = 0x00;
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 1);

    create_IMM(opcode, 0x80);
    cpu.A = 0x00;
    cpu.execute();

    EXPECT_EQ(cpu.P.N, 1);
}

TEST_F(cpu6502_test, EOR_ZPG)
{
    auto opcode = oc::EOR_ZPG;

    bus.write(create_ZPG(opcode), 0x06);
    logic(opcode, 0x03, 0x05);
}

TEST_F(cpu6502_test, EOR_ZPX)
{
    auto opcode = oc::EOR_ZPX;

    bus.write(create_ZP(opcode, cpu.X), 0x06);
    logic(opcode, 0x03, 0x05);
}

TEST_F(cpu6502_test, EOR_ABS)
{
    auto opcode = oc::EOR_ABS;

    bus.write(create_ABS(opcode), 0x06);
    logic(opcode, 0x03, 0x05);
}

TEST_F(cpu6502_test, EOR_ABX)
{
    auto opcode = oc::EOR_ABX;

    bus.write(create_AB(opcode, cpu.X), 0x06);
    logic(opcode, 0x03, 0x05);
}

TEST_F(cpu6502_test, EOR_ABX_with_carry)
{
    auto opcode = oc::EOR_ABX;

    carry = true;
    bus.write(create_AB(opcode, cpu.X), 0x06);
    logic(opcode, 0x03, 0x05);
}

TEST_F(cpu6502_test, EOR_ABY)
{
    auto opcode = oc::EOR_ABY;

    bus.write(create_AB(opcode, cpu.Y), 0x06);
    logic(opcode, 0x03, 0x05);
}

TEST_F(cpu6502_test, EOR_ABY_with_carry)
{
    auto opcode = oc::EOR_ABY;

    carry = true;
    bus.write(create_AB(opcode, cpu.Y), 0x06);
    logic(opcode, 0x03, 0x05);
}

TEST_F(cpu6502_test, EOR_IDX)
{
    auto opcode = oc::EOR_IDX;

    bus.write(create_IDX(opcode), 0x06);
    logic(opcode, 0x03, 0x05);
}

TEST_F(cpu6502_test, EOR_IDY)
{
    auto opcode = oc::EOR_IDY;

    bus.write(create_IDY(opcode), 0x06);
    logic(opcode, 0x03, 0x05);
}

TEST_F(cpu6502_test, EOR_IDY_with_carry)
{
    auto opcode = oc::EOR_IDY;

    carry = true;
    bus.write(create_IDY(opcode), 0x06);
    logic(opcode, 0x03, 0x05);
}

TEST_F(cpu6502_test, ORA_IMM)
{
    // 000000110 |
    // 000000011
    // ---------
    // 000000111

    auto opcode = oc::ORA_IMM;

    create_IMM(opcode, 0x06);
    logic(opcode, 0x03, 0x07);

    create_IMM(opcode, 0x00);
    cpu.A = 0x00;
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 1);

    create_IMM(opcode, 0x80);
    cpu.A = 0x00;
    cpu.execute();

    EXPECT_EQ(cpu.P.N, 1);
}

TEST_F(cpu6502_test, ORA_ZPG)
{
    auto opcode = oc::ORA_ZPG;

    bus.write(create_ZPG(opcode), 0x06);
    logic(opcode, 0x03, 0x07);
}

TEST_F(cpu6502_test, ORA_ZPX)
{
    auto opcode = oc::ORA_ZPX;

    bus.write(create_ZP(opcode, cpu.X), 0x06);
    logic(opcode, 0x03, 0x07);
}

TEST_F(cpu6502_test, ORA_ABS)
{
    auto opcode = oc::ORA_ABS;

    bus.write(create_ABS(opcode), 0x06);
    logic(opcode, 0x03, 0x07);
}

TEST_F(cpu6502_test, ORA_ABX)
{
    auto opcode = oc::ORA_ABX;

    bus.write(create_AB(opcode, cpu.X), 0x06);
    logic(opcode, 0x03, 0x07);
}

TEST_F(cpu6502_test, ORA_ABX_with_carry)
{
    auto opcode = oc::ORA_ABX;

    carry = true;
    bus.write(create_AB(opcode, cpu.X), 0x06);
    logic(opcode, 0x03, 0x07);
}

TEST_F(cpu6502_test, ORA_ABY)
{
    auto opcode = oc::ORA_ABY;

    bus.write(create_AB(opcode, cpu.Y), 0x06);
    logic(opcode, 0x03, 0x07);
}

TEST_F(cpu6502_test, ORA_ABY_with_carry)
{
    auto opcode = oc::ORA_ABY;

    carry = true;
    bus.write(create_AB(opcode, cpu.Y), 0x06);
    logic(opcode, 0x03, 0x07);
}

TEST_F(cpu6502_test, ORA_IDX)
{
    auto opcode = oc::ORA_IDX;

    bus.write(create_IDX(opcode), 0x06);
    logic(opcode, 0x03, 0x07);
}

TEST_F(cpu6502_test, ORA_IDY)
{
    auto opcode = oc::ORA_IDY;

    bus.write(create_IDY(opcode), 0x06);
    logic(opcode, 0x03, 0x07);
}

TEST_F(cpu6502_test, BIT_ZPG)
{
    auto opcode = oc::BIT_ZPG;

    bus.write(create_ZPG(opcode), 0x00);
    cpu.A = 0x00;
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 1);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

    bus.write(create_ZPG(opcode), 0xFE);
    cpu.A = 0xFF;
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 0);
    EXPECT_EQ(cpu.P.V, 1);
    EXPECT_EQ(cpu.P.N, 1);
    EXPECT_EQ(cpu.A, 0xFF);
}

TEST_F(cpu6502_test, BIT_ABS)
{
    auto opcode = oc::BIT_ABS;

    bus.write(create_ABS(opcode), 0x00);
    cpu.A = 0x00;
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 1);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

    bus.write(create_ABS(opcode), 0xFE);
    cpu.A = 0xFF;
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 0);
    EXPECT_EQ(cpu.P.V, 1);
    EXPECT_EQ(cpu.P.N, 1);
    EXPECT_EQ(cpu.A, 0xFF);
}

TEST_F(cpu6502_test, ADC_IMM_flag_C_Z)
{
    auto opcode = oc::ADC_IMM;

    create_IMM(opcode, 0xFF);
    cpu.A = 0x01;
    cpu.execute();

    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(cpu.P.Z, 1);
}

TEST_F(cpu6502_test, ADC_IMM_flag_N)
{
    auto opcode = oc::ADC_IMM;

    create_IMM(opcode, 0x7F);
    cpu.A = 0x01;
    cpu.execute();

    EXPECT_EQ(cpu.P.N, 1);
}

TEST_F(cpu6502_test, ADC_IMM_flag_V_both_positive)
{
    auto opcode = oc::ADC_IMM;

    create_IMM(opcode, 0x40);
    cpu.A = 0x40;
    cpu.execute();

    EXPECT_EQ(cpu.P.V, 1);
}

TEST_F(cpu6502_test, ADC_IMM_flag_V_both_negative)
{
    auto opcode = oc::ADC_IMM;

    create_IMM(opcode, 0x80);
    cpu.A = 0x80;
    cpu.execute();

    EXPECT_EQ(cpu.P.V, 1);
}

TEST_F(cpu6502_test, ADC_IMM_flag_V_positive_negative)
{
    auto opcode = oc::ADC_IMM;

    create_IMM(opcode, 0x80);
    cpu.A = 0x40;
    cpu.execute();

    EXPECT_EQ(cpu.P.V, 0);
}

TEST_F(cpu6502_test, ADC_IMM_flag_V_negative_positive)
{
    auto opcode = oc::ADC_IMM;

    create_IMM(opcode, 0x40);
    cpu.A = 0x80;
    cpu.execute();

    EXPECT_EQ(cpu.P.V, 0);
}

void cpu6502_test::add(oc opcode)
{
    cpu.A = 0x02;
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(cpu.A, 0x04);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
}

TEST_F(cpu6502_test, ADC_IMM)
{
    auto opcode = oc::ADC_IMM;

    create_IMM(opcode, 0x01);
    add(opcode);
}

TEST_F(cpu6502_test, ADC_ZPG)
{
    auto opcode = oc::ADC_ZPG;

    bus.write(create_ZPG(opcode), 0x01);
    add(opcode);
}

TEST_F(cpu6502_test, ADC_ZPX)
{
    auto opcode = oc::ADC_ZPX;

    bus.write(create_ZP(opcode, cpu.X), 0x01);
    add(opcode);
}

TEST_F(cpu6502_test, ADC_ABS)
{
    auto opcode = oc::ADC_ABS;

    bus.write(create_ABS(opcode), 0x01);
    add(opcode);
}

TEST_F(cpu6502_test, ADC_ABX)
{
    auto opcode = oc::ADC_ABX;

    bus.write(create_AB(opcode, cpu.X), 0x01);
    add(opcode);
}

TEST_F(cpu6502_test, ADC_ABX_with_carry)
{
    auto opcode = oc::ADC_ABX;

    carry = true;
    bus.write(create_AB(opcode, cpu.X), 0x01);
    add(opcode);
}

TEST_F(cpu6502_test, ADC_ABY)
{
    auto opcode = oc::ADC_ABY;

    bus.write(create_AB(opcode, cpu.Y), 0x01);
    add(opcode);
}

TEST_F(cpu6502_test, ADC_ABY_with_carry)
{
    auto opcode = oc::ADC_ABY;

    carry = true;
    bus.write(create_AB(opcode, cpu.Y), 0x01);
    add(opcode);
}

TEST_F(cpu6502_test, ADC_IDX)
{
    auto opcode = oc::ADC_IDX;

    bus.write(create_IDX(opcode), 0x01);
    add(opcode);
}

TEST_F(cpu6502_test, ADC_IDY)
{
    auto opcode = oc::ADC_IDY;

    bus.write(create_IDY(opcode), 0x01);
    add(opcode);
}

TEST_F(cpu6502_test, SBC_IMM_flag_C_Z)
{
    auto opcode = oc::SBC_IMM;

    create_IMM(opcode, 0x04);
    cpu.A = 0x05;
    cpu.execute();

    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(cpu.P.Z, 1);
}

TEST_F(cpu6502_test, SBC_IMM_flag_N)
{
    auto opcode = oc::SBC_IMM;

    create_IMM(opcode, 0x7E);
    cpu.A = 0x01;
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(cpu.P.N, 1);
}

TEST_F(cpu6502_test, SBC_IMM_flag_V_both_positive)
{
    auto opcode = oc::SBC_IMM;

    create_IMM(opcode, 0x40);
    cpu.A = 0x40;
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(cpu.P.V, 0);
}

TEST_F(cpu6502_test, SBC_IMM_flag_V_both_negative)
{
    auto opcode = oc::SBC_IMM;

    create_IMM(opcode, 0x80);
    cpu.A = 0x80;
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(cpu.P.V, 0);
}

TEST_F(cpu6502_test, SBC_IMM_flag_V_positive_negative)
{
    auto opcode = oc::SBC_IMM;
    create_IMM(opcode, 0x70);
    cpu.A = 0xD0;
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(cpu.P.V, 1);
}

TEST_F(cpu6502_test, SBC_IMM_flag_V_negative_positive)
{
    auto opcode = oc::SBC_IMM;

    create_IMM(opcode, 0x90);
    cpu.A = 0x10;
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(cpu.P.V, 1);
}

void cpu6502_test::substract(oc opcode)
{
    cpu.A = 0x05;
    cpu.P.C = 0;
    cpu.execute();

    EXPECT_EQ(cpu.A, 0x02);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
}

TEST_F(cpu6502_test, SBC_IMM)
{
    auto opcode = oc::SBC_IMM;

    create_IMM(opcode, 0x02);
    substract(opcode);
}

TEST_F(cpu6502_test, SBC_ZPG)
{
    auto opcode = oc::SBC_ZPG;

    bus.write(create_ZPG(opcode), 0x02);
    substract(opcode);
}

TEST_F(cpu6502_test, SBC_ZPX)
{
    auto opcode = oc::SBC_ZPX;

    bus.write(create_ZP(opcode, cpu.X), 0x02);
    substract(opcode);
}

TEST_F(cpu6502_test, SBC_ABS)
{
    auto opcode = oc::SBC_ABS;

    bus.write(create_ABS(opcode), 0x02);
    substract(opcode);
}

TEST_F(cpu6502_test, SBC_ABX)
{
    auto opcode = oc::SBC_ABX;

    bus.write(create_AB(opcode, cpu.X), 0x02);
    substract(opcode);
}

TEST_F(cpu6502_test, SBC_ABX_with_carry)
{
    auto opcode = oc::SBC_ABX;

    carry = true;
    bus.write(create_AB(opcode, cpu.X), 0x02);
    substract(opcode);
}

TEST_F(cpu6502_test, SBC_ABY)
{
    auto opcode = oc::SBC_ABY;

    bus.write(create_AB(opcode, cpu.Y), 0x02);
    substract(opcode);
}

TEST_F(cpu6502_test, SBC_ABY_with_carry)
{
    auto opcode = oc::SBC_ABY;

    carry = true;
    bus.write(create_AB(opcode, cpu.Y), 0x02);
    substract(opcode);
}

TEST_F(cpu6502_test, SBC_IDX)
{
    auto opcode = oc::SBC_IDX;

    bus.write(create_IDX(opcode), 0x02);
    substract(opcode);
}

TEST_F(cpu6502_test, SBC_IDY)
{
    auto opcode = oc::SBC_IDY;

    bus.write(create_IDY(opcode), 0x02);
    substract(opcode);
}

void cpu6502_test::compare(oc opcode, uint8_t& reg)
{
    reg = 0x05;
    cpu.P.C = 0;
    cpu.execute();

    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
}

TEST_F(cpu6502_test, CMP_IMM)
{
    auto opcode = oc::CMP_IMM;

    create_IMM(opcode, 0x02);
    compare(opcode, cpu.A);

    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(cpu.P.Z, 0);
    EXPECT_EQ(cpu.P.N, 0);

    create_IMM(opcode, 0x05);
    cpu.execute();

    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(cpu.P.Z, 1);
    EXPECT_EQ(cpu.P.N, 0);

    create_IMM(opcode, 0x07);
    cpu.execute();

    EXPECT_EQ(cpu.P.C, 0);
    EXPECT_EQ(cpu.P.Z, 0);
    EXPECT_EQ(cpu.P.N, 1);
}

TEST_F(cpu6502_test, CMP_ZPG)
{
    auto opcode = oc::CMP_ZPG;

    bus.write(create_ZPG(opcode), 0x02);
    compare(opcode, cpu.A);
}

TEST_F(cpu6502_test, CMP_ZPX)
{
    auto opcode = oc::CMP_ZPX;

    bus.write(create_ZP(opcode, cpu.X), 0x02);
    compare(opcode, cpu.A);
}

TEST_F(cpu6502_test, CMP_ABS)
{
    auto opcode = oc::CMP_ABS;

    bus.write(create_ABS(opcode), 0x02);
    compare(opcode, cpu.A);
}

TEST_F(cpu6502_test, CMP_ABX)
{
    auto opcode = oc::CMP_ABX;

    bus.write(create_AB(opcode, cpu.X), 0x02);
    compare(opcode, cpu.A);
}

TEST_F(cpu6502_test, CMP_ABX_with_carry)
{
    auto opcode = oc::CMP_ABX;

    carry = true;
    bus.write(create_AB(opcode, cpu.X), 0x02);
    compare(opcode, cpu.A);
}

TEST_F(cpu6502_test, CMP_ABY)
{
    auto opcode = oc::CMP_ABY;

    bus.write(create_AB(opcode, cpu.Y), 0x02);
    compare(opcode, cpu.A);
}

TEST_F(cpu6502_test, CMP_ABY_with_carry)
{
    auto opcode = oc::CMP_ABY;

    carry = true;
    bus.write(create_AB(opcode, cpu.Y), 0x02);
    compare(opcode, cpu.A);
}

TEST_F(cpu6502_test, CMP_IDX)
{
    auto opcode = oc::CMP_IDX;

    bus.write(create_IDX(opcode), 0x02);
    compare(opcode, cpu.A);
}

TEST_F(cpu6502_test, CMP_IDY)
{
    auto opcode = oc::CMP_IDY;

    bus.write(create_IDY(opcode), 0x02);
    compare(opcode, cpu.A);
}

TEST_F(cpu6502_test, CPX_IMM)
{
    auto opcode = oc::CPX_IMM;

    create_IMM(opcode, 0x02);
    compare(opcode, cpu.X);

    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(cpu.P.Z, 0);
    EXPECT_EQ(cpu.P.N, 0);

    create_IMM(opcode, 0x05);
    cpu.execute();

    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(cpu.P.Z, 1);
    EXPECT_EQ(cpu.P.N, 0);

    create_IMM(opcode, 0x07);
    cpu.execute();

    EXPECT_EQ(cpu.P.C, 0);
    EXPECT_EQ(cpu.P.Z, 0);
    EXPECT_EQ(cpu.P.N, 1);
}

TEST_F(cpu6502_test, CPX_ZPG)
{
    auto opcode = oc::CPX_ZPG;

    bus.write(create_ZPG(opcode), 0x02);
    compare(opcode, cpu.X);
}

TEST_F(cpu6502_test, CPX_ABS)
{
    auto opcode = oc::CPX_ABS;

    bus.write(create_ABS(opcode), 0x02);
    compare(opcode, cpu.X);
}

TEST_F(cpu6502_test, CPY_IMM)
{
    auto opcode = oc::CPY_IMM;

    create_IMM(opcode, 0x02);
    compare(opcode, cpu.Y);

    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(cpu.P.Z, 0);
    EXPECT_EQ(cpu.P.N, 0);

    create_IMM(opcode, 0x05);
    cpu.execute();

    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(cpu.P.Z, 1);
    EXPECT_EQ(cpu.P.N, 0);

    create_IMM(opcode, 0x07);
    cpu.execute();

    EXPECT_EQ(cpu.P.C, 0);
    EXPECT_EQ(cpu.P.Z, 0);
    EXPECT_EQ(cpu.P.N, 1);
}

TEST_F(cpu6502_test, CPY_ZPG)
{
    auto opcode = oc::CPY_ZPG;

    bus.write(create_ZPG(opcode), 0x02);
    compare(opcode, cpu.Y);
}

TEST_F(cpu6502_test, CPY_ABS)
{
    auto opcode = oc::CPY_ABS;

    bus.write(create_ABS(opcode), 0x02);
    compare(opcode, cpu.Y);
}

void cpu6502_test::increment(oc opcode, uint16_t address)
{
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0x03);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, INC_ZPG)
{
    auto opcode = oc::INC_ZPG;

    auto address = create_ZPG(opcode);
    bus.write(address, 0x02);
    increment(opcode, address);
}

TEST_F(cpu6502_test, INC_ZPX)
{
    auto opcode = oc::INC_ZPX;

    auto address = create_ZP(opcode, cpu.X);
    bus.write(address, 0x02);
    increment(opcode, address);
}

TEST_F(cpu6502_test, INC_ABS)
{
    auto opcode = oc::INC_ABS;

    auto address = create_ABS(opcode);
    bus.write(address, 0x02);
    increment(opcode, address);
}

TEST_F(cpu6502_test, INC_ABX)
{
    auto opcode = oc::INC_ABX;

    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0x02);
    increment(opcode, address);
}

TEST_F(cpu6502_test, INC_ABX_with_carry)
{
    auto opcode = oc::INC_ABX;

    carry = true;
    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0x02);
    increment(opcode, address);
}

TEST_F(cpu6502_test, INX_IMP)
{
    auto opcode = oc::INX_IMP;

    bus.write(counter++, opcode);
    cpu.X = 0x02;
    cpu.execute();

    EXPECT_EQ(cpu.X, 0x03);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, INY_IMP)
{
    auto opcode = oc::INY_IMP;

    bus.write(counter++, opcode);
    cpu.Y = 0x02;
    cpu.execute();

    EXPECT_EQ(cpu.Y, 0x03);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

void cpu6502_test::decrement(oc opcode, uint16_t address)
{
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0x01);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, DEC_ZPG)
{
    auto opcode = oc::DEC_ZPG;

    auto address = create_ZPG(opcode);
    bus.write(address, 0x02);
    decrement(opcode, address);
}

TEST_F(cpu6502_test, DEC_ZPX)
{
    auto opcode = oc::DEC_ZPX;

    auto address = create_ZP(opcode, cpu.X);
    bus.write(address, 0x02);
    decrement(opcode, address);
}

TEST_F(cpu6502_test, DEC_ABS)
{
    auto opcode = oc::DEC_ABS;

    auto address = create_ABS(opcode);
    bus.write(address, 0x02);
    decrement(opcode, address);
}

TEST_F(cpu6502_test, DEC_ABX)
{
    auto opcode = oc::DEC_ABX;

    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0x02);
    decrement(opcode, address);
}

TEST_F(cpu6502_test, DEC_ABX_with_carry)
{
    auto opcode = oc::DEC_ABX;

    carry = true;
    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0x02);
    decrement(opcode, address);
}

TEST_F(cpu6502_test, DEX_IMP)
{
    auto opcode = oc::DEX_IMP;

    bus.write(counter++, opcode);
    cpu.X = 0x02;
    cpu.execute();

    EXPECT_EQ(cpu.X, 0x01);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, DEY_IMP)
{
    auto opcode = oc::DEY_IMP;

    bus.write(counter++, opcode);
    cpu.Y = 0x02;
    cpu.execute();

    EXPECT_EQ(cpu.Y, 0x01);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, ASL_ACC)
{
    auto opcode = oc::ASL_ACC;

    bus.write(counter++, opcode);
    cpu.A = 0b10000001;
    cpu.execute();

    EXPECT_EQ(cpu.A, 0b00000010);
    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

    bus.write(counter++, opcode);
    cpu.A = 0b10000000;
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 1);

    bus.write(counter++, opcode);
    cpu.A = 0b01000000;
    cpu.execute();

    EXPECT_EQ(cpu.P.N, 1);
}

void cpu6502_test::asl(oc opcode, uint16_t address)
{
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0b00000010);
    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, ASL_ZPG)
{
    auto opcode = oc::ASL_ZPG;

    auto address = create_ZPG(opcode);
    bus.write(address, 0b10000001);
    asl(opcode, address);
}

TEST_F(cpu6502_test, ASL_ZPX)
{
    auto opcode = oc::ASL_ZPX;

    auto address = create_ZP(opcode, cpu.X);
    bus.write(address, 0b10000001);
    asl(opcode, address);
}

TEST_F(cpu6502_test, ASL_ABS)
{
    auto opcode = oc::ASL_ABS;

    auto address = create_ABS(opcode);
    bus.write(address, 0b10000001);
    asl(opcode, address);
}

TEST_F(cpu6502_test, ASL_ABX)
{
    auto opcode = oc::ASL_ABX;

    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0b10000001);
    asl(opcode, address);
}

TEST_F(cpu6502_test, ASL_ABX_with_carry)
{
    auto opcode = oc::ASL_ABX;

    carry = true;
    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0b10000001);
    asl(opcode, address);
}

TEST_F(cpu6502_test, LSR_ACC)
{
    auto opcode = oc::LSR_ACC;

    bus.write(counter++, opcode);
    cpu.A = 0b10000001;
    cpu.execute();

    EXPECT_EQ(cpu.A, 0b01000000);
    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

    bus.write(counter++, opcode);
    cpu.A = 0b00000001;
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 1);
}

void cpu6502_test::lsr(oc opcode, uint16_t address)
{
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0b01000000);
    EXPECT_EQ(cpu.P.C, 1);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, LSR_ZPG)
{
    auto opcode = oc::LSR_ZPG;

    auto address = create_ZPG(opcode);
    bus.write(address, 0b10000001);
    lsr(opcode, address);
}

TEST_F(cpu6502_test, LSR_ZPX)
{
    auto opcode = oc::LSR_ZPX;

    auto address = create_ZP(opcode, cpu.X);
    bus.write(address, 0b10000001);
    lsr(opcode, address);
}

TEST_F(cpu6502_test, LSR_ABS)
{
    auto opcode = oc::LSR_ABS;

    auto address = create_ABS(opcode);
    bus.write(address, 0b10000001);
    lsr(opcode, address);
}

TEST_F(cpu6502_test, LSR_ABX)
{
    auto opcode = oc::LSR_ABX;

    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0b10000001);
    lsr(opcode, address);
}

TEST_F(cpu6502_test, LSR_ABX_with_carry)
{
    auto opcode = oc::LSR_ABX;

    carry = true;
    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0b10000001);
    lsr(opcode, address);
}

TEST_F(cpu6502_test, ROL_ACC)
{
    auto opcode = oc::ROL_ACC;

    bus.write(counter++, opcode);
    cpu.A = 0b00000001;
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(cpu.A, 0b00000011);
    EXPECT_EQ(cpu.P.C, 0);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

    bus.write(counter++, opcode);
    cpu.A = 0b10000000;
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 1);

    bus.write(counter++, opcode);
    cpu.A = 0b01000000;
    cpu.execute();

    EXPECT_EQ(cpu.P.N, 1);
}

void cpu6502_test::rol(oc opcode, uint16_t address)
{
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0b00000011);
    EXPECT_EQ(cpu.P.C, 0);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, ROL_ZPG)
{
    auto opcode = oc::ROL_ZPG;

    auto address = create_ZPG(opcode);
    bus.write(address, 0b00000001);
    rol(opcode, address);
}

TEST_F(cpu6502_test, ROL_ZPX)
{
    auto opcode = oc::ROL_ZPX;

    auto address = create_ZP(opcode, cpu.X);
    bus.write(address, 0b00000001);
    rol(opcode, address);
}

TEST_F(cpu6502_test, ROL_ABS)
{
    auto opcode = oc::ROL_ABS;

    auto address = create_ABS(opcode);
    bus.write(address, 0b00000001);
    rol(opcode, address);
}

TEST_F(cpu6502_test, ROL_ABX)
{
    auto opcode = oc::ROL_ABX;

    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0b00000001);
    rol(opcode, address);
}

TEST_F(cpu6502_test, ROL_ABX_with_carry)
{
    auto opcode = oc::ROL_ABX;

    carry = true;
    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0b00000001);
    rol(opcode, address);
}

TEST_F(cpu6502_test, ROR_ACC)
{
    auto opcode = oc::ROR_ACC;

    bus.write(counter++, opcode);
    cpu.A = 0b10000000;
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(cpu.A, 0b11000000);
    EXPECT_EQ(cpu.P.C, 0);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

    bus.write(counter++, opcode);
    cpu.A = 0b00000001;
    cpu.P.C = 0;
    cpu.execute();

    EXPECT_EQ(cpu.P.Z, 1);

    bus.write(counter++, opcode);
    cpu.A = 0b00000000;
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(cpu.P.N, 1);
}

void cpu6502_test::ror(oc opcode, uint16_t address)
{
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(bus.read(address), 0b11000000);
    EXPECT_EQ(cpu.P.C, 0);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, ROR_ZPG)
{
    auto opcode = oc::ROR_ZPG;

    auto address = create_ZPG(opcode);
    bus.write(address, 0b10000000);
    ror(opcode, address);
}

TEST_F(cpu6502_test, ROR_ZPX)
{
    auto opcode = oc::ROR_ZPX;

    auto address = create_ZP(opcode, cpu.X);
    bus.write(address, 0b10000000);
    ror(opcode, address);
}

TEST_F(cpu6502_test, ROR_ABS)
{
    auto opcode = oc::ROR_ABS;

    auto address = create_ABS(opcode);
    bus.write(address, 0b10000000);
    ror(opcode, address);
}

TEST_F(cpu6502_test, ROR_ABX)
{
    auto opcode = oc::ROR_ABX;

    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0b10000000);
    ror(opcode, address);
}

TEST_F(cpu6502_test, ROR_ABX_with_carry)
{
    auto opcode = oc::ROR_ABX;

    carry = true;
    auto address = create_AB(opcode, cpu.X);
    bus.write(address, 0b10000000);
    ror(opcode, address);
}

TEST_F(cpu6502_test, JMP_ABS)
{
    auto opcode = oc::JMP_ABS;

    auto address = create_ABS(opcode);
    cpu.execute();

    EXPECT_EQ(cpu.PC, address);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, JMP_IND)
{
    auto opcode = oc::JMP_IND;

    auto address = create_IND(opcode);
    cpu.execute();

    EXPECT_EQ(cpu.PC, address);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, JSR____)
{
    auto opcode = oc::JSR____;

    auto address = create_ABS(opcode);
    cpu.execute();

    EXPECT_EQ(cpu.S, 0xFD);
    EXPECT_EQ(bus.read(0x01FF), 0x02);
    EXPECT_EQ(bus.read(0x01FE), 0x02);
    EXPECT_EQ(cpu.PC, address);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, RTS_IMP)
{
    auto opcode = oc::RTS_IMP;

    cpu.PC = 0x4080;
    bus.write(cpu.PC, opcode);
    bus.write(0x01FF, 0x02);
    bus.write(0x01FE, 0x03);
    cpu.S = 0xFD;
    cpu.execute();

    EXPECT_EQ(cpu.S, 0xFF);
    EXPECT_EQ(cpu.PC, 0x0204);
    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, BCS____)
{
    auto opcode = oc::BCS____;

    bus.write(counter++, opcode);
    bus.write(counter++, 0x00);
    cpu.execute();

    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

    clock.reset();
    bus.write(counter++, opcode);
    bus.write(counter++, 0x00);
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + 1);

    clock.reset();
    bus.write(counter++, opcode);
    bus.write(counter++, 0x80);
    cpu.P.C = 1;
    cpu.execute();

    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + 2);
}

TEST_F(cpu6502_test, BRK____)
{
    auto opcode = oc::BRK____;

    bus.write(cpu.PC, opcode);
    cpu.execute();

    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, RTI_IMP)
{
    auto opcode = oc::RTI_IMP;

    bus.write(cpu.PC, opcode);
    cpu.execute();

    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, NOP_IMP)
{
    auto opcode = oc::NOP_IMP;

    bus.write(cpu.PC, opcode);
    cpu.execute();

    EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
}

TEST_F(cpu6502_test, RUN)
{
    bus.load_file("C:/Users/rafal/Source/cpu6502/docs/6502_65C02_functional_tests-master/6502_functional_test.bin");
    clock.set_timing(0);
    cpu.PC = 0x0400;
    cpu.run(0x3699);
}

}