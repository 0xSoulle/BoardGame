#include "GUI.h"
#include <iostream>
#include <algorithm>

GUI::GUI() : game(nullptr), running(true), state(GUIState::SETUP), 
             selected_num_players(2), dropdown_open(false), active_text_input(-1), notification_message("") {
    player_names.resize(6, "");
    initSDL();
}

GUI::~GUI() {
    if (game) {
        delete game;
    }
    cleanupSDL();
}

void GUI::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }
    
    if (TTF_Init() < 0) {
        std::cerr << "TTF initialization failed: " << TTF_GetError() << std::endl;
        running = false;
        return;
    }
    
    window = SDL_CreateWindow("Board Game",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
    
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }
    
    font = TTF_OpenFont("/usr/share/fonts/Adwaita/AdwaitaMono-Bold.ttf", 24);
    if (!font) {
        font = TTF_OpenFont("/usr/share/fonts/truetype/Adwaita/AdwaitaMono-Bold.ttf", 24);
    }
    if (!font) {
        std::cerr << "Warning: Could not load font. Text rendering may not work." << std::endl;
    }
    
    smallFont = TTF_OpenFont("/usr/share/fonts/Adwaita/AdwaitaMono-Regular.ttf", 14);
    if (!smallFont) {
        smallFont = TTF_OpenFont("/usr/share/fonts/truetype/Adwaita/AdwaitaMono-Regular.ttf", 14);
    }
    if (!smallFont) {
        smallFont = font;
    }
}

void GUI::cleanupSDL() {
    if (font) TTF_CloseFont(font);
    if (smallFont && smallFont != font) TTF_CloseFont(smallFont);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void GUI::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        else if (state == GUIState::SETUP) {
            handleSetupEvents(event);
        }
        else if (state == GUIState::GAME) {
            handleGameEvents(event);
        }
    }
}

void GUI::handleSetupEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouse_x = event.button.x;
        int mouse_y = event.button.y;
        
        // Dropdown button (Number of Players)
        int dropdown_x = WINDOW_WIDTH / 2 - 100;
        int dropdown_y = 150;
        int dropdown_w = 200;
        int dropdown_h = 40;
        
        if (isPointInRect(mouse_x, mouse_y, dropdown_x, dropdown_y, dropdown_w, dropdown_h)) {
            dropdown_open = !dropdown_open;
        }
        else if (dropdown_open) {
            // Check dropdown options
            for (int i = 1; i <= 6; i++) {
                int option_y = dropdown_y + dropdown_h + (i - 1) * 35;
                if (isPointInRect(mouse_x, mouse_y, dropdown_x, option_y, dropdown_w, 35)) {
                    // Save current input if active
                    if (active_text_input >= 0 && active_text_input < player_names.size()) {
                        player_names[active_text_input] = input_text;
                    }
                    selected_num_players = i;
                    dropdown_open = false;
                    // Resize player_names if needed
                    if (player_names.size() < selected_num_players) {
                        player_names.resize(selected_num_players, "");
                    }
                    // Clear active input if it's now out of range
                    if (active_text_input >= selected_num_players) {
                        active_text_input = -1;
                        input_text = "";
                    }
                    break;
                }
            }
        }
        
        // Text input fields for player names - match renderSetup layout
        int input_start_y = dropdown_y + dropdown_h + (dropdown_open ? 6 * 35 + 20 : 20);
        int input_w = 300;
        int input_h = 35;
        for (int i = 0; i < selected_num_players; i++) {
            int input_x = WINDOW_WIDTH / 2 - input_w / 2;
            int input_y = input_start_y + i * 50;
            if (isPointInRect(mouse_x, mouse_y, input_x, input_y, input_w, input_h)) {
                // Save current input if switching
                if (active_text_input >= 0 && active_text_input < player_names.size()) {
                    player_names[active_text_input] = input_text;
                }
                active_text_input = i;
                input_text = player_names[i];
                break;
            }
        }
        
        // Start button - match renderSetup layout
        int start_x = WINDOW_WIDTH / 2 - 100;
        int start_y = input_start_y + selected_num_players * 50 + 30;
        int start_w = 200;
        int start_h = 60;
        if (isPointInRect(mouse_x, mouse_y, start_x, start_y, start_w, start_h)) {
            // Save current input if active
            if (active_text_input >= 0 && active_text_input < player_names.size()) {
                player_names[active_text_input] = input_text;
                active_text_input = -1;
                input_text = "";
            }
            
            // Validate all names are filled
            bool all_filled = true;
            for (int i = 0; i < selected_num_players; i++) {
                if (player_names[i].empty()) {
                    all_filled = false;
                    break;
                }
            }
            if (all_filled) {
                startGame();
            }
        }
        
        // Close dropdown if clicking outside (account for open dropdown height)
        int dropdown_total_height = dropdown_h + (dropdown_open ? 6 * 35 : 0);
        if (dropdown_open && !isPointInRect(mouse_x, mouse_y, dropdown_x, dropdown_y, dropdown_w, dropdown_total_height)) {
            dropdown_open = false;
        }
        
        // Deselect text input if clicking outside
        if (active_text_input >= 0) {
            int input_x = WINDOW_WIDTH / 2 - input_w / 2;
            int input_y = input_start_y + active_text_input * 50;
            bool clicked_on_input = false;
            for (int i = 0; i < selected_num_players; i++) {
                int check_x = WINDOW_WIDTH / 2 - input_w / 2;
                int check_y = input_start_y + i * 50;
                if (isPointInRect(mouse_x, mouse_y, check_x, check_y, input_w, input_h)) {
                    clicked_on_input = true;
                    break;
                }
            }
            if (!clicked_on_input && !isPointInRect(mouse_x, mouse_y, start_x, start_y, start_w, start_h)) {
                if (active_text_input < player_names.size()) {
                    player_names[active_text_input] = input_text;
                }
                active_text_input = -1;
                input_text = "";
            }
        }
    }
    else if (event.type == SDL_KEYDOWN && active_text_input >= 0) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && !input_text.empty()) {
            input_text.pop_back();
        }
        else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
            if (active_text_input < player_names.size()) {
                player_names[active_text_input] = input_text;
            }
            active_text_input = -1;
            input_text = "";
        }
        else if (event.key.keysym.sym == SDLK_ESCAPE) {
            active_text_input = -1;
            input_text = "";
        }
    }
    else if (event.type == SDL_TEXTINPUT && active_text_input >= 0) {
        if (input_text.length() < 20) { // Limit name length
            input_text += event.text.text;
        }
    }
}

