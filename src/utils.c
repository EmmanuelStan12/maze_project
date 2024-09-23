#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * allocateMap - Allocates memory for a 2D map.
 * @size: The size of the 2D array (size x size).
 *
 * Return: Pointer to the allocated 2D array.
 */
int **allocateMap(int size)
{
	int **map;
	int i;

	map = malloc(size * sizeof(int *));
	if (map == NULL)
		return (NULL);
	for (i = 0; i < size; i++)
	{
		map[i] = malloc(size * sizeof(int));
		if (map[i] == NULL)
		{
			for (i = 0; i < size; i++)
				free(map[i]);
			free(map);
			return (NULL);
		}
	}
	return (map);
}

/**
 * freeMap - Frees the allocated memory for a 2D map.
 * @map: The map to free.
 * @size: The size of the map.
 */
void freeMap(int **map, int size)
{
	int i;

	for (i = 0; i < size; i++)
		free(map[i]);
	free(map);
}

/**
 * readMapFromFile - Reads a map from a file.
 * @filename: The file containing the map.
 * @size: Pointer to store the size of the map.
 *
 * Return: Pointer to the 2D map, or NULL on failure.
 */
int **readMapFromFile(const char *filename, int *size)
{
	FILE *file;
	int **map, i, j;

	file = fopen(filename, "r");
	if (file == NULL)
	{
		fprintf(stderr, "Error: Unable to open file %s\n", filename);
		return (NULL);
	}

	/* Read the size of the map from the first line */
	if (fscanf(file, "%d", size) != 1 || *size <= 0)
	{
		fprintf(stderr, "Error: Invalid map size in file\n");
		fclose(file);
		return (NULL);
	}

	/* Allocate memory for the map */
	map = allocateMap(*size);
	if (map == NULL)
	{
		fclose(file);
		return (NULL);
	}

	/* Read the map data */
	for (i = 0; i < *size; i++)
	{
		for (j = 0; j < *size; j++)
		{
			if (fscanf(file, "%d", &map[i][j]) != 1)
			{
				fprintf(stderr, "Error: Invalid data in file\n");
				freeMap(map, *size);
				fclose(file);
				return (NULL);
			}
		}
	}

	fclose(file);
	return (map);
}

/**
 * printMap - Prints the map to the console.
 * @map: The map to print.
 * @size: The size of the map.
 */
void printMap(int **map, int size)
{
	int i, j;

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
			printf("%d ", map[i][j]);
		printf("\n");
	}
}

/**
 * printCoordinate - Prints the coordinate values based on the type
 * @coord: pointer to the coordinate struct (void*)
 * @type: the type of coordinate (enum CoordType)
 * @prefix: the prefix of the print statement
 *
 * Return: void
 */
void printCoordinate(void *coord, CoordType type, char *prefix)
{
    switch (type)
    {
    case INT_COORD: {
        Coordinate_I *c = (Coordinate_I *) coord;
        printf("%s -> Coordinate_I: x = %d, y = %d\n", prefix, c->x, c->y);
        break;
    }
    case DOUBLE_COORD: {
        Coordinate_D *c = (Coordinate_D *) coord;
        printf("%s -> Coordinate_D: x = %lf, y = %lf\n", prefix, c->x, c->y);
        break;
    }
    case FLOAT_COORD: {
        Coordinate_F *c = (Coordinate_F *) coord;
        printf("%s -> Coordinate_F: x = %f, y = %f\n", prefix, c->x, c->y);
        break;
    }
    default:
        printf("Unknown coordinate type\n");
        break;
    }
}
