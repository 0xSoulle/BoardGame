

#include <iostream>
#include <limits>
#include "game.cpp"


using namespace std;

const string HELP = "-1 - Exit\n0 - Continue Game\n1 - Print Status\n2 - Print Next Player\n3 - Print Board\n4 - Print this Menu\n";

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

    if (*num_players > 6) {
        cout << "Let's keep it small. Number of players set to 6"<< endl;
        num_players = new int(6);
    }

    string player_names[*num_players];
    for (int player = 0; player < *num_players; player++) {
        cout << "Enter name for Player " << player + 1 << ": ";
        cin >> player_names[player];
    }

    Game game = Game(*num_players, player_names);
    
    int command = 0;   
    cout << HELP;
    do {
        cout << "Enter command: ";
        
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
    }
}

void printStatus(Game game) {
    for (int i = 0; i < *num_players; i++) {
        Player player = game.getPlayer(i);
        cout << "Player "<< "#" << i+1 << ", " << player.getName() << ", has " << player.getFreeze() << " fines and is at position " << player.getPosition() << endl;
    }
}

void printNext(Game game) {
    cout << "Next player is player"<< game.getNextPlayer().getName() << endl;
}

void printBoard(Game game) {
    cout << "Board viewing not implemented yet." << endl;
}

void nextPlay(Game game) {
    Player player = game.getNextPlayer();

    game.playRound();
    int dice = game.getDice();
    cout << " Player " << player.getName() << " rolled " << dice << endl;
}