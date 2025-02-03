#include "cpu6502.h"

namespace emulator {

cpu6502::cpu6502(i_clock& clock, i_bus& bus)
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
    try {
        while (true) {
            execute();
        }
    } catch (const std::exception&) {
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

static bool is_zero(uint8_t data)
{
    return data == 0;
}

static bool is_negative(uint8_t data)
{
    return (data & (1 << 7)) != 0;
}

static bool is_bit_set(uint8_t data, uint8_t bit)
{
    return (data & (1 << bit)) != 0;
}

static uint8_t lo_byte(uint16_t word)
{
    return word & 0xFF;
}

static uint8_t hi_byte(uint16_t word)
{
    return (word >> 8) & 0xFF;
}

static uint16_t stack_address(uint8_t S)
{
    return 0x0100 + S;
}

static uint8_t status_byte(cpu6502::status P)
{
    return std::bit_cast<uint8_t>(P);
}

void cpu6502::ACC()
{
    acc_addressing = true;
    address = PC;
    clock.cycle();
}

void cpu6502::IMP()
{
    address = PC;
    clock.cycle();
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

void cpu6502::REL()
{
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
    lo = add_register(lo, offset, true);
    uint8_t hi = bus.read(address);
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
    lo = add_register(lo, Y, true);
    uint8_t hi = bus.read(address);
    clock.cycle();

    address = (hi << 8) | lo;
}

uint8_t cpu6502::load(extra_cycle e)
{
    uint8_t data = bus.read(address);
    clock.cycle();

    if (e == extra_cycle::if_carry) {
        if (add_carry) {
            address += 0x0100;
            data = bus.read(address);
            clock.cycle();
        }
    } else if (e == extra_cycle::if_carry_possible) {
        if (add_carry) {
            address += 0x0100;
            data = bus.read(address);
        }

        if (add_cycle)
            clock.cycle();
    } else {
        throw std::exception("");
    }

    return data;
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

void cpu6502::store(uint8_t data, extra_cycle e)
{
    if (e == extra_cycle::if_carry_possible) {
        if (add_carry)
            address += 0x0100;

        if (add_cycle)
            clock.cycle();
    } else if (e == extra_cycle::always) {
        clock.cycle();
    } else {
        throw std::exception("");
    }

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

void cpu6502::transfer(uint8_t src, uint8_t& dst)
{
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
    S = X;
};

void cpu6502::push(uint8_t data)
{
    address = stack_address(S);
    S--;
    bus.write(address, data);
    clock.cycle();
}

void cpu6502::PHA()
{
    push(A);
};

void cpu6502::PHP()
{
    push(status_byte(P));
};

uint8_t cpu6502::pull()
{
    address = stack_address(S);
    S++;
    clock.cycle();

    address = stack_address(S);
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
    P.V = is_bit_set(data, 6);
};

void cpu6502::add_or_substract(uint8_t data)
{
    uint16_t word = A + data + P.C;

    P.C = word > 0xFF;
    uint8_t byte = static_cast<uint8_t>(word);
    P.V = ((A ^ byte) & (data ^ byte) & 0x80) != 0;

    A = byte;
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
    uint8_t data = load(extra_cycle::if_carry_possible);

    data++;
    clock.cycle();

    bus.write(address, data);
    clock.cycle();

    P.Z = is_zero(data);
    P.N = is_negative(data);
};

void cpu6502::increment(uint8_t& reg)
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

void cpu6502::decrement(uint8_t& reg)
{
    reg--;
    P.Z = is_zero(reg);
    P.N = is_negative(reg);
}

void cpu6502::DEC()
{
    uint8_t data = load(extra_cycle::if_carry_possible);

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
    auto asl = [&](auto& reg) {
        auto is_carry = is_bit_set(reg, 7);
        reg = reg << 1;
        P.C = is_carry;
        P.Z = is_zero(reg);
        P.N = is_negative(reg);
    };

    shift(asl, acc_addressing);
};

void cpu6502::LSR()
{
    auto lsr = [&](auto& reg) {
        auto is_carry = is_bit_set(reg, 0);
        reg = reg >> 1;
        P.C = is_carry;
        P.Z = is_zero(reg);
        P.N = is_negative(reg);
    };

    shift(lsr, acc_addressing);
};

void cpu6502::ROL()
{
    auto rol = [&](auto& reg) {
        auto is_carry = is_bit_set(reg, 7);
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
    auto ror = [&](auto& reg) {
        auto is_carry = is_bit_set(reg, 0);
        reg = reg >> 1;
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

void cpu6502::JSR()
{
    address = stack_address(S);
    clock.cycle();

    address = stack_address(S);
    S--;
    bus.write(address, hi_byte(PC));
    clock.cycle();

    address = stack_address(S);
    S--;
    bus.write(address, lo_byte(PC));
    clock.cycle();
};

void cpu6502::RTS()
{
    address = stack_address(S);
    S++;
    clock.cycle();

    address = stack_address(S);
    S++;
    uint8_t lo = bus.read(address);
    clock.cycle();

    address = stack_address(S);
    uint8_t hi = bus.read(address);
    clock.cycle();

    address = (hi << 8) | lo;
    PC++;
    clock.cycle();
};

void cpu6502::branch(bool is_branch)
{
    address = PC;
    PC++;
    int8_t offset = bus.read(address);
    clock.cycle();

    if (!is_branch)
        return;

    address = PC;
    PC += offset;
    clock.cycle();

    if (lo_byte(PC) >= offset)
        return;

    address = PC;
    clock.cycle();
}

void cpu6502::BCC()
{
    branch(!P.C);
};

void cpu6502::BCS()
{
    branch(P.C);
};

void cpu6502::BNE()
{
    branch(!P.Z);
};

void cpu6502::BEQ()
{
    branch(P.Z);
};

void cpu6502::BPL()
{
    branch(!P.N);
};

void cpu6502::BMI()
{
    branch(P.N);
};

void cpu6502::BVC()
{
    branch(!P.V);
};

void cpu6502::BVS()
{
    branch(P.V);
};

void cpu6502::CLC()
{
    P.C = 0;
};

void cpu6502::CLD()
{
    P.D = 0;
};

void cpu6502::CLI()
{
    P.I = 0;
};

void cpu6502::CLV()
{
    P.V = 0;
};

void cpu6502::SEC()
{
    P.C = 1;
};

void cpu6502::SED()
{
    P.D = 1;
};

void cpu6502::SEI()
{
    P.I = 1;
};

void cpu6502::BRK()
{
    address = stack_address(S);
    S--;
    P.B = 1;
    bus.write(address, hi_byte(PC));
    clock.cycle();

    address = stack_address(S);
    S--;
    bus.write(address, lo_byte(PC));
    clock.cycle();

    address = stack_address(S);
    S--;
    bus.write(address, status_byte(P));
    clock.cycle();

    address = 0xFFFE;
    uint8_t lo = bus.read(address);
    clock.cycle();

    address = 0xFFFF;
    uint8_t hi = bus.read(address);
    clock.cycle();

    PC = (hi << 8) | lo;
};

void cpu6502::NOP() {};

void cpu6502::RTI() {};

}