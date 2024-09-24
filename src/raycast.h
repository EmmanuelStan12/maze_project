#ifndef RAYCAST_H
#define RAYCAST_H

#include "defs.h"
#include "textures.h"
#include "graphics.h"

void drawWallStrips(GameState *state, SDL_Point map, point_t rayPos,
	point_t rayDir, double distToWall, int x, int side, int textured);
void calculateRayPosition(GameState *state, int stripe, point_t *rayPosition,
	point_t *rayDirection, SDL_Point *tilePosition, point_t *deltaDistance,
	SDL_Point *stepDirection, point_t *sideDistance);
void renderWalls(GameState *state, int textured);
void castCeilingAndFloor(GameState *state);

#endif

