#include "defs.h"

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
    printf("Player Direction: (%f, %f)\n", state->direction.x, state->direction.y);
    printf("Player Plane: (%f, %f)\n", state->viewPlane.x, state->viewPlane.y);
    printf("Time: %f\n", state->time);
    printf("Maze Pointer: %p\n", (void *)state->maze);
}

