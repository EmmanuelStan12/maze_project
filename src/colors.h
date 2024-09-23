#ifndef COLORS_H
#define COLORS_H

#include <SDL2/SDL_stdinc.h>
typedef struct ColorRGBA
{
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    float alpha;
} ColorRGBA;

ColorRGBA generateWallColor(int code, int side);
void printColor(const ColorRGBA *color);

#endif
