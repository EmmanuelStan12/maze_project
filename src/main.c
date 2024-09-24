#include "defs.h"
#include "graphics.h"
#include "player.h"
#include "raycast.h"
#include "map.h"

/**
 * handleExitMaze - Game loop that checks if user quits or
 * toggles fullscreen
 * @state: Represents pointer to the Game struct
 *
 * Return: True if user quits, else False
 */
bool handleExitMaze(GameState *state)
{
	SDL_Event event;
	uint32_t windowFlags;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			return (true);
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return (true);
			case SDLK_q:
				windowFlags = SDL_GetWindowFlags(state->window);
				SDL_SetWindowFullscreen(state->window, (windowFlags &
					SDL_WINDOW_FULLSCREEN) ? 0 :
					SDL_WINDOW_FULLSCREEN);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	SDL_Delay(16);
	return (false);
}

/**
 * runGameLoop - Responsible for running the main state loop
 * @state: Pointer to the state structure
 * @textured: Textured flag
 *
 * Return: Returns 0 on success, non-zero on failure
 */
void runGameLoop(GameState *state, int textured)
{
	while (!state->quit)
	{
		if (!textured)
			castCeilingAndFloor(state);

		renderWalls(state, textured);

		handlePlayerMovement(state);

		if (handleExitMaze(state))
			state->quit = 1;
	}
}

/**
 * main - Entry point
 *
 * Return: status of the execution
 */
int main(void)
{
	char *filename = "assets/maps/map_01";
	int *map;
	GameState state;
	int textured = false;

	map = readMapFromFile(filename);
	if (map == NULL)
		return (1);

	printMap(map);

	if (!init_SDLInstance(&state, map))
	{
		free(map);
        destroy_SDLInstance(&state);
		return (1);
	}

	if (textured)
		loadMapTextures(&state);
	runGameLoop(&state, textured);

	destroy_SDLInstance(&state);
	free(map);
	return (0);
}
