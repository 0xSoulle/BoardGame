class Player {
    private:
        int position;
        int fines;

    public:
        void move(int dice) {
            position += dice;
        }

        int getPosition() {
            return position;
        }
};