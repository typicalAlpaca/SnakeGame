# all:
# 	g++ -I src/include -L src/lib -o bin/Snake Snake.cpp -lmingw32 -lSDL2main -lSDL2


# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -I src/include -Wall

# Linker flags
LDFLAGS = -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

# Directories
SRC_DIR = src

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Target executable
TARGET = SnakeGame

# Default rule
all: $(TARGET)

# Linking rule
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
