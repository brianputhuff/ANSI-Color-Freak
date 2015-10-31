#include "palette_manager.h"

paletteManager::paletteManager(void)
{
    // initialize palette
    ANSIpalette[0x0] = {255, 0, 0, 0};
    ANSIpalette[0x1] = {255, 0, 0, 0};
    ANSIpalette[0x2] = {255, 0, 0, 0};
    ANSIpalette[0x3] = {255, 0, 0, 0};
    ANSIpalette[0x4] = {255, 0, 0, 0};
    ANSIpalette[0x5] = {255, 0, 0, 0};
    ANSIpalette[0x6] = {255, 0, 0, 0};
    ANSIpalette[0x7] = {255, 0, 0, 0};

    ANSIpalette[0x8] = {255, 0, 0, 0};
    ANSIpalette[0x9] = {255, 0, 0, 0};
    ANSIpalette[0xa] = {255, 0, 0, 0};
    ANSIpalette[0xb] = {255, 0, 0, 0};
    ANSIpalette[0xc] = {255, 0, 0, 0};
    ANSIpalette[0xd] = {255, 0, 0, 0};
    ANSIpalette[0xe] = {255, 0, 0, 0};
    ANSIpalette[0xf] = {255, 0, 0, 0};
}

paletteManager::~paletteManager(void)
{
    delete ANSIpalette;
}

SDL_Color paletteManager::getANSIColor(SDL_Color pixel_color)
{
    uint8_t mean_delta = 255;
    uint8_t new_delta = mean_delta;
    uint8_t replacement_color_index;

    for(uint8_t i = 0; i < 16; i++)
    {
        new_delta = sqrt(
                        pow(ANSIpalette[i].r - pixel_color.r, 2) +
                        pow(ANSIpalette[i].g - pixel_color.g, 2) +
                        pow(ANSIpalette[i].b - pixel_color.b, 2)
                    );
        if(new_delta <= mean_delta)
        {
            mean_delta = new_delta;
            replacement_color_index = i;
        }
    }

    return ANSIpalette[replacement_color_index];
}
