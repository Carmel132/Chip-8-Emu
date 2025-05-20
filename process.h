#pragma once
#include "memory.h"
#include "resource/random.h"

void clear_display(Memory* mem) {
    return;
    for (int i = 0; i < GRAPHIC_HEIGHT; ++i) {
        for (int j = 0; j < GRAPHIC_WIDTH; ++j) {
            mem->graphic[i][j] = 0;
        }
    }
    mem->program_counter++;
}

void return_from_subroutine(Memory* mem) {
    mem->program_counter = mem->stack.pop();
}

void jump_machine_code_routine(Memory* mem, uint16_t addr) {
 // Implementation not shown
}

void set_program_counter(Memory* mem, uint16_t addr) {
    mem->program_counter = addr;
}

void call_subroutine(Memory* mem, uint16_t addr) {
    mem->stack.add(addr);
    mem->program_counter = addr;
}

void skip_next_equal(Memory* mem, uint8_t reg, uint8_t byte) {
    if (mem->registers[reg] == byte) {
        mem->program_counter += 2;
    }
    else {
        mem->program_counter++;
    }
}

void skip_next_not_equal(Memory* mem, uint8_t reg, uint8_t byte) {
    if (mem->registers[reg] != byte) {
        mem->program_counter += 2;
    }
    else {
        mem->program_counter++;
    }
}

void skip_next_reg_equal(Memory* mem, uint8_t reg1, uint8_t reg2) {
    if (mem->registers[reg1] == mem->registers[reg2]) {
        mem->program_counter += 2;
    }
    else {
        mem->program_counter++;
    }
}

void insert_reg_byte(Memory* mem, uint8_t reg, uint8_t byte) {
    mem->registers[reg] = byte;
    mem->program_counter++;
}

void add_reg_byte(Memory* mem, uint8_t reg, uint8_t byte) {
    mem->registers[reg] += byte;
    mem->program_counter++;
}

void insert_reg_reg(Memory* mem, uint8_t reg1, uint8_t reg2) {
    mem->registers[reg1] = mem->registers[reg2];
    mem->program_counter++;
}

void or_reg(Memory* mem, uint8_t reg1, uint8_t reg2) {
    mem->registers[reg1] |= mem->registers[reg2];
    mem->program_counter++;
}

void and_reg(Memory* mem, uint8_t reg1, uint8_t reg2) {
    mem->registers[reg1] &= mem->registers[reg2];
    mem->program_counter++;
}

void xor_reg(Memory* mem, uint8_t reg1, uint8_t reg2) {
    mem->registers[reg1] ^= mem->registers[reg2];
    mem->program_counter++;
}

void add_reg(Memory* mem, uint8_t reg1, uint8_t reg2) {
    uint16_t res = mem->registers[reg1] + mem->registers[reg2];
    mem->registers[reg1] = res % 0x100;
    if (res >= 0x100) {
        mem->registers[0xF] = 1;
    }
    else {
        mem->registers[0xF] = 0;
    }
    mem->program_counter++;
}

void sub_reg(Memory* mem, uint8_t reg1, uint8_t reg2) {
    uint16_t res = mem->registers[reg1] - mem->registers[reg2];
    if (mem->registers[reg1] > mem->registers[reg2]) {
        mem->registers[0xF] = 1;
    }
    else {
        mem->registers[0xF] = 0;
    }

    mem->registers[reg1] = res % 0x100;
    mem->program_counter++;
}

void shift_right(Memory* mem, uint8_t reg1, uint8_t _) {
    if (mem->registers[reg1] & 0x1) {
        mem->registers[0xF] = 1;
    }
    else {
        mem->registers[0xF] = 0;
    }
    mem->registers[reg1] /= 2;
    mem->program_counter++;
}

void subn_reg(Memory* mem, uint8_t reg1, uint8_t reg2) {
    mem->registers[0xF] = mem->registers[reg2] > mem->registers[reg1];
    
    mem->registers[reg1] = mem->registers[reg2] - mem->registers[reg1];
    mem->program_counter++;
}

void shift_left(Memory* mem, uint8_t reg1, uint8_t _) {
    mem->registers[0xF] = (bool)(mem->registers[reg1] & 0x80);
    mem->registers[reg1] *= 2;
    mem->program_counter++;
}

