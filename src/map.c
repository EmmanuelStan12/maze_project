#include "map.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * readMapFromFile - Reads a map from a file.
 * @filename: The file containing the map.
 *
 * Return: Pointer to the 2D map, or NULL on failure.
 */
int *readMapFromFile(char *filename)
{
	FILE *file;
	int *map, i, j;

	file = fopen(filename, "r");
	if (file == NULL)
	{
		fprintf(stderr, "Error: Unable to open file %s\n", filename);
		return (NULL);
	}

	/* Allocate memory for the map */
	map = malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(int));
	if (map == NULL)
	{
		fclose(file);
		return (NULL);
	}

	/* Read the map data */
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH; j++)
		{
			int value;
			if (fscanf(file, "%d", &value) != 1)
			{
				fprintf(stderr, "Error: Invalid data in file\n");
				free(map);
				fclose(file);
				return (NULL);
			}
			map[i * MAP_WIDTH + j] = value;
		}
	}
	fclose(file);
	return (map);
}

/**
 * printMap - Prints the map in rows and columns.
 * @map: Pointer to the 2D map array.
 *
 * Return: void
 */
void printMap(int *map)
{
	int i, j;

	if (map == NULL)
	{
		fprintf(stderr, "Error: Map is NULL\n");
		return;
	}

	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH; j++)
		{
			printf("%d ", map[i * MAP_WIDTH + j]);
		}
		printf("\n");
	}
}
