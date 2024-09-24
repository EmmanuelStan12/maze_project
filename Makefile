CC = gcc
CFLAGS = -Idependencies -Wall -Werror -Wextra -pedantic
LDFLAGS = -lm `sdl2-config --cflags` `sdl2-config --libs` -lSDL2_image

SRC = ./src/*.c
OUT = mazzerre

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(OUT)

clean:
	rm -f $(OUT)

.PHONY: all clean
