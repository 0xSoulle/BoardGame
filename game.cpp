#include <random>

using namespace std; 

class Game {
    private:
        const int num_tiles = 90;
        int fall, crab, hell, death;
        int fines[5];
        int jumps[5];

        int player_fines[5] = {};
        int num_players;
        int nextPlayer;

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

        void finePlayer(int player) {
            player_fines[player] = 1;
        }

        void applyJumo(int player) {

        }

        void applyFall(int player) {

        }

        void applyCrab(int player, int dice) {

        }

        void applyHell(int player) {

        }

        void applyDeath(int player) {
            player_fines[player] = -1;
        }

    public:
        Game(int num_players) {
            this->num_players = num_players;
            drawBoard();

            nextPlayer = 0;

        }

        int rollDice() {
            return genRandInt(1, 6);
        }

        int getNextPlayer() {
            int next = -1;
            int index = nextPlayer;
            while (next == -1) {
                // fines > 0 -> reduce fines
                // fines == 0 -> can play
                // fines < 0 -> player is dead
                if (player_fines[index] > 0) {
                    player_fines[index] --;
                }
                else if (player_fines[index] == 0){
                    next = index;
                }
                index ++;
                if (index == num_players) {
                    index = 0;
                }
            }
           
            return next;
        }

};