
#include <iostream>
#include <stdio.h>
#include <limits>
#include <print>

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
        print(" !!! INVALID INPUT !!!\nPlease Insert Valid Number of Players: ");
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }   

    if (*num_players > 6) {
        print("Let's keep it small. Number of players set to 6");
        num_players = new int(6);
    }

    string player_names[*num_players];
    for (int player = 0; player < *num_players; player++) {
        print("Enter name for player {0}: ", player + 1);
        cin >> player_names[player];
    }

    Game game = Game(*num_players, player_names);
    
    int command = 0;   
    print("-1 - Exit\n0 - Continue Game\n1 - Print Status\n2 - Print Next Player\n3 - Print Board\n4 - Print this Menu\n");
    do {
        print("Enter command: ");
        
        cin >> command; 
        processCommand(game, command);
        
    }
    while(command != -1);

    print("END OF GAME\n");
}

void processCommand(Game game, int command) {
    switch(command) {
        case 0: nextPlay(game);break;
        case 1: printStatus(game);break;
        case 2: printNext(game);break;
        case 3: printBoard(game);break;
        case 4: cout << HELP;break;
    }
}

void printStatus(Game game) {
    for (int i = 0; i < *num_players; i++) {
        Player player = game.getPlayer(i);
        print("Player #{0}, {1}, has {2} fines and is at position {3} \n", i+1, player.getName(), player.getFreeze(), player.getPosition());
    }
}

void printNext(Game game) {
    print("Next to player is player {0} \n", game.getNextPlayer().getName());
}

void printBoard(Game game) {
    cout << "Board viewing not implemented yet." << endl;
}

void nextPlay(Game game) {
    Player player = game.getNextPlayer();

    int event = game.playRound();
    int dice = game.getDice();
    switch (event) {
        case 0: print(" XXX Player {0} caught effect FREEZE and will not play next round XXX\n", player.getName());
        case 1: print(" !!! Player {0} caught effect JUMP and was move 9 tiles FORWARDS !!!\n", player.getName());
        case 2: print(" !!! Player {0} caught effect FALL and was moved 9 tiles BACKWARDS XXX\n", player.getName());
        case 3: print(" !!! Player {0} caught effect CRAB and was moved his dice BACKWARDS XXX\n", player.getName());
        case 4: print(" XXX VADE RETRO SATANA Player {0} caught effect HELL and was placed at the start tile !!!\n", player.getName());
        case 5: print(" R.I.P. VADE RETRO SATANA Player {0} caught effect DEATH and is out of the game R.I.P.\n", player.getName());
    }

    cout << " Player " << player.getName() << " rolled " << dice << endl;
}