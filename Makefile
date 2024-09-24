#Responsible for compiler and flags
CC = gcc
CFLAGS = -Idependencies -Wall -Werror -Wextra -pedantic
LDFLAGS = -lm `sdl2-config --cflags` `sdl2-config --libs` -lSDL2_image

# Represents the  Source and output
SRC = ./src/*.c
OUT = play_game

# Build target
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(OUT)

# Cleaning up
clean:
	rm -f $(OUT)

# Phony targets
.PHONY: all clean
