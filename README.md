# Board Game GUI

A graphical board game implementation using C++ and SDL2.

## Features

- Visual board representation with 90 tiles
- Color-coded special tiles (Freeze, Jump, Fall, Crab, Hell, Death)
- Real-time player position visualization
- Player status display
- Interactive gameplay with keyboard controls

## Dependencies

- C++17 compiler (g++ or clang++)
- SDL2 library
- SDL2_ttf library (for text rendering)

## Installation

### Arch Linux
```bash
sudo pacman -S sdl2 sdl2_ttf
```

### Ubuntu/Debian
```bash
sudo apt-get install libsdl2-dev libsdl2-ttf-dev
```

### macOS
```bash
brew install sdl2 sdl2_ttf
```

Or use the Makefile target:
```bash
make install-deps
```

## Building

```bash
make
```

This will create an executable named `boardgame`.

## Running

```bash
./boardgame
```

## Controls

- **SPACE**: Play a round (roll dice and move current player)
- **ESC**: Exit the game

## Game Rules

- Players start at position 1
- Players roll a dice (1-6) and move forward
- Special tiles have various effects:
  - **Freeze** (Blue): Player skips next turn
  - **Jump** (Green): Player moves 9 tiles forward
  - **Fall** (Orange): Player moves 9 tiles backward
  - **Crab** (Red): Player moves backward by 2x dice roll
  - **Hell** (Purple): Player returns to start
  - **Death** (Dark Red): Player is eliminated

## Code Structure

- `src/player.h` / `src/player.cpp`: Player class implementation
- `src/game.h` / `src/game.cpp`: Game logic and board management
- `src/GUI.h` / `src/GUI.cpp`: SDL2-based graphical interface
- `src/main.cpp`: Main entry point

## Maintenance

The code is structured with clear separation of concerns:
- Game logic is independent of the GUI
- GUI can be easily modified by changing rendering functions in `GUI.cpp`
- Colors, sizes, and layouts are defined as constants in the GUI class
- Special tile visualization can be customized in `getTileColor()`