void skip_next_reg_not_equal(Memory* mem, uint8_t reg1, uint8_t reg2) {
    if (mem->registers[reg1] != mem->registers[reg2]) {
        mem->program_counter += 2;
    }
    else {
        mem->program_counter++;
    }
}

void set_I(Memory* mem, uint16_t addr) {
    mem->I_register = addr;
    mem->program_counter++;
}

void jump_PC_addr_V0(Memory* mem, uint16_t addr) {
    mem->program_counter = mem->registers[0] + addr;
}

void random_and_byte(Memory* mem, uint8_t reg, uint8_t byte) {
    mem->registers[reg] = rand_byte() & byte;
    mem->program_counter++;
}

void disp(Memory* mem, uint8_t reg1, uint8_t reg2, uint8_t sz) {
    auto x = mem->registers[reg1];
    auto y = mem->registers[reg2];
    uint8_t n_VF = 0;
    for (int i = 0; i < sz; ++i) {
        uint8_t scr = mem->memory[mem->I_register + i];
        for (int j = 0; j < 8; j++) {
            auto scr_bit = (scr & (1 << (7-j))) >> (7-j);
            auto bit = mem->graphic[x + j][y];
            
            auto res = bit ^ scr_bit;
            n_VF |= res & bit;
            mem->graphic[x+j][y] = res;
        }
    }
    mem->registers[0xF] = n_VF;
    mem->program_counter++;
}

void skip_next_kb_down(Memory* mem, uint8_t reg) {
    if (mem->keyboard[mem->registers[reg]]) {
        mem->program_counter++;
    }
    mem->program_counter++;
}

void skip_next_kb_not_down(Memory* mem, uint8_t reg) {
    if (!mem->keyboard[mem->registers[reg]]) {
            mem->program_counter++;
        }
        mem->program_counter++;
}

void set_reg_delay_timer(Memory* mem, uint8_t reg) {
    mem->registers[reg] = mem->delay_timer;
}

void wait_for_key(Memory* mem, uint8_t reg) {
    mem->registers[reg] = mem->keyboard[mem->registers[reg]];
    if (mem->registers[reg]) {
        mem->program_counter++;
    }
}

void set_delay_timer_reg(Memory* mem, uint8_t reg) {
    mem->delay_timer = mem->registers[reg];
    mem->program_counter++;
}

void set_sound_timer_reg(Memory* mem, uint8_t reg) {
    mem->sound_timer = mem->registers[reg];
    mem->program_counter++;
}

void add_I_reg(Memory* mem, uint8_t reg) {
    mem->I_register += mem->registers[reg];
    mem->program_counter++;
}

void set_I_sprite_loc(Memory* mem, uint8_t reg) {
    mem->I_register = mem->registers[reg] * 5;
    mem->program_counter++;
}

void copy_upto_reg_at_I(Memory* mem, uint8_t reg) {
    for (int i = 0; i <= reg; i++) {
        mem->memory[mem->I_register + i] = mem->registers[i];
    }
    mem->I_register += reg + 1;
    mem->program_counter++;
}

void copy_at_I_into_reg(Memory* mem, uint8_t reg) {
    for (int i = 0; i <= reg; i++) {
        mem->registers[i] = mem->memory[mem->I_register + i];
    }
    mem->I_register += reg + 1;
    mem->program_counter++;
}

void store_bcd(Memory* mem, uint8_t reg) {
    auto v = mem->registers[reg];
    for (int i = 2; i <= 0; i--) {
        mem->memory[mem->I_register + i] = v%10;
        v/=10; 
    }
    mem->program_counter++;
}

