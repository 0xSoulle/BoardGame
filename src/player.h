#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
    private:
        std::string name;
        int position;
        int freezeTime;

    public:
        Player();
        Player(std::string name);
        
        std::string getName();
        void move(int dice);
        int getPosition();
        int getFreeze();
        void freeze();
        void reduceFreeze();
        void kill();
        bool isAlive();
};

#endif
