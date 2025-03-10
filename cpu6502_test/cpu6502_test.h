#include "../cpu6502/clock.h"
#include "../cpu6502/cpu6502.h"
#include "../cpu6502/memory64k.h"
#include "gtest/gtest.h"

namespace emulator {

class cpu6502_test : public testing::Test {
public:
    using oc = emulator::cpu6502::opcode;

    emulator::clock clock;
    emulator::memory64k bus;
    emulator::cpu6502 cpu { clock, bus };
    uint16_t counter = {};
    bool carry = false;

    void SetUp() override;

    void create_IMM(oc opcode, uint8_t data);
    uint16_t create_ZPG(oc opcode);
    uint16_t create_ZP(oc opcode, uint8_t& offset_reg);
    uint16_t create_ABS(oc opcode);
    uint16_t create_AB(oc opcode, uint8_t& offset_reg);
    uint16_t create_IND(oc opcode);
    uint16_t create_IDX(oc opcode);
    uint16_t create_IDY(oc opcode);

    void load_IMM(oc opcode, uint8_t& reg);
    void load_ZPG(oc opcode, uint8_t& reg);
    void load_ZP(oc opcode, uint8_t& reg, uint8_t& offset_reg);
    void load_ABS(oc opcode, uint8_t& reg);
    void load_AB(oc opcode, uint8_t& reg, uint8_t& offset_reg);
    void load_IDX(oc opcode, uint8_t& reg);
    void load_IDY(oc opcode, uint8_t& reg);

    void store_ZPG(oc opcode, uint8_t& reg);
    void store_ZP(oc opcode, uint8_t& reg, uint8_t& offset_reg);
    void store_ABS(oc opcode, uint8_t& reg);
    void store_AB(oc opcode, uint8_t& reg, uint8_t& offset_reg);
    void store_IDX(oc opcode, uint8_t& reg);
    void store_IDY(oc opcode, uint8_t& reg);

    void transfer(oc opcode, uint8_t& src, uint8_t& dst, bool check_Z_N = true);
    void logic(oc opcode, uint8_t a, uint8_t b);
    void add(oc opcode);
    void substract(oc opcode);
    void compare(oc opcode, uint8_t& reg);
    void increment(oc opcode, uint16_t addr);
    void decrement(oc opcode, uint16_t addr);
    void asl(oc opcode, uint16_t addr);
    void lsr(oc opcode, uint16_t addr);
    void rol(oc opcode, uint16_t addr);
    void ror(oc opcode, uint16_t addr);
};

}