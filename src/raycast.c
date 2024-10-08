#include "../headers/raycast.h"

/**
 * drawTexturedWallStrips - Responsible for drawing textured strips.
 * to buffer
 * @state: pointer to the Game structure
 * @map: X/Y coordinates of box of maze currently in
 * @rayPos: X/Y coordinates of ray position
 * @rayDir: direction of X/Y coordinates of ray position
 * @distToWall: distance to wall from camera
 * @x: number of ray casted
 * @side: determines whether wall is N/S or E/W
 * Return: Always void
 */
void drawTexturedWallStrips(GameState *state, SDL_Point map, point_t rayPos,
	point_t rayDir, double distToWall, int x, int side)
{
	int sliceHeight, drawStart, drawEnd, tileIndex, y;
	double wallX;
	SDL_Point tex;
	uint32_t color;
	int *maze = state->maze;

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
	int sliceHeight, drawStart, drawEnd, width, height;

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
		SDL_SetRenderDrawColor(state->renderer, side == 0 ? 192 : 128,
				side == 0 ? 192 : 128,
				side == 0 ? 192 : 128, 255);
		SDL_RenderDrawLine(state->renderer, x, drawStart, x, drawEnd);
	}
	else
	{
		drawTexturedWallStrips(state, map, rayPos, rayDir,
				distToWall, x, side);
	}
}

/**
 * calculateRayPosition - Calculates the ray position and direction
 * @state: current game state.
 * @i: current ray index.
 * @rayPosition: ray position structure.
 * @rayDirection: ray direction structure.
 * @mapPosition: player current position structure.
 * @distanceToNext: distance to difference in successive blocks in the grid.
 * @stepDirection: step direction for x and y.
 * @positionToNext: current position or next position of the player.
 * Return: void
 */
void calculateRayPosition(GameState *state, int i, point_t *rayPosition,
		point_t *rayDirection, SDL_Point *mapPosition, point_t *distanceToNext,
		SDL_Point *stepDirection, point_t *positionToNext)
{
	double cameraX;

	cameraX = 2 * i / (double)SCREEN_WIDTH - 1;

	rayPosition->x = state->position.x;
	rayPosition->y = state->position.y;
	rayDirection->x = state->direction.x + state->viewPlane.x * cameraX;
	rayDirection->y = state->direction.y + state->viewPlane.y * cameraX;

	mapPosition->x = (int)rayPosition->x;
	mapPosition->y = (int)rayPosition->y;

	distanceToNext->x = rayDirection->x == 0 ? INFINITY :
		sqrt(1 + pow(rayDirection->y, 2) / pow(rayDirection->x, 2));
	distanceToNext->y = rayDirection->y == 0 ? INFINITY :
		sqrt(1 + pow(rayDirection->x, 2) / pow(rayDirection->y, 2));

	stepDirection->x = rayDirection->x < 0 ? -1 : 1;
	stepDirection->y = rayDirection->y < 0 ? -1 : 1;

	positionToNext->x = rayDirection->x < 0 ?
		(rayPosition->x - mapPosition->x) * distanceToNext->x :
		(mapPosition->x + 1.0 - rayPosition->x) *
		distanceToNext->x;
	positionToNext->y = rayDirection->y < 0 ?
		(rayPosition->y - mapPosition->y) * distanceToNext->y :
		(mapPosition->y + 1.0 - rayPosition->y) *
		distanceToNext->y;
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
	point_t rayPosition, rayDirection, positionToNext, distanceToNext;
	SDL_Point mapPosition, stepDirection;
	int hit, side = 0;
	double distanceToWall;
	int *maze = state->maze;

	for (i = 0; i < SCREEN_WIDTH; i++)
	{
		calculateRayPosition(state, i, &rayPosition, &rayDirection,
			&mapPosition, &distanceToNext, &stepDirection, &positionToNext);
		for (hit = 0; hit == 0;)
		{
			if (positionToNext.x < positionToNext.y)
			{
				positionToNext.x += distanceToNext.x;
				mapPosition.x += stepDirection.x;
				side = 0;
			}
			else
			{
				positionToNext.y += distanceToNext.y;
				mapPosition.y += stepDirection.y;
				side = 1;
			}

			if (*((int *)maze + mapPosition.x * MAP_WIDTH + mapPosition.y) > 0)
				hit = 1;
		}
		distanceToWall = side == 0 ? (mapPosition.x - rayPosition.x +
			(1 - stepDirection.x) / 2) / rayDirection.x :
			(mapPosition.y - rayPosition.y + (1 - stepDirection.y) / 2) /
			rayDirection.y;

		drawWallStrips(state, mapPosition, rayPosition, rayDirection,
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
	SDL_Rect rectCeiling;
	SDL_Rect rectFloor;
	int windowWidth;
	int windowHeight;


	SDL_GetWindowSize(state->window, &windowWidth, &windowHeight);

	rectCeiling.x = 0;
	rectCeiling.y = 0;
	rectCeiling.w = windowWidth;
	rectCeiling.h = windowHeight / 2;

	rectFloor.x = 0;
	rectFloor.y = windowHeight / 2;
	rectFloor.w = windowWidth;
	rectFloor.h = windowHeight / 2;

	SDL_SetRenderDrawColor(state->renderer, 135, 206, 235, 255);
	SDL_RenderFillRect(state->renderer, &rectCeiling);

	SDL_SetRenderDrawColor(state->renderer, 34, 139, 34, 255);
	SDL_RenderFillRect(state->renderer, &rectFloor);
}

