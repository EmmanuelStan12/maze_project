#include "graphics.h"
#include "defs.h"

/**
 * init_SDLInstance - Initialize SDL window and renderer
 * @state: Pointer to GameState structure
 * Return: 0 on success, 1 on failure
 */
bool init_SDLInstance(GameState *state, int *maze)
{
	/* Initialize the state structure */
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

	/* Initialize SDL with video subsystem */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return (false);
	}

	/* Create an SDL window */
	state->window = SDL_CreateWindow(
		"Maze",                        /* Window title */
		SDL_WINDOWPOS_UNDEFINED,       /* x position */
		SDL_WINDOWPOS_UNDEFINED,       /* y position */
		SCREEN_WIDTH,                 /* Window width */
		SCREEN_HEIGHT,                /* Window height */
		SDL_WINDOW_SHOWN          /* Window flags */
	);

	if (state->window == NULL)
	{
		fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
		SDL_Quit();
		return (false);
	}

	/* Create an SDL renderer with hardware acceleration and VSync */
	state->renderer = SDL_CreateRenderer(
		state->window, -1,
		SDL_RENDERER_ACCELERATED
	);

	if (state->renderer == NULL)
	{
		SDL_DestroyWindow(state->window);
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		SDL_Quit();
		return (false);
	}

	/* Create a texture for rendering */
	state->texture = SDL_CreateTexture(
		state->renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH,
		SCREEN_HEIGHT
	);

	if (state->texture == NULL)
	{
		SDL_DestroyRenderer(state->renderer);
		SDL_DestroyWindow(state->window);
		fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
		SDL_Quit();
		return (false);
	}

	return (true);
}

/**
 * destroy_SDLInstance - Frees all SDL resources
 * @state: Pointer to GameState structure
 * Return: void
 */
void destroy_SDLInstance(GameState *state)
{
	/* Destroy texture, renderer, and window in reverse order of creation */
	SDL_DestroyTexture(state->texture);
	SDL_DestroyRenderer(state->renderer);
	SDL_DestroyWindow(state->window);

	/* Quit SDL subsystems */
	SDL_Quit();
}

/**
 * update_SDLFrames - Updating renderer with updated buffer / texture
 * @state: Pointer to the GameState structure
 * @textured: True if user enabled textures, otherwise False
 * Return: void
 */
void update_SDLFrames(GameState *state, int textured)
{
	int x, y;

	if (!state || !state->renderer)
	{
		return;  /* Check for NULL pointers */
	}

	/* Draw buffer to renderer */
	if (textured && state->texture)
	{
		SDL_UpdateTexture(state->texture, NULL, state->screenBuffer,
			SCREEN_WIDTH * 4);
		SDL_RenderClear(state->renderer);
		SDL_RenderCopy(state->renderer, state->texture, NULL, NULL);

		/* Clear buffer */
		for (x = 0; x < SCREEN_WIDTH; x++)
		{
			for (y = 0; y < SCREEN_HEIGHT; y++)
			{
				state->screenBuffer[y][x] = 0;
			}
		}
	}

	/* Update screen */
	SDL_RenderPresent(state->renderer);
}
