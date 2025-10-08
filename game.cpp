#include <random>

using namespace std; 

class Game {
    private:
        const int num_tiles = 90;
        int fall, crab, hell, death;
        int fines[5];
        int jumps[5];

        int num_players;
        int nextPlayer;
        int player_info[5][2];
        int rolled_dice;

        int genRandInt(int lower, int higher) {
            random_device rd;
            mt19937 mt(rd());
            uniform_int_distribution<int> randint(lower, higher);
            
            return randint(mt);
        }

        void drawBoard() {
            for(int i = 0; i < 90; i += 10) {
                if (i < 50) {
                    fines[i/10] = i + genRandInt(1, 10);
                    jumps[i/10] = i + genRandInt(1, 10);
                }
                else if (i < 60) {
                    fall = i + genRandInt(1, 10);
                }
                else if (i < 70) {
                    crab = i + genRandInt(1, 10);
                }
                else if (i < 80) {
                    hell = i + genRandInt(1, 10);
                }
                else {
                    death = i + genRandInt(1, 10);
                }
                
            }

        }

        int rollDice() {
            return genRandInt(1, 6);
        }

        void setNextPlayer() {
            int next = -1;
            int index = nextPlayer;
            while (next == -1) {
                // fines > 0 -> reduce fines
                // fines == 0 -> can play
                // fines < 0 -> player is dead
                if (player_info[index][0] > 0) {
                    player_info[index][0] --;
                }
                else if (player_info[index][1] == 0){
                    next = index;
                }
                index ++;
                if (index == num_players) {
                    index = 0;
                }
            }
           
            nextPlayer = next;
        }

        //player can not play for 1 round
        void finePlayer(int player) {
            player_info[player][0] = 1;
        }

        // moves player position 9 tiles ahead
        void applyJump(int player) {
            player_info[player][1] +=9;
        }

        // moves player position 9 tiles behind
        void applyFall(int player) {
            player_info[player][1] -=9;
        }

        // player moves backwards instead the number rolled
        void applyCrab(int player, int dice) {
            player_info[player][1] -= 2 * dice;
        }

        // player returns to the starting tile
        void applyHell(int player) {
            player_info[player][1] = 0;
        }

        // player is removed from the game (infinite fine)
        void applyDeath(int player) {
            player_info[player][0] = -1;
        }

    public:
        Game(int num_players) {
            this->num_players = num_players;
            for (int player = 1; player <= num_players; player ++) {
                player_info[player][0] = 0;
                player_info[player][1] = 1;
            }
            drawBoard();

            nextPlayer = 0;

        }

        int getDice() {
            return rolled_dice;
        }

        int getNextPlayer() {
            return nextPlayer;
        }

        void playRound() {
            rolled_dice = rollDice();

            int player_position = player_info[nextPlayer][1] + rolled_dice;
            
            
            setNextPlayer();
        }
};