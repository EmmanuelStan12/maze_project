#ifndef _UTILS_H_
#define _UTILS_H_

#include "defs.h"

void printMap(int **map, int size);
int **readMapFromFile(const char *filename, int *size);
void freeMap(int **map, int size);
void printCoordinate(void *coord, CoordType type, char *prefix);

#endif // !_UTILS_H_
