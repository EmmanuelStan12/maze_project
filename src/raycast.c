#include "raycast.h"
#include "defs.h"
#include "graphics.h"
#include "utils.h"
#include <math.h>

/**
 * DDA - Digital Differential Analysis
 *
 * @side: the direction the wall is (0 for NS, 1 for EW).
 * @sideDist: distance from the player to the next grid along the ray.
 * @deltaDist: distance between consecutive intersections on the grid.
 * @tile: the current x and y position of the player.
 * @step: the step direction along the x and y axes (-1 or 1).
 * @worldMap: 2D array representing the map grid.
 * Return: value at the current position in worldMap.
 */
int DDA(int *side, Coordinate_D *sideDist, Coordinate_D deltaDist,
        Coordinate_I *tile, Coordinate_I step, int (**worldMap))
{
    int hit = 0;
    int code = 0;

    while (hit == 0)
    {
        if (sideDist->x < sideDist->y)
        {
            sideDist->x += deltaDist.x;
            tile->x += step.x;
            *side = 0;
        }
        else
        {
            sideDist->y += deltaDist.y;
            tile->y += step.y;
            *side = 1;
        }

        code = *(*(worldMap + tile->x) + tile->y);
        if (code > 0)
            hit = 1;
    }

    return (code);
}

/**
 * findRayCoordinates - Calculates ray direction for current stripe.
 *
 * @cameraX: camera X value from -1 to +1 (relative to the plane).
 * @dir: direction vector of the player.
 * @plane: perpendicular plane vector.
 * Return: Ray direction as a Coordinate_D struct.
 */
Coordinate_D findRayCoordinates(double cameraX, Coordinate_D dir, Coordinate_D plane)
{
    Coordinate_D coords;

    coords.x = dir.x + plane.x * cameraX;
    coords.y = dir.y + plane.y * cameraX;

    return (coords);
}

/**
 * findCoordinateSteps - Calculate steps and initial side distances.
 *
 * @step: pointer to step coordinates.
 * @sideDist: pointer to side distance coordinates.
 * @tile: current tile of the player.
 * @rayDir: direction of the ray.
 * @deltaDist: distance between successive intersections.
 * @pos: current position of the player.
 * Return: void
 */
void findCoordinateSteps(Coordinate_I *step, Coordinate_D *sideDist,
                         Coordinate_I tile, Coordinate_D rayDir,
                         Coordinate_D deltaDist, Coordinate_F pos)
{
    if (rayDir.x < 0)
    {
        step->x = -1;
        sideDist->x = (pos.x - tile.x) * deltaDist.x;
    }
    else
    {
        step->x = 1;
        sideDist->x = (tile.x + 1.0 - pos.x) * deltaDist.x;
    }

    if (rayDir.y < 0)
    {
        step->y = -1;
        sideDist->y = (pos.y - tile.y) * deltaDist.y;
    }
    else
    {
        step->y = 1;
        sideDist->y = (tile.y + 1.0 - pos.y) * deltaDist.y;
    }
}

int cast_Ray2(int **worldMap, int x, int *drawStart, int *drawEnd, int *side)
{
    double posX = 2.0, posY = 2.0; // Player's start position
double dirX = -1.0, dirY = 0.0; // Initial direction
double planeX = 0.0, planeY = 0.66; // The 2D raycaster version of the camera plane

    // Calculate ray position and direction
    double cameraX = 2 * x / (double)WINDOW_WIDTH - 1; // X-coordinate in camera space
    double rayDirX = dirX + planeX * cameraX;
    double rayDirY = dirY + planeY * cameraX;

    // Which box of the map we're in
    int mapX = (int)posX;
    int mapY = (int)posY;

    // Length of ray from one x or y-side to next x or y-side
    double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
    double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

    // Calculate step and initial sideDist
    int stepX, stepY;
    double sideDistX, sideDistY;

    // Initial values for step and sideDist
    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }

    // Perform DDA (Digital Differential Analysis)
    int hit = 0; // Was there a wall hit?
    int code;
    while (hit == 0) {
        // Jump to next map square
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            *side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            *side = 1;
        }
        // Check if we hit a wall
        code = worldMap[mapX][mapY];
        if (code > 0) hit = 1;
    }

    // Calculate distance projected on camera direction (Euclidean distance will give a fisheye effect)
    double perpWallDist = (*side == 0)
                          ? (mapX - posX + (1 - stepX) / 2) / rayDirX
                          : (mapY - posY + (1 - stepY) / 2) / rayDirY;

    // Calculate height of line to draw on screen
    int lineHeight = (int)(WINDOW_HEIGHT / perpWallDist);

    // Calculate lowest and highest pixel to fill in current stripe
    *drawStart = -lineHeight / 2 + WINDOW_HEIGHT / 2;
    if (*drawStart < 0) *drawStart = 0;
    *drawEnd = lineHeight / 2 + WINDOW_HEIGHT / 2;
    if (*drawEnd >= WINDOW_HEIGHT) *drawEnd = WINDOW_HEIGHT - 1;

    return (code);
}



