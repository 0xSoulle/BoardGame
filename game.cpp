#include <random>

class Game {
    private:
        const int num_tiles = 90;
        int fall, crab, hell, death;
        int fines[9];
        int jumps[9];

    public:
        void drawBoard() {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> randint(1, 10);

            for(int i = 0; i < 90; i += 10) {
                int tile = randint(mt);
                
            }
        }
};