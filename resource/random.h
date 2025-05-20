#pragma once
#include <iostream>

uint8_t rand_byte() {
    return std::rand() % 0x100;
}