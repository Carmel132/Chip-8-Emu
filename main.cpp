#include <iostream>
#include <ctime>
#include "resource/file.h"
#include "process.h"
int main() {
    std::srand(std::time(0));
    
    auto instr = read_bin("space_invaders.ch8");
    Memory mem{};
    for (int i = 0; i < instr.size() - 1; i += 2) {
        uint16_t j = (instr[i] << 8) + instr[i + 1];
        interpret_instruction(j, &mem);
    }
    std::cout << mem.registers[0];
    std::cout << instr.size();
    return 0;
}