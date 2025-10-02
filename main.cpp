#include <iostream>
#include <string.h>
#include "game.cpp"

using namespace std;
int num_players;

void runGame();

int main (int argc, char *argv[]) {
    cout << "How many players: ";
    cin >> num_players;

    Game game = Game(num_players);
    game.drawBoard();
    
    string command;
    cin >> command; 
    do {
        runGame();
    }
    while(command.compare("QUIT") == 0);
}

void runGame() {
    
}