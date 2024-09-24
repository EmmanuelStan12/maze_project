#include "../headers/defs.h"
#include <SDL2/SDL_image.h>

/**
 * extract_TexturePixels - Responsible for extracting pixels
 * from a single texture
 * @state: Represents a pointer to the Game structure
 * @texture: Pointer to the SDL_Surface representing the texture
 * @index: Index of the texture in the tiles array
 * Return: Always void
 */
void extract_TexturePixels(GameState *state, SDL_Surface *texture, int index)
{
	int j, k, flipped_j;
	uint8_t *pixel;
	uint32_t color;
	Uint8 r, g, b;

	SDL_LockSurface(texture);

	for (j = 0; j < TEXTURE_HEIGHT; j++)
	{
		for (k = 0; k < TEXTURE_WIDTH; k++)
		{
			flipped_j = TEXTURE_HEIGHT - 1 - j;

			pixel = (uint8_t *)texture->pixels +
					 flipped_j * texture->pitch +
					 k * texture->format->BytesPerPixel;

			color = get_ColorFromPixel(pixel, texture->format);

			SDL_GetRGB(color, texture->format, &r, &g, &b);

			state->tiles[index][j][k] = (r << 16) | (g << 8) | b;
		}
	}

	SDL_UnlockSurface(texture);
}

/**
 * loadTextures - loads textures into SDL surfaces
 * @parsedOrigin: array to store loaded textures
 * Return: true if all textures were loaded successfully, false otherwise
 */
int loadTextures(SDL_Surface *parsedOrigin[])
{
	int i, n;
	const char *textureFiles[TEXTURE_COUNT] = {
		"assets/textures/brick_1.jpg",
		"assets/textures/brick_2.jpg",
		"assets/textures/brick_3.jpg",
		"assets/textures/brick_4.jpg",
		"assets/textures/floor.jpg",
		"assets/textures/ceiling.jpg"
	};

	for (n = 0; n < TEXTURE_COUNT; n++)
	{
		parsedOrigin[n] = IMG_Load(textureFiles[n]);
		if (parsedOrigin[n] == NULL)
		{
			fprintf(stderr, "Failed to load texture %d: %s\n", n, IMG_GetError());

			for (i = 0; i < n; i++)
			{
				SDL_FreeSurface(parsedOrigin[i]);
				parsedOrigin[i] = NULL;
			}

			return (false);
		}
	}

	return (true);
}
/**
 * loadMapTextures - loads textures for the default design or the
 * specified design and extracts pixel data
 * @state: pointer to the GameState struct
 * Return: true if all textures were loaded and pixels extracted
 * successfully, false otherwise
 */
bool loadMapTextures(GameState *state)
{
	SDL_Surface *parsedOrigin[TEXTURE_COUNT] = {NULL};

	if (!loadTextures(parsedOrigin))
	{
		return (false);
	}

	extract_Pixels(state, parsedOrigin);

	return (true);
}

/**
 * get_EnvPixelPosition - Responsible for calculating the env pixel position
 * based on wall side and ray direction
 * @mapPos: Represents position of the map in the state
 * @rayDir: Represents the direction of the ray
 * @wallX: Represents the X-coordinate of the wall intersection
 * @wallSide: Indicates which side of the wall is being hit (0 for vertical,
 * 1 for horizontal)
 *
 * Return: The calculated environment pixel position
 */
point_t get_EnvPixelPosition(SDL_Point mapPos, point_t rayDir, double wallX,
int wallSide)
{
	point_t envPixelPos;

	if (wallSide == 0 && rayDir.x > 0)
	{
		envPixelPos.x = mapPos.x;
		envPixelPos.y = mapPos.y + wallX;
	}
	else if (wallSide == 0 && rayDir.x < 0)
	{
		envPixelPos.x = mapPos.x + 1.0;
		envPixelPos.y = mapPos.y + wallX;
	}
	else if (wallSide == 1 && rayDir.y > 0)
	{
		envPixelPos.x = mapPos.x + wallX;
		envPixelPos.y = mapPos.y;
	}
	else
	{
		envPixelPos.x = mapPos.x + wallX;
		envPixelPos.y = mapPos.y + 1.0;
	}

	return (envPixelPos);
}

/**
 * cast_EnvTextures - Responsible for rendering the environment texture
 * based on the player's position
 * @state: Represents a pointer to the Game structure
 * @mapPos: Represents the position of the map in the state
 * @rayDir: Represents the direction of the ray
 * @distToWall: Represents the distance from the player to the wall
 * @wallX: Represents the X-coordinate of the wall intersection
 * @drawEnd: Represents the Y-coordinate where drawing ends
 * @col: Represents the column index for the texture
 * @wallSide: Indicates which side of the wall is being hit
 * (0 for vertical, 1 for horizontal)
 *
 * Return: void
 */
void cast_EnvTextures(GameState *state, SDL_Point mapPos, point_t rayDir,
		double distToWall, double wallX, int drawEnd, int col, int wallSide)
{

	point_t envPixelPos = get_EnvPixelPosition(mapPos, rayDir, wallX, wallSide);
	point_t currentEnvPixel;
	SDL_Point txPos;
	double weight, currentDist;
	int y, textureIndex;

	textureIndex = wallSide == 0 ? 0 : 1;
	if (drawEnd < 0)
		drawEnd = SCREEN_HEIGHT;

	for (y = drawEnd + 1; y < SCREEN_HEIGHT; y++)
	{
		currentDist = SCREEN_HEIGHT / (2.0 * y - SCREEN_HEIGHT);
		weight = currentDist / distToWall;

		currentEnvPixel.x = weight * envPixelPos.x +
			(1.0 - weight) * state->position.x;
		currentEnvPixel.y = weight * envPixelPos.y +
			(1.0 - weight) * state->position.y;

		txPos.x = (int)(currentEnvPixel.x * TEXTURE_WIDTH) % TEXTURE_WIDTH;
		txPos.y = (int)(currentEnvPixel.y * TEXTURE_HEIGHT) % TEXTURE_HEIGHT;

		if (txPos.x < 0 || txPos.x >= TEXTURE_WIDTH ||
		    txPos.y < 0 || txPos.y >= TEXTURE_HEIGHT)
			continue;

		if (y >= 0 && y < SCREEN_HEIGHT)
			state->screenBuffer[y][col] = state->tiles[textureIndex][txPos.y][txPos.x];
		if (SCREEN_HEIGHT - y >= 0 &&
		    SCREEN_HEIGHT - y < SCREEN_HEIGHT)
		{
			state->screenBuffer[SCREEN_HEIGHT - y][col] =
				state->tiles[5][txPos.y][txPos.x];
			state->screenBuffer[y][col] =
				state->tiles[4][txPos.y][txPos.x];
		}
	}
}