/**
 * renderWalls - Render vertical wall slices.
 *
 * @pos: current player position.
 * @dir: player direction vector.
 * @plane: perpendicular plane vector.
 * @worldMap: 2D array of map tiles.
 * Return: void
 */
void renderWalls(Coordinate_F pos, Coordinate_D dir, Coordinate_D plane, int **worldMap)
{
    int i;

    for (i = 0; i < WINDOW_WIDTH; i++)
    {
        int drawStart, drawEnd, side;
        // int code = castRay(pos, dir, plane, worldMap, i, &drawStart, &drawEnd, &side);
        int code = cast_Ray2(worldMap, i, &drawStart, &drawEnd, &side);
        render_WallStrip(drawStart, drawEnd, code, side, i);
    }
}

/**
 * castRay - Cast a ray for a single vertical slice.
 *
 * @pos: current player position.
 * @dir: player direction vector.
 * @plane: perpendicular plane vector.
 * @worldMap: 2D array of map tiles.
 * @stripe: current vertical slice index.
 * @drawStart: pointer to start drawing pixel.
 * @drawEnd: pointer to end drawing pixel.
 * @side: pointer to side (0 for x-side, 1 for y-side).
 * Return: value at the current position in worldMap.
 */
int castRay(Coordinate_F pos, Coordinate_D dir, Coordinate_D plane,
            int **worldMap, int stripe, int *drawStart, int *drawEnd, int *side)
{
    double cameraX = 2 * stripe / (double)WINDOW_WIDTH - 1;
    Coordinate_D rayDir = findRayCoordinates(cameraX, dir, plane);
    Coordinate_I currentTile = { (int)pos.x, (int)pos.y };

    double deltaDistX = (rayDir.x == 0) ? 1e30 : fabs(1 / rayDir.x);
    double deltaDistY = (rayDir.y == 0) ? 1e30 : fabs(1 / rayDir.y);

    Coordinate_D deltaDist = { deltaDistX, deltaDistY };
    Coordinate_I step = { 0, 0 };
    Coordinate_D sideDist = { 0.0, 0.0 };

    findCoordinateSteps(&step, &sideDist, currentTile, rayDir, deltaDist, pos);

    int code = DDA(side, &sideDist, deltaDist, &currentTile, step, worldMap);

    int distToWall = (*side == 0) ? (sideDist.x - deltaDist.x) : (sideDist.y - deltaDist.y);

    printf("Distance to wall %d\n", distToWall);
    printCoordinate(&rayDir, DOUBLE_COORD, "Ray dir");
    printCoordinate(&sideDist, INT_COORD, "Side dist");
    printCoordinate(&deltaDist, DOUBLE_COORD, "Delta dist");
    printCoordinate(&currentTile, INT_COORD, "Player tile");
    if (distToWall == 0)
        distToWall = 1;

    int lineHeight = ((int)WINDOW_HEIGHT / distToWall);

    *drawStart = (WINDOW_HEIGHT / 2) - (lineHeight / 2);
    if (*drawStart < 0)
        *drawStart = 0;

    *drawEnd = lineHeight / 2 + WINDOW_HEIGHT / 2;
    if (*drawEnd >= WINDOW_HEIGHT)
        *drawEnd = WINDOW_HEIGHT - 1;

    printf("DrawStart: %d, DrawEnd: %d\n", *drawStart, *drawEnd);

    return (code);
}
