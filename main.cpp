#include <iostream>
#include <ctime>
#include "resource/file.h"
#include "process.h"
#include "window.h"

int main() {
    std::srand(std::time(0));
    

    Window w{};
    w.init();
    w.start();
    w.run();
    /*print_screen(&mem);
    std::cout << mem.registers[0];
    std::cout << instr.size();*/
    return 0;
}