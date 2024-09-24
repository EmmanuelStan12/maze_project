#include "player.h"

/**
 * movePlayer - handling player movement based on keyboard input
 * @state: Pointer to the GameState structure containing state state
 * @maze: Pointer to the integer array representing the maze layout
 * @directionX: X direction of movement
 * @directionY: Y direction of movement
 * @modifier: Movement speed modifier
 */
void movePlayer(GameState *state, int *maze, double directionX,
		double directionY, double modifier)
{
    printGameState(state);
	int newSpotX = (int)(state->position.x + directionX * modifier);
	int newSpotY = (int)(state->position.y + directionY * modifier);

    printf("newSpotX %d, newSpotY %d, spot %d, first %d\n", newSpotX, newSpotY, maze[(newSpotX * MAP_WIDTH) + newSpotY], state->maze[0]);
	if (newSpotX >= 0 && newSpotX < MAP_WIDTH && newSpotY >= 0 &&
	    newSpotY < MAP_HEIGHT && !maze[newSpotX * MAP_WIDTH + newSpotY])
	{
		state->position.x += directionX * modifier;
		state->position.y += directionY * modifier;
	}
}

/**
 * rotatePlayer - Handles player rotation based on keyboard input
 * @state: Pointer to the GameState structure containing state state
 * @modifier: Rotation speed modifier
 * @direction: Direction of rotation (positive for right, negative for left)
 */
void rotatePlayer(GameState *state, double modifier, int direction)
{
	double prevDirX = state->direction.x;
	double prevPlaneX = state->viewPlane.x;

	state->direction.x = state->direction.x * cos(direction * modifier) -
			    state->direction.y * sin(direction * modifier);
	state->direction.y = prevDirX * sin(direction * modifier) +
			    state->direction.y * cos(direction * modifier);

	state->viewPlane.x = state->viewPlane.x * cos(direction * modifier) -
			    state->viewPlane.y * sin(direction * modifier);
	state->viewPlane.y = prevPlaneX * sin(direction * modifier) +
			    state->viewPlane.y * cos(direction * modifier);
}

/**
 * handlePlayerMovement - Handles player movement and rotation based on input
 * @state: Pointer to the GameState structure containing state state
 */
void handlePlayerMovement(GameState *state)
{
	const uint8_t *keyboardState;
	double previousTime;
	double currentTime;
	double moveModifier;
	double rotModifier;
	int *maze = state->maze;

	keyboardState = SDL_GetKeyboardState(NULL);
	previousTime = state->time;
	state->time = SDL_GetTicks();
	currentTime = (state->time - previousTime) / 1000.0;
	moveModifier = currentTime * 5.0;
	rotModifier = currentTime * 3.0;
	if (keyboardState[SDL_SCANCODE_W])
	{
	    printf("W clicked %.2f\n", moveModifier);
		movePlayer(state, maze, state->direction.x,
			   state->direction.y, moveModifier);
	}
	if (keyboardState[SDL_SCANCODE_S])
	{
		movePlayer(state, maze, -state->direction.x,
			   -state->direction.y, moveModifier);
	}
	if (keyboardState[SDL_SCANCODE_E])
	{
		movePlayer(state, maze, state->viewPlane.x,
			   state->viewPlane.y, moveModifier);
	}
	if (keyboardState[SDL_SCANCODE_D])
	{
	    printf("D clicked\n");
		rotatePlayer(state, rotModifier, 1);
	}
	if (keyboardState[SDL_SCANCODE_A])
	{
		rotatePlayer(state, rotModifier, -1);
	}
}
