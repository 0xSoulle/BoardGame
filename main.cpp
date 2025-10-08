#include <iostream>
#include "game.cpp"

using namespace std;

const string HELP = "-1 - Exit\n0 / Enter - Continue Game\n1 - Print Status\n2 - Print Next Player\n3 - Print Board\n4 - Print this Menu\n";

void processCommand(Game game, int command);
void printStatus(Game game);
void printNext(Game game);
void printBoard(Game game);
void nextPlay(Game game);

int main (int argc, char *argv[]) {

    int num_players = -1;
    while ((std::cout << "How many players? ") && !(std::cin >> num_players)) {
        cout << " !!! INVALID INPUT !!!\nPlease Insert Valid Number of Players: ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }   

    Game game = Game(num_players);
    
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
        case 4: cout << HELP;break;
        default: nextPlay(game);break;
    }
}

void printStatus(Game game) {
}
void printNext(Game game) {

}
void printBoard(Game game) {
}
void nextPlay(Game game) {
    int dice = game.getDice();
    int player = game.getNextPlayer();
    cout << "Player "<< player << " rolled " << dice;


}