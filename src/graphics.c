#include "graphics.h"
#include "colors.h"
#include "defs.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

/* Global pointer to hold the SDL instance */
static SDL_Instance *instance = NULL;

/**
 * init_SDLInstance - Initialize SDL window and renderer
 * Return: 0 on success, 1 on failure
 */
int init_SDLInstance(void)
{
    /* Allocate memory for the instance */
    instance = malloc(sizeof(SDL_Instance));
    if (instance == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for SDL instance\n");
        return (1);
    }

    /* Initialize SDL with video subsystem */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        free(instance);
        return (1);
    }

    /* Create an SDL window */
    instance->window = SDL_CreateWindow(
        NULL,                        /* Window title */
        SDL_WINDOWPOS_CENTERED,       /* x position */
        SDL_WINDOWPOS_CENTERED,       /* y position */
        WINDOW_WIDTH,                 /* Window width */
        WINDOW_HEIGHT,                /* Window height */
        SDL_WINDOW_RESIZABLE          /* Window flags */
    );

    if (instance->window == NULL)
    {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        free(instance);
        return (1);
    }

    /* Create an SDL renderer with hardware acceleration and VSync */
    instance->renderer = SDL_CreateRenderer(
        instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (instance->renderer == NULL)
    {
        SDL_DestroyWindow(instance->window);
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        SDL_Quit();
        free(instance);
        return (1);
    }

    /* Set the draw color to black and clear the screen */
    SDL_SetRenderDrawColor(instance->renderer, 0, 0, 0, 255);
    SDL_RenderClear(instance->renderer);
    SDL_RenderPresent(instance->renderer);

    /* Create a texture for rendering */
    instance->texture = SDL_CreateTexture(
        instance->renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    if (instance->texture == NULL)
    {
        SDL_DestroyRenderer(instance->renderer);
        SDL_DestroyWindow(instance->window);
        fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
        SDL_Quit();
        free(instance);
        return (1);
    }

    return (0);
}

/**
 * destroy_SDLInstance - Frees all SDL resources
 */
void destroy_SDLInstance(void)
{
    /* Destroy texture, renderer, and window in reverse order of creation */
    SDL_DestroyTexture(instance->texture);
    SDL_DestroyRenderer(instance->renderer);
    SDL_DestroyWindow(instance->window);

    /* Free the instance memory */
    free(instance);

    /* Quit SDL subsystems */
    SDL_Quit();
}

/**
* render_WallStripe - renders a wall stripe
*
* @drawStart: start of the drawing
* @drawEnd: end of the drawing
* @code: the code of the tile
* @side: the direction of the ray.
* @stripe: the vertical stripe.
* Returns: void
*/
void render_WallStrip(int drawStart, int drawEnd, int code, int side, int stripe)
{
    ColorRGBA color = generateWallColor(code, side);
    printColor(&color);  /* Print the color for debugging purposes */

    /* Set the color to the renderer */
    SDL_SetRenderDrawColor(instance->renderer, color.red, color.green, color.blue, color.alpha);
    SDL_RenderClear(instance->renderer);
    SDL_RenderDrawLine(instance->renderer, stripe, drawStart, stripe, drawEnd);
    /* Render only within the range from drawStart to drawEnd */ 
    SDL_RenderPresent(instance->renderer);
}

