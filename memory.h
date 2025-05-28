#pragma once
#include <iostream>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include "resource/stack.h"
#define GRAPHIC_WIDTH 64
#define GRAPHIC_HEIGHT 32

struct Memory {
    
    /*
        0x000-0x1FF - interpreter (unused)
        0x200 - typical start of program (some use 0x600)
        0xFFF - end of RAM
    */
    uint8_t memory[0x1000] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0xF0, 0x80, 0xF0, 
        0xF0, 0x80, 0xF0, 0x80, 0x80
    };
    // VF should not be used by any program as it is used as a flag for some instructions
    uint8_t registers[0x10]{};
    
    Stack<uint16_t, 0x10> stack{};
    uint16_t I_register{}, program_counter{0x200};
    uint8_t delay_timer{}, sound_timer{}/*, stack_pointer*/;
    std::bitset<GRAPHIC_WIDTH> graphic[GRAPHIC_HEIGHT]{};
    std::bitset<16> keyboard{};
};

void load_program_bytes(Memory* mem, std::vector<uint8_t> program, uint16_t start = 0x200) {
    for (int i = 0; i < program.size(); ++i) {
        mem->memory[start + i] = program[i];
    }
}

void print_screen(const Memory* mem) {
    system("cls");
    char buf[GRAPHIC_WIDTH + 1]{};
    buf[GRAPHIC_WIDTH] = '\0';
    for (int i = 0; i < GRAPHIC_HEIGHT; i++) {
        for (int j = 0; j < GRAPHIC_WIDTH; j++)
        {
            buf[j] = mem->graphic[i][j] ? '#' : ' ';
        }

        std::cout << buf << "\n";
    }
}

void tick(Memory* mem) {
    if (mem->delay_timer > 0) {
        mem->delay_timer--;
    }
    if (mem->sound_timer > 0) {
        mem->sound_timer--;
    }
}