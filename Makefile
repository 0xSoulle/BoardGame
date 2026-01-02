CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lSDL2 -lSDL2_ttf

SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = boardgame

.PHONY: all clean install-deps

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) a.out

install-deps:
	@echo "Installing SDL2 and SDL2_ttf..."
	@if command -v pacman >/dev/null 2>&1; then \
		sudo pacman -S sdl2 sdl2_ttf; \
	elif command -v apt-get >/dev/null 2>&1; then \
		sudo apt-get install libsdl2-dev libsdl2-ttf-dev; \
	elif command -v yum >/dev/null 2>&1; then \
		sudo yum install SDL2-devel SDL2_ttf-devel; \
	elif command -v brew >/dev/null 2>&1; then \
		brew install sdl2 sdl2_ttf; \
	else \
		echo "Please install SDL2 and SDL2_ttf manually for your distribution"; \
	fi
