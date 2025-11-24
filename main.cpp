

#include <iostream>
#include "game.cpp"
#include <limits>

using namespace std;

const string HELP = "-1 - Exit\n0 / Enter - Continue Game\n1 - Print Status\n2 - Print Next Player\n3 - Print Board\n4 - Print this Menu\n";

void processCommand(Game game, int command);
void printStatus(Game game);
void printNext(Game game);
void printBoard(Game game);
void nextPlay(Game game);

int* num_players;

int main (int argc, char *argv[]) {

    num_players = new int;
    while ((std::cout << "How many players? ") && !(std::cin >> *num_players)) {
        cout << " !!! INVALID INPUT !!!\nPlease Insert Valid Number of Players: ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }   

    Game game = Game(*num_players);
    
    int command = 0;   
    cout << HELP;
    do {
        cout << "Enter command: ";
        if (cin.get() == '\n') {
            nextPlay(game);
        }
        cin >> command; 
        processCommand(game, command);
        
    }
    while(command != -1);

    cout << "END OF GAME\n";
}

void processCommand(Game game, int command) {
    switch(command) {
        case 0: nextPlay(game);break;
        case 1: printStatus(game);break;
        case 2: printNext(game);break;
        case 3: printBoard(game);break;
        case 4: cout << HELP << endl;break;
        default: nextPlay(game);break;
    }
}

void printStatus(Game game) {
    for (int i = 0; i < *num_players; i++) {
        Player player = game.getPlayer(i);
        cout << "Player " << i << " has " << player.getFreeze() << " fines and is at position " << player.getPosition() << endl;
    }
}

void printNext(Game game) {
    cout << "Next Player is at position " << game.getNextPlayer().getPosition() << endl;
}

void printBoard(Game game) {
    cout << "Board Status not implemented yet." << endl;
}

void nextPlay(Game game) {
    int dice = game.getDice();
    Player player = game.getNextPlayer();
    cout << " Next Player Rolled " << dice << endl;

}