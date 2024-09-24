#include "../headers/defs.h"
#include "../headers/graphics.h"
#include "../headers/player.h"
#include "../headers/raycast.h"
#include "../headers/map.h"

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
 * initializeState - initializes the state of the application
 *
 * @state: the initial state.
 * @maze: the current map of the game.
 * Returns: voide
 */
void initializeState(GameState *state, int *maze)
{
	memset(state, 0, sizeof(GameState));

	state->position.x = 1;
	state->position.y = 12;
	state->direction.x = 1;
	state->direction.y = -0.66;
	state->viewPlane.x = 0;
	state->viewPlane.y = 0.66;
	state->time = 0;
	state->quit = false;
	state->maze = maze;
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
	int textured = true;

	map = readMapFromFile(filename);
	if (map == NULL)
		return (1);

	initializeState(&state, map);
	if (!init_SDLInstance(&state))
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
