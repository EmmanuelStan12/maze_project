#include "raycast.h"

/**
 * drawWallStrips - Responsible for drawing slice of wall
 * to buffer
 * @state: pointer to the Game structure
 * @map: X/Y coordinates of box of maze currently in
 * @rayPos: X/Y coordinates of ray position
 * @rayDir: direction of X/Y coordinates of ray position
 * @distToWall: distance to wall from camera
 * @x: number of ray casted
 * @side: determines whether wall is N/S or E/W
 * @textured: True if user enabled textures, otherwise False
 * Return: Always void
 */
void drawWallStrips(GameState *state, SDL_Point map, point_t rayPos,
	point_t rayDir, double distToWall, int x, int side, int textured)
{
	int sliceHeight, drawStart, drawEnd, tileIndex, width, height, y;
	double wallX;
	SDL_Point tex;
	uint32_t color;
	int *maze = state->maze;

	if (!state || !maze)
		return;
	if (!textured)
	{
		SDL_GetWindowSize(state->window, &width, &height);
		sliceHeight = (int)(height / distToWall);
		drawStart = -sliceHeight / 2 + height / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = sliceHeight / 2 + height / 2;
		if (drawEnd >= height)
			drawEnd = height - 1;
		SDL_SetRenderDrawColor(state->renderer,
            side == 0 ? 192 : 128,
            side == 0 ? 192 : 128,
            side == 0 ? 192 : 128, 255);
		SDL_RenderDrawLine(state->renderer, x, drawStart, x, drawEnd);
	}
	else
	{
		sliceHeight = (int)(SCREEN_HEIGHT / distToWall);
		drawStart = -sliceHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = sliceHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawEnd >= SCREEN_HEIGHT)
			drawEnd = SCREEN_HEIGHT - 1;
		wallX = side == 0 ? rayPos.y + distToWall * rayDir.y
			: rayPos.x + distToWall * rayDir.x;
		if (map.x < 0 || map.x >= MAP_WIDTH || map.y < 0 ||
			map.y >= MAP_HEIGHT)
			return;
		tileIndex = *((int *)maze + map.x * MAP_WIDTH + map.y) - 1;
		wallX -= floor(wallX);
		tex.x = (int)(wallX * (double)TEXTURE_WIDTH);
		if (tex.x < 0 || tex.x >= TEXTURE_WIDTH)
			return;
		if ((side == 0 && rayDir.x > 0) || (side == 1 && rayDir.y < 0))
			tex.x = TEXTURE_WIDTH - tex.x - 1;
		for (y = drawStart; y < drawEnd; y++)
		{
			tex.y = ((((y << 1) - SCREEN_HEIGHT + sliceHeight)
				<< (int)log2(TEXTURE_HEIGHT)) / sliceHeight) >> 1;
			if (tex.y < 0 || tex.y >= TEXTURE_HEIGHT)
				continue;
			color = state->tiles[tileIndex][tex.x][tex.y];
			if (side == 1)
				color = (color >> 1) & 0x7F7F7F;
			if (y >= 0 && y < SCREEN_HEIGHT)
				state->screenBuffer[y][x] = color;
		}
		cast_EnvTextures(state, map, rayDir, distToWall, wallX,
			drawEnd, x, side);
	}
}

/**
 * calculateRayPosition - Calculates the ray position and direction
 * @state: pointer to the GameState structure
 * @stripe: current ray counter
 * @rayPosition: pointer to the ray position structure
 * @rayDirection: pointer to the ray direction structure
 * @tilePosition: pointer to the map position structure
 * @deltaDistance: pointer to the distance to next structure
 * @stepDirection: pointer to the step direction structure
 * @sidePosition: pointer to the position to next structure
 * Return: void
 */
