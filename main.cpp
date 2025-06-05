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
    return 0;
}