void GUI::handleGameEvents(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
        }
        else if (event.key.keysym.sym == SDLK_SPACE && game) {
            Player currentPlayer = game->getNextPlayer();
            int event_type = game->playRound();
            int dice = game->getDice();
            
            notification_message = "Player " + currentPlayer.getName() + " rolled " + std::to_string(dice);
            switch (event_type) {
                case 0: notification_message += " - FREEZE!"; break;
                case 1: notification_message += " - JUMP +9!"; break;
                case 2: notification_message += " - FALL -9!"; break;
                case 3: notification_message += " - CRAB!"; break;
                case 4: notification_message += " - HELL!"; break;
                case 5: notification_message += " - DEATH!"; break;
                default: break;
            }
        }
    }
}

void GUI::render() {
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderClear(renderer);
    
    if (state == GUIState::SETUP) {
        renderSetup();
    }
    else if (state == GUIState::GAME && game) {
        renderBoard();
        renderPlayers();
        renderUI();
        renderNotification();
    }
    
    SDL_RenderPresent(renderer);
}

void GUI::renderSetup() {
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Color bgColor = {255, 255, 255, 255};
    SDL_Color borderColor = {100, 100, 100, 255};
    SDL_Color highlightColor = {200, 200, 255, 255};
    SDL_Color buttonColor = {100, 150, 255, 255};
    SDL_Color buttonHoverColor = {150, 200, 255, 255};
    
    // Title
    renderText("Board Game Setup", WINDOW_WIDTH / 2 - 150, 50, textColor, font);
    
    // Number of Players label
    renderText("Number of Players:", WINDOW_WIDTH / 2 - 150, 120, textColor, font);
    
    // Dropdown button
    int dropdown_x = WINDOW_WIDTH / 2 - 100;
    int dropdown_y = 150;
    int dropdown_w = 200;
    int dropdown_h = 40;
    
    SDL_Color ddColor = dropdown_open ? highlightColor : bgColor;
    drawFilledRect(dropdown_x, dropdown_y, dropdown_w, dropdown_h, ddColor);
    drawRect(dropdown_x, dropdown_y, dropdown_w, dropdown_h, borderColor);
    
    std::string dropdown_text = std::to_string(selected_num_players) + " player" + (selected_num_players > 1 ? "s" : "");
    renderText(dropdown_text, dropdown_x + 10, dropdown_y + 10, textColor, smallFont);
    
    // Dropdown arrow
    renderText(dropdown_open ? "^" : "v", dropdown_x + dropdown_w - 25, dropdown_y + 10, textColor, smallFont);
    
    // Dropdown options
    if (dropdown_open) {
        for (int i = 1; i <= 6; i++) {
            int option_y = dropdown_y + dropdown_h + (i - 1) * 35;
            SDL_Color optionColor = (i == selected_num_players) ? highlightColor : bgColor;
            drawFilledRect(dropdown_x, option_y, dropdown_w, 35, optionColor);
            drawRect(dropdown_x, option_y, dropdown_w, 35, borderColor);
            std::string option_text = std::to_string(i) + " player" + (i > 1 ? "s" : "");
            renderText(option_text, dropdown_x + 10, option_y + 8, textColor, smallFont);
        }
    }
    
    // Player name inputs - moved down to avoid dropdown overlap
    int input_start_y = dropdown_y + dropdown_h + (dropdown_open ? 6 * 35 + 20 : 20);
    int input_w = 300;
    int input_h = 35;
    
    for (int i = 0; i < selected_num_players; i++) {
        int input_x = WINDOW_WIDTH / 2 - input_w / 2;
        int input_y = input_start_y + i * 50;
        
        // Label
        std::string label = "Player " + std::to_string(i + 1) + ":";
        renderText(label, input_x - 120, input_y + 8, textColor, smallFont);
        
        // Input box
        SDL_Color inputBgColor = (active_text_input == i) ? highlightColor : bgColor;
        drawFilledRect(input_x, input_y, input_w, input_h, inputBgColor);
        drawRect(input_x, input_y, input_w, input_h, borderColor);
        
        // Text
        std::string display_text = (active_text_input == i) ? input_text : player_names[i];
        if (display_text.empty() && active_text_input != i) {
            display_text = "Enter name...";
            SDL_Color placeholderColor = {150, 150, 150, 255};
            renderText(display_text, input_x + 5, input_y + 8, placeholderColor, smallFont);
        }
        else {
            renderText(display_text, input_x + 5, input_y + 8, textColor, smallFont);
        }
        
        // Cursor for active input
        if (active_text_input == i) {
            int cursor_x = input_x + 5;
            if (!input_text.empty() && font) {
                SDL_Surface* temp_surface = TTF_RenderText_Solid(smallFont, input_text.c_str(), textColor);
                if (temp_surface) {
                    cursor_x += temp_surface->w;
                    SDL_FreeSurface(temp_surface);
                }
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawLine(renderer, cursor_x, input_y + 5, cursor_x, input_y + input_h - 5);
        }
    }
    
    // Start button - made bigger
    int start_x = WINDOW_WIDTH / 2 - 100;
    int start_y = input_start_y + selected_num_players * 50 + 30;
    int start_w = 200;
    int start_h = 60;
    
    // Check if all names are filled
    bool all_filled = true;
    for (int i = 0; i < selected_num_players; i++) {
        if (player_names[i].empty() && active_text_input != i) {
            all_filled = false;
            break;
        }
        if (active_text_input == i && input_text.empty()) {
            all_filled = false;
            break;
        }
    }
    
    SDL_Color btnColor;
    SDL_Color btnTextColor;
    if (all_filled) {
        btnColor = buttonColor;
        btnTextColor = textColor;
    } else {
        btnColor = {200, 200, 200, 255};
        btnTextColor = {150, 150, 150, 255};
    }
    drawFilledRect(start_x, start_y, start_w, start_h, btnColor);
    drawRect(start_x, start_y, start_w, start_h, borderColor);
    
    renderText("Start Game", start_x + 50, start_y + 18, btnTextColor, font);
}

void GUI::renderBoard() {
    const int tiles_per_row = 10;
    const int num_rows = 9;
    
    for (int tile = 1; tile <= 90; tile++) {
        int row = (tile - 1) / tiles_per_row;
        int col = (tile - 1) % tiles_per_row;
        
        int x = BOARD_X + col * TILE_SIZE;
        int y = BOARD_Y + row * TILE_SIZE;
        
        SDL_Color tileColor = getTileColor(tile);
        drawFilledRect(x, y, TILE_SIZE - 2, TILE_SIZE - 2, tileColor);
        
        SDL_Color borderColor = {100, 100, 100, 255};
        drawRect(x, y, TILE_SIZE - 2, TILE_SIZE - 2, borderColor);
        
        if (tile % 10 == 0 || tile == 1) {
            SDL_Color textColor = {0, 0, 0, 255};
            renderText(std::to_string(tile), x + 5, y + 5, textColor, smallFont);
        }
    }
}

SDL_Color GUI::getTileColor(int tile) {
    if (!game) return {255, 255, 255, 255};
    
    SDL_Color normal = {255, 255, 255, 255};
    SDL_Color freezeColor = {100, 150, 255, 255};
    SDL_Color jumpColor = {100, 255, 100, 255};
    SDL_Color fallColor = {255, 200, 100, 255};
    SDL_Color crabColor = {255, 150, 150, 255};
    SDL_Color hellColor = {200, 100, 200, 255};
    SDL_Color deathColor = {150, 0, 0, 255};
    
    if (tile < 50) {
        for (int i = 0; i < 5; i++) {
            if (tile == game->getFreezeTile(i)) return freezeColor;
            if (tile == game->getJumpTile(i)) return jumpColor;
        }
    }
    else if (tile >= 50 && tile < 60) {
        if (tile == game->getSpecialTile(2)) return fallColor;
    }
    else if (tile >= 60 && tile < 70) {
        if (tile == game->getSpecialTile(3)) return crabColor;
    }
    else if (tile >= 70 && tile < 80) {
        if (tile == game->getSpecialTile(4)) return hellColor;
    }
    else if (tile >= 80 && tile < 90) {
        if (tile == game->getSpecialTile(5)) return deathColor;
    }
    
    return normal;
}

SDL_Color GUI::getPlayerColor(int playerIndex) {
    SDL_Color colors[] = {
        {255, 0, 0, 255},     // Red
        {0, 0, 255, 255},     // Blue
        {0, 255, 0, 255},     // Green
        {255, 255, 0, 255},   // Yellow
        {255, 0, 255, 255},   // Magenta
        {0, 255, 255, 255}    // Cyan
    };
    return colors[playerIndex % 6];
}

void GUI::renderPlayers() {
    if (!game) return;
    for (int i = 0; i < game->getNumPlayers(); i++) {
        Player player = game->getPlayer(i);
        if (!player.isAlive()) continue;
        
        int position = player.getPosition();
        if (position < 1 || position > 90) continue;
        
        int row = (position - 1) / 10;
        int col = (position - 1) % 10;
        
        int x = BOARD_X + col * TILE_SIZE + TILE_SIZE / 2;
        int y = BOARD_Y + row * TILE_SIZE + TILE_SIZE / 2;
        
        SDL_Color playerColor = getPlayerColor(i);
        
        int radius = 8;
        int offset_x = (i % 3) * 6 - 6;
        int offset_y = (i / 3) * 6 - 6;
        
        drawFilledCircle(x + offset_x, y + offset_y, radius, playerColor);
        
        SDL_Color borderColor = {0, 0, 0, 255};
        drawCircle(x + offset_x, y + offset_y, radius, borderColor);
    }
}

void GUI::renderUI() {
    if (!game) return;
    int ui_x = BOARD_X + BOARD_WIDTH + 20;
    int ui_y = BOARD_Y;
    
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Color highlightColor = {255, 100, 100, 255};
    
    renderText("BOARD GAME", ui_x, ui_y, textColor, font);
    ui_y += 40;
    
    renderText("Controls:", ui_x, ui_y, textColor, font);
    ui_y += 30;
    renderText("SPACE - Play Round", ui_x, ui_y, textColor, smallFont);
    ui_y += 20;
    renderText("ESC - Exit", ui_x, ui_y, textColor, smallFont);
    ui_y += 40;
    
    Player nextPlayer = game->getNextPlayer();
    renderText("Next Player:", ui_x, ui_y, textColor, font);
    ui_y += 30;
    renderText(nextPlayer.getName(), ui_x, ui_y, highlightColor, font);
    ui_y += 40;
    
    renderText("Player Status:", ui_x, ui_y, textColor, font);
    ui_y += 30;
    
    for (int i = 0; i < game->getNumPlayers(); i++) {
        Player player = game->getPlayer(i);
        SDL_Color playerColor = getPlayerColor(i);
        
        std::string status = player.getName() + ":";
        renderText(status, ui_x, ui_y, playerColor, smallFont);
        
        std::string info = "  Pos: " + std::to_string(player.getPosition());
        if (player.getFreeze() > 0) {
            info += " (Frozen: " + std::to_string(player.getFreeze()) + ")";
        }
        if (!player.isAlive()) {
            info += " (DEAD)";
        }
        renderText(info, ui_x + 100, ui_y, textColor, smallFont);
        ui_y += 25;
    }
    
    ui_y += 20;
    renderText("Special Tiles:", ui_x, ui_y, textColor, font);
    ui_y += 25;
    
    SDL_Color freezeColor = {100, 150, 255, 255};
    SDL_Color jumpColor = {100, 255, 100, 255};
    SDL_Color fallColor = {255, 200, 100, 255};
    SDL_Color crabColor = {255, 150, 150, 255};
    SDL_Color hellColor = {200, 100, 200, 255};
    SDL_Color deathColor = {150, 0, 0, 255};
    
    renderText("Freeze (Blue)", ui_x, ui_y, freezeColor, smallFont);
    ui_y += 20;
    renderText("Jump (Green)", ui_x, ui_y, jumpColor, smallFont);
    ui_y += 20;
    renderText("Fall (Orange)", ui_x, ui_y, fallColor, smallFont);
    ui_y += 20;
    renderText("Crab (Red)", ui_x, ui_y, crabColor, smallFont);
    ui_y += 20;
    renderText("Hell (Purple)", ui_x, ui_y, hellColor, smallFont);
    ui_y += 20;
    renderText("Death (Dark Red)", ui_x, ui_y, deathColor, smallFont);
}

void GUI::renderText(const std::string& text, int x, int y, SDL_Color color, TTF_Font* font) {
    if (!font) return;
    
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    
    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void GUI::drawRect(int x, int y, int w, int h, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderDrawRect(renderer, &rect);
}

void GUI::drawFilledRect(int x, int y, int w, int h, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

void GUI::drawCircle(int x, int y, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius) && (dx*dx + dy*dy) >= ((radius-1) * (radius-1))) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void GUI::drawFilledCircle(int x, int y, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void GUI::update() {
    SDL_Delay(16);
}

bool GUI::isRunning() {
    return running;
}

bool GUI::isPointInRect(int x, int y, int rect_x, int rect_y, int rect_w, int rect_h) {
    return (x >= rect_x && x <= rect_x + rect_w && y >= rect_y && y <= rect_y + rect_h);
}

void GUI::startGame() {
    std::string* names_array = new std::string[selected_num_players];
    for (int i = 0; i < selected_num_players; i++) {
        names_array[i] = player_names[i];
    }
    
    game = new Game(selected_num_players, names_array);
    delete[] names_array;
    
    state = GUIState::GAME;
    std::cout << "\n=== Board Game Started ===" << std::endl;
    std::cout << "Press SPACE to play a round" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;
}

bool GUI::isGameReady() {
    return state == GUIState::GAME && game != nullptr;
}

Game* GUI::getGame() {
    return game;
}

void GUI::renderNotification() {
    if (notification_message.empty()) return;
    
    int notification_y = BOARD_Y + BOARD_HEIGHT + 20;
    int notification_x = BOARD_X;
    
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Color bgColor = {255, 255, 200, 255};
    
    // Draw background for notification
    int text_width = 0;
    if (font) {
        SDL_Surface* temp_surface = TTF_RenderText_Solid(font, notification_message.c_str(), textColor);
        if (temp_surface) {
            text_width = temp_surface->w;
            SDL_FreeSurface(temp_surface);
        }
    }
    
    // Ensure minimum width and add padding
    int box_width = (text_width > 0) ? std::max(text_width + 40, 400) : 400;
    int box_height = 40;
    
    SDL_Color borderColor = {150, 150, 150, 255};
    drawFilledRect(notification_x, notification_y, box_width, box_height, bgColor);
    drawRect(notification_x, notification_y, box_width, box_height, borderColor);
    
    renderText(notification_message, notification_x + 20, notification_y + 8, textColor, font);
}
