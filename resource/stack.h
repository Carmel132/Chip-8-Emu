#pragma once
#include <iostream>

template <typename T, int sz>
struct Stack {
    T data[sz] = {};
    // Top idx
    int idx = 0;
    void add(const T& _val) {
        data[idx] = _val;
        idx++;
    }
    T pop() {
        idx--;
        T ret{data[idx]};
        return ret;
    }
    T top() const {
        return data[idx - 1];
    }
};