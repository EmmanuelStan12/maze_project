#include "../headers/defs.h"
#include "../headers/textures.h"

/**
 * printGameState - Prints the current state of the GameState structure.
 *
 * @state: Pointer to the GameState to print
 */
void printGameState(GameState *state)
{
	printf("Current Game State:\n");
	printf("--------------------\n");
	printf("Window Pointer: %p\n", (void *)state->window);
	printf("Renderer Pointer: %p\n", (void *)state->renderer);
	printf("Texture Pointer: %p\n", (void *)state->texture);
	printf("Textured: %d\n", state->textured);
	printf("Quit Flag: %d\n", state->quit);
	printf("Player Position: (%f, %f)\n", state->position.x, state->position.y);
	printf("Direction: (%f, %f)\n", state->direction.x, state->direction.y);
	printf("Player Plane: (%f, %f)\n", state->viewPlane.x, state->viewPlane.y);
	printf("Time: %f\n", state->time);
	printf("Maze Pointer: %p\n", (void *)state->maze);
}

/**
 * get_ColorFromPixel - Responsible for retrieving color
 * value from a pixel
 * @pixel: Represents a pointer to the pixel data
 * @format: Represents a pointer to the SDL_PixelFormat
 * structure
 * Return: 32-bit color value in ARGB8888 format
 */
uint32_t get_ColorFromPixel(uint8_t *pixel, SDL_PixelFormat *format)
{
	uint32_t color = 0;

	switch (format->BytesPerPixel)
	{
	case 1:
		color = *pixel;
		break;
	case 2:
		color = *(uint16_t *)pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			color = pixel[0] << 16 | pixel[1] << 8 | pixel[2];
		else
			color = pixel[0] | pixel[1] << 8 | pixel[2] << 16;
		break;
	case 4:
		color = *(uint32_t *)pixel;
		break;
	}
	return (color);
}

/**
 * extract_Pixels - Responsible for extracting pixel data
 * from loaded textures
 * @state: Represents a pointer to the GameState structure
 * @parsedOrigin: Represnts an array of loaded textures
 * Return: Always void
 */
void extract_Pixels(GameState *state, SDL_Surface * parsedOrigin[])
{
	int i;

	for (i = 0; i < TEXTURE_COUNT; i++)
	{
		extract_TexturePixels(state, parsedOrigin[i], i);
		SDL_FreeSurface(parsedOrigin[i]);
		parsedOrigin[i] = NULL;
	}
}


