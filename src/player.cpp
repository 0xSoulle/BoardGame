

#include <string>

using namespace std;
class Player {
    private:
        string name;
        int position;
        int freezeTime;

    public:
        // Default constructor for array initialization
        Player() {}

        Player(string name) {
            this->name = name;
            this->position = 1;
            this->freezeTime   = 0;
        }

        string getName() {
            return name;
        }

        void move(int dice) {
            position += dice;
        }

        int getPosition() {
            return position;
        }

        int getFreeze() {
            return freezeTime;
        }

        void freeze() {
            freezeTime = 1;
        }

        void reduceFreeze() {
            if (freezeTime > 0) {
                freezeTime--;
            }
        }
        
        void kill() {
            freezeTime = -1;
        }

};