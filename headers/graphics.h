#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "textures.h"
#include "defs.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

bool init_SDLInstance(GameState *state);
void destroy_SDLInstance(GameState *state);
void update_SDLFrames(GameState *state, int textured);

#endif
