#include "../cpu6502/clock.h"
#include "../cpu6502/cpu6502.h"
#include "../cpu6502/memory64k.h"

int main(int argc, char** argv)
{
    using oc = emulator::cpu6502::opcode;

    emulator::clock clock;
    emulator::memory64k bus;
    emulator::cpu6502 cpu { clock, bus };

    bus.load_file("C:/Users/rafal/Source/cpu6502/docs/6502_65C02_functional_tests-master/6502_functional_test.bin");
    clock.set_timing(0);
    cpu.PC = 0x0400;

    auto start = std::chrono::steady_clock::now();

    cpu.run(0x3699);

    auto end = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<
        std::chrono::duration<double>>(end - start)
                                 .count();
    std::cout << elapsed_seconds << '\n';
}