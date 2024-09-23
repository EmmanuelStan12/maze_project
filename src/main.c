#include "defs.h"
#include "graphics.h"
#include "raycast.h"
#include "utils.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

int isRunning = 1;

/*
 * processInput - process input from the sdl instance.
 *
 * Returns: void
 * */
void processInput()
{
    SDL_Event event;

    SDL_PollEvent(&event);
    switch (event.type)
    {
        case SDL_QUIT:
        {
            isRunning = 0;
            break;
        }
        case SDL_KEYDOWN:
        {
            break;
        }
    }
 
}

/*
 * render - renders the current frame of the player
 *
 * Returns: void
 */
void render(Coordinate_F pos, Coordinate_D dir, Coordinate_D plane, int **map)
{
    renderWalls(pos, dir, plane, map);
}

/*
 * main - Entry point
 *
 * Returns: status of the execution
 * */
int main() {
    const char *filename = "map.txt";
    int **map, size;

    map = readMapFromFile(filename, &size);
    if (map == NULL)
        return (1);

    printMap(map, size);

    double time = 0;
    double oldTime = 0;
    Coordinate_F currentPosition = { 22.0f, 12.0f };
    Coordinate_D dir = { -1, 0 };
    Coordinate_D plane = { 0, 0.66 };

    if (init_SDLInstance())
    {
        freeMap(map, size);
        return (1);
    }

    while (isRunning)
    {
        printf("started running %d\n", isRunning);
        processInput();
        render(currentPosition, dir, plane, map);
    }

    destroy_SDLInstance();
    freeMap(map, size);
    return (0);
}

