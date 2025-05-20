#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

std::vector<uint8_t> read_bin(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    std::vector<uint8_t> ret(std::istreambuf_iterator<char>(file), {});
    return ret;
}