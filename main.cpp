#include <iostream>
#include <ctime>
#include "resource/file.h"
#include "process.h"
int main() {
    std::srand(std::time(0));

    Memory mem{};

    std::vector<uint8_t> instr = read_bin("programs/space_invaders.ch8");
    load_program_bytes(&mem, instr);

    for(;;) {
        // Fetch
        uint16_t instruction = get_instr_at_PC(&mem);
        //std::cout << std::hex << instruction << "\n";
        mem.program_counter += 2;
        // Decode
        interpret_instruction(instruction, & mem);
    }
    print_screen(&mem);
    std::cout << mem.registers[0];
    std::cout << instr.size();
    return 0;
}