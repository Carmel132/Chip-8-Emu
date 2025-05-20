#pragma once
#include <iostream>

template <typename T, int sz>
struct Stack {
    T data[sz];
    // Top idx
    int idx = 0;
    void add(const T& _val) {
        data[idx + 1] = _val;
        idx++;
    }
    T pop() {
        T ret{data[idx]};
        idx--;
        return ret;
    }
    T top() const {
        return data[idx];
    }
};