#ifndef RAYCAST_H
#define RAYCAST_H

#include "defs.h"

int castRay(Coordinate_F pos, Coordinate_D dir, Coordinate_D plane, int **worldMap, int stripe, int *drawStart, int *drawEnd, int *side);
void renderWalls(Coordinate_F pos, Coordinate_D dir, Coordinate_D plane, int **worldMap);

#endif
