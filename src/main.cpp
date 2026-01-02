#include <iostream>
#include <limits>
#include <string>
#include "game.h"
#include "GUI.h"

int main(int argc, char *argv[]) {
    int num_players;
    
    std::cout << "How many players? ";
    while (!(std::cin >> num_players)) {
        std::cout << "!!! INVALID INPUT !!!\nPlease Insert Valid Number of Players: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    if (num_players > 6) {
        std::cout << "Let's keep it small. Number of players set to 6" << std::endl;
        num_players = 6;
    }
    if (num_players < 1) {
        std::cout << "Need at least 1 player. Setting to 1." << std::endl;
        num_players = 1;
    }
    
    std::string* player_names = new std::string[num_players];
    for (int player = 0; player < num_players; player++) {
        std::cout << "Enter name for player " << (player + 1) << ": ";
        std::cin >> player_names[player];
    }
    
    Game game(num_players, player_names);
    delete[] player_names;
    
    GUI gui(&game);
    
    std::cout << "\n=== Board Game Started ===" << std::endl;
    std::cout << "Press SPACE to play a round" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;
    
    while (gui.isRunning()) {
        gui.handleEvents();
        gui.render();
        gui.update();
    }
    
    std::cout << "END OF GAME" << std::endl;
    return 0;
}