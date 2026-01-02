#ifndef GUI_H
#define GUI_H

#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

enum class GUIState {
    SETUP,
    GAME
};

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
        GUIState state;
        
        // Setup state variables
        int selected_num_players;
        bool dropdown_open;
        std::vector<std::string> player_names;
        int active_text_input; // -1 if none, 0-5 for player index
        std::string input_text;
        
        // Game notification
        std::string notification_message;
        
        void initSDL();
        void cleanupSDL();
        void renderBoard();
        void renderPlayers();
        void renderUI();
        void renderSetup();
        void renderNotification();
        void renderText(const std::string& text, int x, int y, SDL_Color color, TTF_Font* font);
        SDL_Color getTileColor(int tile);
        SDL_Color getPlayerColor(int playerIndex);
        void drawRect(int x, int y, int w, int h, SDL_Color color);
        void drawFilledRect(int x, int y, int w, int h, SDL_Color color);
        void drawCircle(int x, int y, int radius, SDL_Color color);
        void drawFilledCircle(int x, int y, int radius, SDL_Color color);
        bool isPointInRect(int x, int y, int rect_x, int rect_y, int rect_w, int rect_h);
        void handleSetupEvents(SDL_Event& event);
        void handleGameEvents(SDL_Event& event);
        void startGame();
        
    public:
        GUI();
        ~GUI();
        bool isRunning();
        void handleEvents();
        void render();
        void update();
        bool isGameReady();
        Game* getGame();
};

#endif
