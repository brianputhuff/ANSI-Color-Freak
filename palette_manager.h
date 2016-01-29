#include <iostream>
#include <vector>
#include <stdint.h>
#include <math.h>
#include <cmath>
#include <SDL2/SDL.h>
#include "color.h"
#include "hsv.h"

class paletteManager
{
    public:
        paletteManager(void);
        ~paletteManager(void);
        Uint32 getANSIColor(Uint32 pixel_color);

    private:
        color* ANSIpalette[16];
        std::vector<int> selections;
        double getDeltaE2000(color color_1, color color_2);
        double radians(double degrees);
        double degrees(double radians);
        const double PI = 3.14159265359;

};

