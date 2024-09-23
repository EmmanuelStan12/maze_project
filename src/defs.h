#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

#define WINDOW_WIDTH 640 
#define WINDOW_HEIGHT 480

/**
 * struct Coordinate_I - Struct for integer coordinates
 * @x: x coordinate
 * @y: y coordinate
 */
typedef struct
{
    int x;
    int y;
} Coordinate_I;

/**
 * struct Coordinate_D - Struct for double coordinates
 * @x: x coordinate
 * @y: y coordinate
 */
typedef struct
{
    double x;
    double y;
} Coordinate_D;

/**
 * struct Coordinate_F - Struct for float coordinates
 * @x: x coordinate
 * @y: y coordinate
 */
typedef struct
{
    float x;
    float y;
} Coordinate_F;

/**
 * enum CoordType - Enum to define the coordinate type
 * @INT_COORD: Integer coordinate
 * @DOUBLE_COORD: Double coordinate
 * @FLOAT_COORD: Float coordinate
 */
typedef enum
{
    INT_COORD,
    DOUBLE_COORD,
    FLOAT_COORD
} CoordType;

#endif // !DEBUG
