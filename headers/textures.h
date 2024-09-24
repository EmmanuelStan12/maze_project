#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "defs.h"

void get_TexturePaths(char *textureFiles[]);
bool loadTextures(SDL_Surface * parsedOrigin[]);
bool loadMapTextures(GameState *state);
point_t get_EnvPixelPosition(SDL_Point mapPos, point_t rayDir,
		double wallX, int wallSide);
void cast_EnvTextures(GameState *state, SDL_Point mapPos,
		point_t rayDir, double distToWall,
		double wallX, int drawEnd, int col, int wallSide);
#endif
