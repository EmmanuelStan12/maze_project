#include "colors.h"

const ColorRGBA BLACK = {0, 0, 0, 255};
const ColorRGBA WHITE = {255, 255, 255, 255};
const ColorRGBA RED = {255, 0, 0, 255};
const ColorRGBA GREEN = {0, 255, 0, 255};
const ColorRGBA BLUE = {0, 0, 255, 255};
const ColorRGBA YELLOW = {255, 255, 0, 1.0};

/*
 * findWallColor - generates wall color code based on code and side of the ray cast.
 *
 * @code: the integer value of the wall.
 * @side: the direction the ray cast was hit that detected the wall.
 * Return: the color code of the wall.
 * */
ColorRGBA generateWallColor(int code, int side)
{
    ColorRGBA color;

    switch (code)
    {
        case 1:
            color = RED;
            break;
        case 2:
            color = GREEN;
            break;
        case 3:
            color = BLUE;
            break;
        case 4:
            color = WHITE;
            break;
        default:
            color = YELLOW;
            break;
    }

    return (color);
}

/*
 * divideColor - divides the first color by the second
 *
 * @color1: the first color
 * @color2: the second color
 * Returns: the result of the division.
 * */
ColorRGBA divideColor(ColorRGBA color1, ColorRGBA color2)
{
    ColorRGBA result;

    result.red = (color2.red == 0) ? 255 : color1.red / color2.red;
    result.green = (color2.green == 0) ? 255 : color1.green / color2.green;
    result.blue = (color2.blue == 0) ? 255 : color1.blue / color2.blue;
    result.alpha = (color2.alpha == 0) ? 255 : color1.alpha / color2.alpha;

    return result;
}

/**
 * printColor - prints the RGBA values of a ColorRGBA struct
 * @color: a pointer to the ColorRGBA struct
 *
 * Description: This function prints out the red, green, blue, and alpha
 * values of a color in the format: (r, g, b, a).
 * Return: void
 */
void printColor(const ColorRGBA *color)
{
    if (color == NULL)
    {
        printf("Color is NULL\n");
        return;
    }

    printf("Color RGBA: (%d, %d, %d, %f)\n", color->red, color->green, color->blue, color->alpha);
}


