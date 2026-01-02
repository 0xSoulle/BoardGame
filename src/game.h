#ifndef GAME_H
#define GAME_H

#include "player.h"
#include <string>

class Game {
    private:
        const int NUM_TILES = 90;
        int fall, crab, hell, death;
        int freeze[5];
        int jumps[5];

        Player *players;
        int num_players;
        int nextPlayer = 0;
        int rolled_dice;
        
        void drawBoard();
        int rollDice();
        void setNextPlayer();
        int checkPositionEffects(int player, int position);
        void applyJump(int player);
        void freezePlayer(int player);
        void applyFall(int player);
        void applyCrab(int player, int dice);
        void applyHell(int player);
        void applyDeath(int player);
    
    public:
        Game(int num_players, std::string player_names[]);
        ~Game();
        
        int getDice();
        Player getNextPlayer();
        Player getPlayer(int index);
        int getNumPlayers();
        int getNumTiles();
        int getNextPlayerIndex();
        int getSpecialTile(int type); // 0=freeze, 1=jump, 2=fall, 3=crab, 4=hell, 5=death
        int getFreezeTile(int index); // index 0-4
        int getJumpTile(int index);   // index 0-4
        bool isSpecialTile(int tile); // check if tile has special effect
        int playRound();
};

#endif
