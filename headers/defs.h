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

/**
 * struct GameState_s - Stores the current state of the game
 *
 * @window: Pointer to the SDL_Window, which represents the game window
 * @renderer: Pointer to the SDL_Renderer, used for rendering graphics
 * @texture: Pointer to the SDL_Texture, used for applying textures
 * @tiles: 3D array storing textures for each tile of the game world
 * @screenBuffer: 2D array representing the screen pixels for rendering
 * @position: Player's position in the game world (x, y coordinates)
 * @direction: Player's direction vector for movement
 * @viewPlane: Plane perpendicular to the player's direction for field of view
 * @time: Time elapsed in the game (used for calculating frame updates)
 * @maze: Pointer to the maze map data (2D array representing the game level)
 * @textured: Flag to indicate if textures are being used in the game
 * @quit: Flag to indicate if the game loop should exit
 */
typedef struct GameState_s
{
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

void extract_TexturePixels(GameState *state, SDL_Surface *texture, int index);
void extract_Pixels(GameState *state, SDL_Surface *parsedOrigin[]);
uint32_t get_ColorFromPixel(uint8_t *pixel, SDL_PixelFormat *format);
void printGameState(GameState *state);

#endif