void calculateRayPosition(GameState *state, int i, point_t *rayPosition,
		point_t *rayDirection, SDL_Point *currentPosition, point_t *deltaDistance,
		SDL_Point *stepDirection, point_t *sidePosition)
{
	double camX;

	camX = 2 * i / (double)SCREEN_WIDTH - 1;

	rayPosition->x = state->position.x;
	rayPosition->y = state->position.y;
	rayDirection->x = state->direction.x + state->viewPlane.x * camX;
	rayDirection->y = state->direction.y + state->viewPlane.y * camX;

	currentPosition->x = (int)rayPosition->x;
	currentPosition->y = (int)rayPosition->y;

	deltaDistance->x = rayDirection->x == 0 ? INFINITY :
		sqrt(1 + pow(rayDirection->y, 2) / pow(rayDirection->x, 2));
	deltaDistance->y = rayDirection->y == 0 ? INFINITY :
		sqrt(1 + pow(rayDirection->x, 2) / pow(rayDirection->y, 2));

	stepDirection->x = rayDirection->x < 0 ? -1 : 1;
	stepDirection->y = rayDirection->y < 0 ? -1 : 1;

	sidePosition->x = rayDirection->x < 0 ? (rayPosition->x - currentPosition->x) *
		deltaDistance->x : (currentPosition->x + 1.0 - rayPosition->x) *
		deltaDistance->x;
	sidePosition->y = rayDirection->y < 0 ? (rayPosition->y - currentPosition->y) *
		deltaDistance->y : (currentPosition->y + 1.0 - rayPosition->y) *
		deltaDistance->y;
}

/**
 * renderWalls - Renders vertical wall slices
 * @state: pointer to the GameState structure
 * @textured: flag to determine whether to render textures
 * Return: void
 */
void renderWalls(GameState *state, int textured)
{
    int i;
	point_t rayPosition, rayDirection, sidePosition, deltaDistance;
	SDL_Point currentPosition, stepDirection;
	int hit, side;
	double distanceToWall;
    int *maze = state->maze;

	for (i = 0; i < SCREEN_WIDTH; i++)
	{
		calculateRayPosition(state, i, &rayPosition, &rayDirection,
			&currentPosition, &deltaDistance, &stepDirection, &sidePosition);

		for (hit = 0; hit == 0;)
		{
			if (sidePosition.x < sidePosition.y)
			{
				sidePosition.x += deltaDistance.x;
				currentPosition.x += stepDirection.x;
				side = 0;
			}
			else
			{
				sidePosition.y += deltaDistance.y;
				currentPosition.y += stepDirection.y;
				side = 1;
			}

			if (*((int *)maze + currentPosition.x * MAP_WIDTH + currentPosition.y) > 0)
				hit = 1;
		}

		distanceToWall = side == 0 ? (currentPosition.x - rayPosition.x +
			(1 - stepDirection.x) / 2) / rayDirection.x :
			(currentPosition.y - rayPosition.y + (1 - stepDirection.y) / 2) /
			rayDirection.y;

		drawWallStrips(state, currentPosition, rayPosition, rayDirection,
			distanceToWall, i, side, textured);
	}

	update_SDLFrames(state, textured);
}

/**
 * castCeilingAndFloor - Renders the background ceiling and floor
 * @state: pointer to the GameState structure
 * Return: void
 */
void castCeilingAndFloor(GameState *state)
{
	SDL_Rect bgCeiling;
    SDL_Rect bgFloor;
    int winWidth;
    int winHeight;


    SDL_GetWindowSize(state->window, &winWidth, &winHeight);

    bgCeiling.x = 0;
    bgCeiling.y = 0;
    bgCeiling.w = winWidth;
    bgCeiling.h = winHeight / 2;

    bgFloor.x = 0;
    bgFloor.y = winHeight / 2;
    bgFloor.w = winWidth;
    bgFloor.h = winHeight / 2;

    /* draw background ceiling */
    SDL_SetRenderDrawColor(state->renderer, 135, 206, 235, 255);
    SDL_RenderFillRect(state->renderer, &bgCeiling);

    /* draw background floor */
    SDL_SetRenderDrawColor(state->renderer, 34, 139, 34, 255);
    SDL_RenderFillRect(state->renderer, &bgFloor);
}

