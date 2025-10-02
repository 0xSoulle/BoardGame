#include <random>

using namespace std; 

class Game {
    private:
        const int num_tiles = 90;
        int num_players;
        int fall, crab, hell, death;
        int fines[9];
        int jumps[9];

    public:
        Game(int num_players) {
            this->num_players = num_players;
        }

        void drawBoard() {
            random_device rd;
            mt19937 mt(rd());
            uniform_int_distribution<int> randint(1, 10);

            for(int i = 0; i < 90; i += 10) {
                if (i < 50) {
                    f
                }
                int tile = randint(mt);
                
            }
        }
};