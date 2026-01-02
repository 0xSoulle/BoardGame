#include "game.h"
#include <cstdlib>
#include <ctime>

void Game::drawBoard() {
    for(int i = 0; i < 90; i += 10) {
        if (i < 50) {
            freeze[i/10] = i + rand()%9 + 1;
            jumps[i/10] = i + rand()%9 + 1;
        }
        else if (i < 60) {
            fall = i + rand()%10;
        }
        else if (i < 70) {
            crab = i + rand()%10;
        }
        else if (i < 80) {
            hell = i + rand()%10;
        }
        else {
            death = i + rand()%10;
        }
    }
}

int Game::rollDice() {
    return rand()%6 + 1;
}

void Game::setNextPlayer() {
    nextPlayer++;
    if (nextPlayer == num_players) {
        nextPlayer = 0;
    }
    
    while(players[nextPlayer].getFreeze() != 0 && players[nextPlayer].isAlive()) {
        players[nextPlayer].reduceFreeze();
        nextPlayer++;
        if (nextPlayer == num_players) {
            nextPlayer = 0;
        }
    }
}

int Game::checkPositionEffects(int player, int position) {
    if (position < 50) {
        for (int i = 0; i < 5; i++) {
            if (position == freeze[i]) {
                freezePlayer(player);
                return 0;
            }
            else if (position == jumps[i]) {
                applyJump(player);
                return 1;
            }
        }
    }
    else if (position == fall) {
        applyFall(player);
        return 2;
    }
    else if (position == crab) {
        applyCrab(player, rolled_dice);
        return 3;
    }
    else if (position == hell) {
        applyHell(player);
        return 4;
    }
    else if (position == death) {
        applyDeath(player);
        return 5;
    }
    return -1;
}

void Game::applyJump(int player) {
    players[player].move(9);
}

void Game::freezePlayer(int player) {
    players[player].freeze();
}

void Game::applyFall(int player) {
    players[player].move(-9);
}

void Game::applyCrab(int player, int dice) {
    players[player].move(-2 * dice);
}

void Game::applyHell(int player) {
    players[player].move(-players[player].getPosition());
}

void Game::applyDeath(int player) {
    players[player].kill();
}

Game::Game(int num_players, std::string player_names[]) {
    this->num_players = num_players;
    players = new Player[num_players];
    for (int player = 0; player < num_players; player++) {
        players[player] = Player(player_names[player]);
    }
    
    srand(time(0));
    drawBoard();
}

Game::~Game() {
    delete[] players;
}

int Game::getDice() {
    return rolled_dice;
}

Player Game::getNextPlayer() {
    return players[nextPlayer];
}

Player Game::getPlayer(int index) {
    return players[index];
}

int Game::getNumPlayers() {
    return num_players;
}

int Game::getNumTiles() {
    return NUM_TILES;
}

int Game::getNextPlayerIndex() {
    return nextPlayer;
}

int Game::getSpecialTile(int type) {
    switch(type) {
        case 0: return freeze[0]; // First freeze tile
        case 1: return jumps[0];  // First jump tile
        case 2: return fall;
        case 3: return crab;
        case 4: return hell;
        case 5: return death;
        default: return -1;
    }
}

int Game::getFreezeTile(int index) {
    if (index >= 0 && index < 5) {
        return freeze[index];
    }
    return -1;
}

int Game::getJumpTile(int index) {
    if (index >= 0 && index < 5) {
        return jumps[index];
    }
    return -1;
}

bool Game::isSpecialTile(int tile) {
    if (tile < 50) {
        for (int i = 0; i < 5; i++) {
            if (tile == freeze[i] || tile == jumps[i]) {
                return true;
            }
        }
    }
    else if (tile == fall || tile == crab || tile == hell || tile == death) {
        return true;
    }
    return false;
}

int Game::playRound() {
    if (!players[nextPlayer].isAlive()) {
        setNextPlayer();
        return -1;
    }
    
    rolled_dice = rollDice();
    players[nextPlayer].move(rolled_dice);
    int current_position = players[nextPlayer].getPosition();
    
    int event = checkPositionEffects(nextPlayer, current_position);
    
    setNextPlayer();
    return event;
}