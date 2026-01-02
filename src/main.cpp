#include <iostream>
#include "GUI.h"

int main(int argc, char *argv[]) {
    GUI gui;
    
    while (gui.isRunning()) {
        gui.handleEvents();
        gui.render();
        gui.update();
    }
    
    std::cout << "END OF GAME" << std::endl;
    return 0;
}