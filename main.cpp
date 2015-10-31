#include <iostream>
#include <stdint.h>
#include "SDL2/SDL.h"
#include "palette_manager.h"

/*
    ANSI COLOR FREAK
    by Brian Puthuff

*/

int main(int argc, char** argv)
{

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 640;

    // initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO < 0))
    {
        std::cerr << "Could not initialize SDL. Program terminated.\n\n";
        return 1;
    }

    SDL_Surface* input_image = SDL_LoadBMP(argv[1]);
    if(input_image == NULL)
    {
        std::cerr << "Could not load input image. Program terminated.\n\n";
        SDL_Quit();
        return 1;
    }

    // create window and render surface
    SDL_Window* window = NULL;
    SDL_Surface* render_Surface = NULL;
    window = SDL_CreateWindow("ANSI Color Freak", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    render_Surface = SDL_GetWindowSurface(window);


    // optimize image surface
    SDL_Surface* optimized_surface = NULL;
    optimized_surface = SDL_ConvertSurface(input_image, render_Surface->format, NULL);

    // instantiate ANSI palette manager object
    paletteManager* ANSI_colors = new paletteManager();


    Uint32* pixels = (Uint32*) optimized_surface->pixels;

    // main loop
    for(int row = 0; row < optimized_surface->h; row++)
    {
        for(int col = 0; col < optimized_surface->w; col++)
        {
            Uint32 pixel = pixels[row * optimized_surface->w + col];
            //
        }
    }





    // bye
    SDL_FreeSurface(optimized_surface);
    SDL_FreeSurface(render_Surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

