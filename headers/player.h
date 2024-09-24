#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "defs.h"

void movePlayer(GameState * state, int *maze, double directionX,
		double directionY, double modifier);
void rotatePlayer(GameState *state, double modifier, int direction);
void handlePlayerMovement(GameState *state);

#endif
