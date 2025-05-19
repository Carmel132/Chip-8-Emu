#pragma once
#include <iostream>

struct Memory {
    
    /*
        0x000-0x1FF - interpreter (unused)
        0x200 - typical start of program (some use 0x600)
        0xFFF - end of RAM
    */
    uint8_t memory[0x1000];
    // VF should not be used by any program as it is used as a flag for some instructions
    uint8_t registers[0x10];
    uint16_t stack[0x10];
    uint16_t I_Register, program_counter;
    uint8_t delay_timer, sound_timer, stack_pointer;
};