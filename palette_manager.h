#include <iostream>
#include <stdint.h>
#include <math.h>
#include "SDL2/SDL.h"

class paletteManager
{
    public:
    paletteManager();
    ~paletteManager();
    SDL_Color getANSIColor(SDL_Color pixel_color);

    private:
        SDL_Color ANSIpalette[16];
};
