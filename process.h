#pragma once
#include "memory.h"

void clear_display(Memory* mem) {

}

void return_from_subroutine(Memory* mem) {
    
}

void jump_machine_code_routine(Memory* mem, uint16_t addr) {

}

void set_program_counter(Memory* mem, uint16_t addr) {

}

void call_subroutine(Memory* mem, uint16_t addr) {

}

void skip_next_equal(Memory* mem, uint8_t reg, uint8_t byte) {

}

void skip_next_not_equal(Memory* mem, uint8_t reg, uint8_t byte) {

}

void skip_next_reg_equal(Memory* mem, uint8_t reg1, uint8_t reg2) {

}

void insert_reg_byte(Memory* mem, uint8_t reg, uint8_t byte) {

}

void add_reg_byte(Memory* mem, uint8_t reg, uint8_t byte) {

}

void insert_reg_reg(Memory* mem, uint8_t reg1, uint8_t reg2) {

}

void or_reg(Memory* mem, uint8_t reg1, uint8_t reg2) {

}


void interpret_instruction(uint16_t inst, Memory* mem) {
    if (inst == 0x00E0) { // CLS
        clear_display(mem);
    }
    else if (inst == 0x00EE) { // RET
        return_from_subroutine(mem);
    }
    else if (inst & 0xF000 == 0x0000) { // SYS addr
        jump_machine_code_routine(mem, inst & 0x0FFF);
    }
    else if (inst & 0xF000 == 0x1000) { // JP addr
        set_program_counter(mem, inst & 0x0FFF);
    }
    else if (inst & 0xF000 == 0x2000) { // CALL addr
        call_subroutine(mem, inst & 0x0FFF);
    }
    else if (inst & 0xF000 == 0x3000) { // SE Vx, byte
        skip_next_equal(mem, (inst & 0x0F00) >> 8, inst & 0x00FF);
    }
    else if (inst & 0xF000 == 0x4000) {// SNE Vx, byte
        skip_next_not_equal(mem, (inst & 0x0F00) >> 8, inst & 0x00FF);
    }
    else if (inst & 0xF00F == 0x5000) { // SE Vx Vy
        skip_next_reg_equal(mem, (inst & 0x0F00) >> 8, (inst & 0x00F0)>>4);
    }
    else if (inst & 0xF000 == 0x6000) { //LD Vx, byte
        insert_reg_byte(mem, (inst & 0x0F00) >> 8, inst & 0x00FF);
    }
    else if (inst & 0xF000 == 0x7000) { // ADD Vx, byte
        add_reg_byte(mem, ((inst & 0x0F00) >> 8), inst & 0x00FF);
    }
    else if (inst & 0xF000 == 0x8000) {
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
    else if (inst & 0xF00F == 0x9000) { // SNE Vx, Vy
        skip_next_reg_not_equal(mem, (inst & 0x0F00) >> 8, (inst & 0x00F0) >> 4);
    }
    else if (inst & 0xF000 == 0xA000) { // LD I, addr
        set_I(mem, inst & 0x0FFF);
    }
    else if (inst & 0xF000 == 0xB000) { // JP V0, addr
        jump_PC_addr_V0(mem, inst & 0xFFF);
    }
    else if (inst & 0xF000 == 0xC000) {// RND Vx, byte
        random_and_byte(mem, (inst & 0x0F00) >> 8, inst & 0x00FF);    
    }
    else if (inst & 0xF000 == 0xD000) {/// DRW Vx, Vy, nibble
        disp(mem, (inst & 0x0F00) >> 8, (inst & 0x00F0) >> 4, inst & 0x000F);    
    }
    else if (inst & 0xF000 == 0xE000) {
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
    else if (inst & 0xF000 == 0xF000) {
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