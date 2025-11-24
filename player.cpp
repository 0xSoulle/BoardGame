class Player {
    private:
        int position;
        int freezeTime;

    public:
        Player() {
            position = 0;
            freezeTime   = 0;
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