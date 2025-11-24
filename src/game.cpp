

#include "player.cpp"
#include <cstdlib>

using namespace std;

class Game {
    private:
        const int NUM_TILES = 90;
        // action tiles indices
        int fall, crab, hell, death;
        int freeze[5];
        int jumps[5];

        Player *players;
        int num_players;
        int nextPlayer = 0;

        int rolled_dice;
        
        /*
            freeze tiles one every 10 tiles from 0 to 50
            jump tiles one every 10 tiles from 0 to 50
            1 fall tile between 50 and 60
            1 crab tile between 60 and 70
            1 hell tile between 70 and 80
            1 death tile between 80 and 90
        */
        void drawBoard() {
            for(int i = 0; i < 90; i += 10) {
                if (i < 50) {
                    freeze[i/10] = i + rand()%10 + 1;
                    jumps[i/10] = i + rand()%10 + 1;
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

        int rollDice() {
            return rand()%6 + 1;
        }

        void setNextPlayer() {
            nextPlayer ++;
            if (nextPlayer == num_players) {
                nextPlayer = 0;
            }
            
            while(players[nextPlayer].getFreeze() != 0) {
                players[nextPlayer].reduceFreeze();
                nextPlayer ++;
                if (nextPlayer == num_players) {
                    nextPlayer = 0;
                }
            }
        }

        void checkPositionEffects(int player, int position) {
            if (position < 50) {
                for (int i = 0; i < 5; i++) {
                    if (position == freeze[i]) {
                        freezePlayer(player);
                        return;
                    }
                    else if (position == jumps[i]) {
                        applyJump(player);
                        return;
                    }
                }
            }
            else if (position == fall) {
                applyFall(player);
                return;
            }
            else if (position == crab) {
                applyCrab(player, rolled_dice);
                return;
            }
            else if (position == hell) {
                applyHell(player);
                return;
            }
            else if (position == death) {
                applyDeath(player);
                return;
            }
        }

        // moves player position 9 tiles ahead
        void applyJump(int player) {
            players[player].move(9);
        }

        // freezes player for one turn
        void freezePlayer(int player) {
            players[player].freeze();
        }

        // moves player position 9 tiles behind
        void applyFall(int player) {
            players[player].move(-9);
        }

        // player moves backwards instead the number rolled
        void applyCrab(int player, int dice) {
            players[player].move(-2 * dice);
        }

        // player returns to the starting tile
        void applyHell(int player) {
            players[player].move(-players[player].getPosition());
        }

        // player is removed from the game
        void applyDeath(int player) {
            players[player].kill();
        }
    
    public:
        /* 
        * Constructor -> set player status at 0 fines and position 1
        * initialize random seed
        * draw board action tiles
        */
        Game(int num_players, string player_names[]) {
            this->num_players = num_players;
            players = new Player[num_players];
            for (int player = 0; player < num_players; player++) {
                players[player] = Player(player_names[player]);
            }
            
            srand(time(0));

            drawBoard();
        }

        int getDice() {
            return rolled_dice;
        }

        Player getNextPlayer() {
            return players[nextPlayer];
        }

        Player getPlayer(int index) {
            return players[index];
        }
        void playRound() {
            rolled_dice = rollDice();
            int player_position = players[nextPlayer].getPosition() + rolled_dice;
            
            checkPositionEffects(nextPlayer, player_position);

            setNextPlayer();
        }
};