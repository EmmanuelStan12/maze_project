#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL2/SDL.h>
#include "defs.h"
#include "colors.h"

typedef struct SDL_Instance
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} SDL_Instance;

int init_SDLInstance(void);
void destroy_SDLInstance(void);
void render_WallStrip(int drawStart, int drawEnd, int code, int side, int stripe);

#endif
