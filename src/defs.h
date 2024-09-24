#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>
#include <SDL2/SDL.h>
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64
#define TEXTURE_COUNT 6
#define MAP_WIDTH 25
#define MAP_HEIGHT 24
#define MAP_CHAR_COUNT 1275

#include <stdbool.h>

/**
 * struct point_s - data structure for XY point coordinates of type double
 * @x: x coordinate
 * @y: y coordinate
 */
typedef struct point_s
{
    double x;
    double y;
} point_t;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint32_t tiles[TEXTURE_COUNT][TEXTURE_HEIGHT][TEXTURE_HEIGHT];
    uint32_t screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
    point_t position;
    point_t direction;
    point_t viewPlane;
    double time;
    int *maze;
    int textured;
    int quit;
} GameState;

void printGameState(GameState *state);

#endif // !DEBUG
