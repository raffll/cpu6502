#include "cpu6502.h"

namespace emulator {

cpu6502::cpu6502(i_clock& clock, i_bus& bus)
    : clock(clock)
    , bus(bus)
{
}

void cpu6502::log(bool show)
{
    if (!show)
        return;

    static size_t last_cycle {};
    std::string cycle = (clock.get_cycles() == last_cycle)
        ? std::string(std::to_string(clock.get_cycles()).size(), '-')
        : std::to_string(clock.get_cycles());

    std::stringstream ss;
    ss << "oc: " << instructions.at(oc).name
       << " cycle: " << cycle
       << " r/w: " << control
       << " address: " << std::format("{:#06x}", address)
       << " data: " << std::format("{:#04x}", data)
       << " A: " << std::format("{:#04x}", A)
       << " X: " << std::format("{:#04x}", X)
       << " Y: " << std::format("{:#04x}", Y)
       << " S: " << std::format("{:#04x}", S)
       << " NV_BDIZC: "
       << std::to_string(P.N)
       << std::to_string(P.V)
       << std::to_string(P._)
       << std::to_string(P.B)
       << std::to_string(P.D)
       << std::to_string(P.I)
       << std::to_string(P.Z)
       << std::to_string(P.C)
       << " PC: " << std::format("{:#06x}", PC)
       << "\n";

    last_cycle = clock.get_cycles();
    std::printf(ss.str().c_str());
}

void cpu6502::run(uint16_t stop)
{
    while (address != stop) {
        execute();
    }
    log(true);
}

void cpu6502::cycle()
{
    clock.cycle();
    log(false);
}

uint8_t cpu6502::read()
{
    control = true;
    data = bus.read(address);
    return data;
}

void cpu6502::write()
{
    control = false;
    bus.write(address, data);
}

static bool is_zero(uint8_t byte)
{
    return byte == 0;
}

static bool is_negative(uint8_t byte)
{
    return (byte & (1 << 7)) != 0;
}

static bool is_bit_set(uint8_t byte, uint8_t bit)
{
    return (byte & (1 << bit)) != 0;
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

static cpu6502::status status_byte(uint8_t byte)
{
    auto status = std::bit_cast<cpu6502::status>(byte);
    status.B = 1;
    status._ = 1;
    return status;
}

void cpu6502::execute()
{
    add_cycle = cpu6502::cycle_mode::never;
    add_carry = false;
    acc_addressing = false;

    address = PC;
    PC++;
    oc = read();
    cycle();

    instructions[oc].addressing(*this);
    instructions[oc].operation(*this);
}

void cpu6502::reset()
{
    oc = {};

    A = {};
    X = {};
    Y = {};

    PC = 0x0200;

    S = 0xFF;
    P = {};

    address = {};
    data = {};
}

void cpu6502::add_register(uint8_t& byte, uint8_t reg)
{
    byte += reg;
    add_carry = byte < reg;
}

void cpu6502::ACC()
{
    acc_addressing = true;
    IMP();
}

void cpu6502::IMP()
{
    address = PC;
    read();
    cycle();
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
    uint8_t adl = read();
    cycle();

    address = adl;
}

void cpu6502::zero_page(uint8_t reg)
{
    address = PC;
    PC++;
    uint8_t bal = read();
    cycle();

    address = bal;
    add_register(bal, reg);
    cycle();

    address = bal;
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
    uint8_t adl = read();
    cycle();

    address = PC;
    PC++;
    uint8_t adh = read();
    cycle();

    address = (adh << 8) | adl;
}

void cpu6502::absolute(uint8_t reg)
{
    add_cycle = cycle_mode::if_carry;

    address = PC;
    PC++;
    uint8_t bal = read();
    cycle();

    address = PC;
    PC++;
    add_register(bal, reg);
    uint8_t bah = read();
    cycle();

    address = (bah << 8) | bal;
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
    uint8_t ial = read();
    cycle();

    address = PC;
    PC++;
    uint8_t iah = read();
    cycle();

    address = (iah << 8) | ial;
    uint8_t adl = read();
    cycle();

    address = (iah << 8) | (ial + 1);
    uint8_t adh = read();
    cycle();

    address = (adh << 8) | adl;
}

void cpu6502::IDX()
{
    address = PC;
    PC++;
    uint8_t bal = read();
    cycle();

    address = bal;
    add_register(bal, X);
    cycle();

    address = bal;
    uint8_t adl = read();
    cycle();

    address = bal + 1;
    uint8_t adh = read();
    cycle();

    address = (adh << 8) | adl;
}

void cpu6502::IDY()
{
    add_cycle = cycle_mode::if_carry;

    address = PC;
    PC++;
    uint8_t ial = read();
    cycle();

    address = ial;
    uint8_t bal = read();
    cycle();

    address = ial + 1;
    add_register(bal, Y);
    uint8_t bah = read();
    cycle();

    address = (bah << 8) | bal;
}

void cpu6502::extra_cycle()
{
    switch (add_cycle) {
    case cycle_mode::never:
        break;

    case cycle_mode::if_carry:
        if (add_carry) {
            address += 0x0100;
            read();
            cycle();
        }
        break;

    case cycle_mode::if_carry_possible:
        if (add_carry) {
            address += 0x0100;
        }
        read();
        cycle();
        break;

    case cycle_mode::always:
        read();
        cycle();
        break;

    default:
        throw std::exception("");
    }
}

void cpu6502::load()
{
    read();
    cycle();

    extra_cycle();
}

void cpu6502::LDA()
{
    load();
    A = data;
    P.Z = is_zero(A);
    P.N = is_negative(A);
}

void cpu6502::LDX()
{
    load();
    X = data;
    P.Z = is_zero(X);
    P.N = is_negative(X);
}

void cpu6502::LDY()
{
    load();
    Y = data;
    P.Z = is_zero(Y);
    P.N = is_negative(Y);
}

void cpu6502::store(uint8_t reg)
{
    if (add_cycle == cycle_mode::if_carry)
        add_cycle = cycle_mode::if_carry_possible;

    extra_cycle();

    data = reg;
    write();
    cycle();
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

void cpu6502::push()
{
    address = stack_address(S);
    S--;
    write();
    cycle();
}

void cpu6502::PHA()
{
    data = A;
    push();
};

void cpu6502::PHP()
{
    data = status_byte(P);
    push();
};

void cpu6502::pull()
{
    address = stack_address(S);
    S++;
    cycle();

    address = stack_address(S);
    read();
    cycle();
}

void cpu6502::PLA()
{
    pull();
    A = data;
    P.Z = is_zero(A);
    P.N = is_negative(A);
};

void cpu6502::PLP()
{
    pull();
    P = status_byte(data);
};

void cpu6502::AND()
{
    load();
    A = A & data;
    P.Z = is_zero(A);
    P.N = is_negative(A);
};

void cpu6502::EOR()
{
    load();
    A = A ^ data;
    P.Z = is_zero(A);
    P.N = is_negative(A);
};

void cpu6502::ORA()
{
    load();
    A = A | data;
    P.Z = is_zero(A);
    P.N = is_negative(A);
};

void cpu6502::BIT()
{
    load();
    P.Z = is_zero(A & data);
    P.N = is_negative(data);
    P.V = is_bit_set(data, 6);
};

void cpu6502::add(bool substract)
{
    uint8_t byte = data;

    if (substract)
        byte = ~byte;

    uint16_t result = A + byte + P.C;

    P.C = result > 0xFF;
    uint8_t lo_result = static_cast<uint8_t>(result);
    P.V = ((A ^ lo_result) & (byte ^ lo_result) & 0x80) != 0;

    A = lo_result;
    P.Z = is_zero(A);
    P.N = is_negative(A);
}

void cpu6502::ADC()
{
    load();
    add(false);
};

void cpu6502::SBC()
{
    load();
    add(true);
};

void cpu6502::compare(uint8_t reg)
{
    load();
    P.C = reg >= data;
    P.Z = is_zero(reg - data);
    P.N = is_negative(reg - data);
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
    auto inc = [&](auto& reg) {
        reg++;
    };

    modify(inc);

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
    auto dec = [&](auto& reg) {
        reg--;
    };

    modify(dec);

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

    modify(asl);
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

    modify(lsr);
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

    modify(rol);
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

    modify(ror);
};

void cpu6502::JMP()
{
    PC = address;
};

void cpu6502::JSR()
{
    address = PC;
    PC++;
    uint8_t adl = read();
    cycle();

    address = stack_address(S);
    cycle();

    address = stack_address(S);
    S--;
    data = hi_byte(PC);
    write();
    cycle();

    address = stack_address(S);
    S--;
    data = lo_byte(PC);
    write();
    cycle();

    address = PC;
    uint8_t adh = read();
    PC = (adh << 8) | adl;
    cycle();
};

void cpu6502::RTS()
{
    address = stack_address(S);
    S++;
    cycle();

    address = stack_address(S);
    S++;
    uint8_t pcl = read();
    cycle();

    address = stack_address(S);
    uint8_t pch = read();
    PC = (pch << 8) | pcl;
    cycle();

    address = PC;
    PC++;
    cycle();
};

void cpu6502::branch(bool is_branch)
{
    address = PC;
    PC++;
    int8_t offset = read(); // signed
    cycle();

    if (!is_branch)
        return;

    uint8_t pch = hi_byte(PC);
    uint8_t pcl = lo_byte(PC);

    uint16_t result = PC;
    result += offset;
    uint8_t hi = hi_byte(result);
    uint8_t lo = lo_byte(result);

    PC = (pch << 8) | lo;
    address = PC;
    cycle();

    if (hi == pch)
        return;

    PC = (hi << 8) | lo;
    address = PC;
    cycle();
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

void cpu6502::RTI()
{
    address = stack_address(S);
    S++;
    cycle();

    address = stack_address(S);
    S++;
    P = status_byte(read());
    cycle();

    address = stack_address(S);
    S++;
    uint8_t pcl = read();
    cycle();

    address = stack_address(S);
    uint8_t pch = read();
    PC = (pch << 8) | pcl;
    cycle();
};

void cpu6502::interrupt(const interrupt_vec& vec)
{
    address = PC;
    PC++;
    cycle();

    address = stack_address(S);
    S--;
    data = hi_byte(PC);
    write();
    cycle();

    address = stack_address(S);
    S--;
    data = lo_byte(PC);
    write();
    cycle();

    address = stack_address(S);
    S--;
    data = status_byte(P);
    write();
    cycle();

    address = vec.first;
    uint8_t adl = read();
    cycle();

    address = vec.second;
    uint8_t adh = read();
    PC = (adh << 8) | adl;
    cycle();

    P.I = 1;
};

void cpu6502::BRK()
{
    interrupt(IRQ_VEC);
};

void IRQ() {};
void NMI() {};

}