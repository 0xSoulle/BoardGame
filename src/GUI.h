#ifndef GUI_H
#define GUI_H

#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class GUI {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        TTF_Font* font;
        TTF_Font* smallFont;
        
        const int WINDOW_WIDTH = 1200;
        const int WINDOW_HEIGHT = 800;
        const int BOARD_X = 50;
        const int BOARD_Y = 50;
        const int BOARD_WIDTH = 700;
        const int BOARD_HEIGHT = 700;
        const int TILE_SIZE = 35;
        
        bool running;
        Game* game;
        
        void initSDL();
        void cleanupSDL();
        void renderBoard();
        void renderPlayers();
        void renderUI();
        void renderText(const std::string& text, int x, int y, SDL_Color color, TTF_Font* font);
        SDL_Color getTileColor(int tile);
        SDL_Color getPlayerColor(int playerIndex);
        void drawRect(int x, int y, int w, int h, SDL_Color color);
        void drawFilledRect(int x, int y, int w, int h, SDL_Color color);
        void drawCircle(int x, int y, int radius, SDL_Color color);
        void drawFilledCircle(int x, int y, int radius, SDL_Color color);
        
    public:
        GUI(Game* game);
        ~GUI();
        bool isRunning();
        void handleEvents();
        void render();
        void update();
};

#endif