void interpret_instruction(uint16_t inst, Memory* mem) {
    if (inst == 0x00E0) { // CLS
        clear_display(mem);
    }
    else if (inst == 0x00EE) { // RET
        return_from_subroutine(mem);
    }
    else if ((inst & 0xF000) == 0x0000) { // SYS addr
        jump_machine_code_routine(mem, inst & 0x0FFF);
    }
    else if ((inst & 0xF000) == 0x1000) { // JP addr
        set_program_counter(mem, inst & 0x0FFF);
    }
    else if ((inst & 0xF000) == 0x2000) { // CALL addr
        call_subroutine(mem, inst & 0x0FFF);
    }
    else if ((inst & 0xF000) == 0x3000) { // SE Vx, byte
        skip_next_equal(mem, (inst & 0x0F00) >> 8, inst & 0x00FF);
    }
    else if ((inst & 0xF000) == 0x4000) {// SNE Vx, byte
        skip_next_not_equal(mem, (inst & 0x0F00) >> 8, inst & 0x00FF);
    }
    else if ((inst & 0xF00F) == 0x5000) { // SE Vx Vy
        skip_next_reg_equal(mem, (inst & 0x0F00) >> 8, (inst & 0x00F0)>>4);
    }
    else if ((inst & 0xF000) == 0x6000) { //LD Vx, byte
        insert_reg_byte(mem, (inst & 0x0F00) >> 8, inst & 0x00FF);
    }
    else if ((inst & 0xF000) == 0x7000) { // ADD Vx, byte
        add_reg_byte(mem, ((inst & 0x0F00) >> 8), inst & 0x00FF);
    }
    else if ((inst & 0xF000) == 0x8000) {
        uint8_t reg1 = (inst & 0x0F00) >> 8;
        uint8_t reg2 = (inst & 0x00F0) >> 4;
        switch (inst & 0x000F) {
            case 0x0: // LD Vx, Vy
                insert_reg_reg(mem, reg1, reg2);
                break;
            case 0x1: // OR Vx, Vy
                or_reg(mem, reg1, reg2);
                break;
            case 0x2: // AND Vx, Vy
                and_reg(mem, reg1, reg2);
                break;
            case 0x3: // XOR Vx, Vy
                xor_reg(mem, reg1, reg2);
                break;
            case 0x4: // ADD Vx, Vy
                add_reg(mem, reg1, reg2);
                break;
            case 0x5: // SUB Vx, Vy
                sub_reg(mem, reg1, reg2);
                break;
            case 0x6: // SHR Vx {, Vy}
                shift_right(mem, reg1, reg2);
                break;
            case 0x7: // SUBN Vx, Vy
                subn_reg(mem, reg1, reg2);
                break;
            case 0xE: // SHL Vx {, Vy}
                shift_left(mem, reg1, reg2);
                break;
        }
    }
    else if ((inst & 0xF00F) == 0x9000) { // SNE Vx, Vy
        skip_next_reg_not_equal(mem, (inst & 0x0F00) >> 8, (inst & 0x00F0) >> 4);
    }
    else if ((inst & 0xF000) == 0xA000) { // LD I, addr
        set_I(mem, inst & 0x0FFF);
    }
    else if ((inst & 0xF000) == 0xB000) { // JP V0, addr
        jump_PC_addr_V0(mem, inst & 0xFFF);
    }
    else if ((inst & 0xF000) == 0xC000) {// RND Vx, byte
        random_and_byte(mem, (inst & 0x0F00) >> 8, inst & 0x00FF);    
    }
    else if ((inst & 0xF000) == 0xD000) {/// DRW Vx, Vy, nibble
        disp(mem, (inst & 0x0F00) >> 8, (inst & 0x00F0) >> 4, inst & 0x000F);    
    }
    else if ((inst & 0xF000) == 0xE000) {
        uint8_t reg = (inst & 0x0F00) >> 8;
        switch (inst & 0x00FF) {
            case 0x9E: // SKP Vx
                skip_next_kb_down(mem, reg);
                break;
            case 0xA1: // SKNP Vx
                skip_next_kb_not_down(mem, reg);
                break;
        }
    }
    else if ((inst & 0xF000) == 0xF000) {
        uint8_t reg = (inst & 0x0F00) >> 8;
        switch (inst & 0x00FF) {
            case 0x07: // LD Vx, DT
                set_reg_delay_timer(mem, reg);
                break;
            case 0x0A: // LD Vx, K
                wait_for_key(mem, reg);
                break;
            case 0x15: // LD DT, Vx
                set_delay_timer_reg(mem, reg);
                break;
            case 0x18: // LD ST, Vx
                set_sound_timer_reg(mem, reg);
                break;
            case 0x1E: // ADD I, Vx
                add_I_reg(mem, reg);
                break;
            case 0x29: //LD F, Vx
                set_I_sprite_loc(mem, reg);
                break;
            case 0x33: // LD B, Vx
                store_bcd(mem, reg);
                break;
            case 0x55: // LD [I], Vx
                copy_upto_reg_at_I(mem, reg);
                break;
            case 0x65: // LD Vx, [I]
                copy_at_I_into_reg(mem, reg);
                break;
        }
    }
}