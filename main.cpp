#include <iostream>
#include <stdint.h>
#include <math.h>
#include <iomanip>
#include <string>
#include "SDL2/SDL.h"

/*
    ANSI COLOR FREAK
    by Brian Puthuff
*/

enum { red, red_yellow, yellow, yellow_green, green, green_cyan, cyan, cyan_blue, blue, blue_magenta, magenta, magenta_red, mystery };

// HSV color structure
struct HSV
{
    int H;
    double S;
    double V;
};

/* function prototypes */

// get HSV color structure from RGB
HSV getHSV(Uint8 r, Uint8 g, Uint8 b);

// check if H value is within fixed palette range
int dropHueRange(int hue);
int getDeltaXY(const SDL_Point* pixel_pair, const SDL_Point* opt1_pair, const SDL_Point* opt2_pair);

// check if H value is within fixed palette range
//bool isWithinArcBounds(int hue, int index);

/* MAIN */
int main(int argc, char** argv)
{

    // temp window size for preview
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 640;

    // initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Could not initialize SDL. Program terminated.\n\n";
        return 1;
    }

    // attempt to load input image
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
    window = SDL_CreateWindow("ANSI Color Freak", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    render_Surface = SDL_GetWindowSurface(window);


    // optimize image surface
    SDL_Surface* optimized_surface = NULL;
    optimized_surface = SDL_ConvertSurface(input_image, render_Surface->format, 0);
    SDL_FreeSurface(input_image);


    // build ANSI palette
    SDL_Color colors[16];
    colors[0x0] = {0x00, 0x00, 0x00, 0xff}; // BLACK
    colors[0x1] = {0xb2, 0x18, 0x18, 0xff}; // DARK RED
    colors[0x2] = {0x18, 0xb2, 0x18, 0xff}; // DARK GREEN
    colors[0x3] = {0xb2, 0x68, 0x18, 0xff}; // DARK YELLOW
    colors[0x4] = {0x18, 0x18, 0xb2, 0xff}; // DARK BLUE
    colors[0x5] = {0xb2, 0x18, 0xb2, 0xff}; // DARK MAGENTA
    colors[0x6] = {0x18, 0xb2, 0xb2, 0xff}; // DARK CYAN
    colors[0x7] = {0xb2, 0xb2, 0xb2, 0xff}; // LIGHT GRAY
    colors[0x8] = {0x68, 0x68, 0x68, 0xff}; // DARK GRAY
    colors[0x9] = {0xff, 0x54, 0x54, 0xff}; // BRIGHT RED
    colors[0xa] = {0x54, 0xff, 0x54, 0xff}; // BRIGHT GREEN
    colors[0xb] = {0xff, 0xff, 0x54, 0xff}; // BRIGHT YELLOW
    colors[0xc] = {0x54, 0x54, 0xff, 0xff}; // BRIGHT BLUE
    colors[0xd] = {0xff, 0x54, 0xff, 0xff}; // BRIGHT MAGENTA
    colors[0xe] = {0x54, 0xff, 0xff, 0xff}; // BRIGHT CYAN
    colors[0xf] = {0xff, 0xff, 0xff, 0xff}; // WHITE

    // pointer to image data
    Uint32* pixels = (Uint32*) optimized_surface->pixels;

    // single pixel value
    Uint32 pixel;
    Uint8 r, g, b;

    // main loop
    for(int row = 0; row < optimized_surface->h; row++)
    {
        for(int col = 0; col < optimized_surface->w; col++)
        {
            // get pixel
            pixel = pixels[row * optimized_surface->w + col];

            // get pixel HSV
            SDL_GetRGB(pixel, optimized_surface->format, &r, &g, &b);
            HSV pixel_HSV = getHSV(r, g, b);

            if(pixel_HSV.S <= .20)
            { // if saturation is low, determine grays

                if(pixel_HSV.V >= .90)
                    pixel = SDL_MapRGB(optimized_surface->format, colors[0xf].r, colors[0xf].g, colors[0xf].b); // WHITE
                else if(pixel_HSV.V >= .52)
                    pixel = SDL_MapRGB(optimized_surface->format, colors[0x7].r, colors[0x7].g, colors[0x7].b); // LT GRAY
                else if(pixel_HSV.V >= .15)
                    pixel = SDL_MapRGB(optimized_surface->format, colors[0x8].r, colors[0x8].g, colors[0x8].b); // DK GRAY
                else
                    pixel = SDL_MapRGB(optimized_surface->format, colors[0x0].r, colors[0x0].g, colors[0x0].b); // BLACK
            }
            else
            { // enough saturation to determine color
                int selected_index = 0;
                int drop = dropHueRange(pixel_HSV.H);
                SDL_Point c;
                SDL_Point o1;
                SDL_Point o2;
                switch(drop)
                {
                    case red:
                        selected_index = 0x9; // RED
                        break;

                    case red_yellow:
                        c = {r, g};
                        o1 = {colors[0x9].r, colors[0x9].g}; // RED
                        o2 = {colors[0xb].r, colors[0xb].g}; // YELLOW
                        if(getDeltaXY(&c, &o1, &o2) == 0)
                            selected_index = 0x9; // RED
                        else
                            selected_index = 0xb; // YELLOW
                        break;

                    case yellow:
                        selected_index = 0xb; // YELLOW
                        break;

                    case yellow_green:
                        c = {r, g};
                        o1 = {colors[0xb].r, colors[0xb].g}; // YELLOW
                        o2 = {colors[0xa].r, colors[0xa].g}; // GREEN
                        if(getDeltaXY(&c, &o1, &o2) == 0)
                            selected_index = 0xb; // YELLOW
                        else
                            selected_index = 0xa; // GREEN
                        break;

                    case green:
                        selected_index = 0xa; // GREEN
                        break;

                    case green_cyan:
                        c = {g, b};
                        o1 = {colors[0xa].g, colors[0xa].b}; // GREEN
                        o2 = {colors[0xe].g, colors[0xe].b}; // CYAN
                        if(getDeltaXY(&c, &o1, &o2) == 0)
                            selected_index = 0xa; // GREEN
                        else
                            selected_index = 0xe; // CYAN
                        break;

                    case cyan:
                        selected_index = 0xe; // CYAN
                        break;

                    case cyan_blue:
                        c = {g, b};
                        o1 = {colors[0xe].g, colors[0xe].b}; // CYAN
                        o2 = {colors[0xc].g, colors[0xc].b}; // BLUE
                        if(getDeltaXY(&c, &o1, &o2) == 0)
                            selected_index = 0xe; // CYAN
                        else
                            selected_index = 0xc; // BLUE
                        break;

                    case blue:
                        selected_index = 0xc; // BLUE
                        break;

                    case blue_magenta:
                        c = {b, r};
                        o1 = {colors[0xc].b, colors[0xc].r}; // BLUE
                        o2 = {colors[0xd].b, colors[0xd].r}; // MAGENTA
                        if(getDeltaXY(&c, &o1, &o2) == 0)
                            selected_index = 0xc; // BLUE
                        else
                            selected_index = 0xd; // MAGENTA
                        break;

                    case magenta:
                        selected_index = 0xd; // MAGENTA
                        break;

                    case magenta_red:
                        c = {b, r};
                        o1 = {colors[0xd].b, colors[0xd].r}; // MAGENTA
                        o2 = {colors[0x9].b, colors[0x9].r}; // RED
                        if(getDeltaXY(&c, &o1, &o2) == 0)
                            selected_index = 0xd; // MAGENTA
                        else
                            selected_index = 0x9; // RED
                        break;

                    case mystery:
                    default:
                        selected_index = 0x0; // BLACK
                        break;
                }

                HSV selected_HSV = getHSV(colors[selected_index - 8].r, colors[selected_index - 8].g, colors[selected_index - 8].b);
                if(pixel_HSV.V <= selected_HSV.V)
                {
                    if(pixel_HSV.V < .15)
                        selected_index = 0x0;
                    else
                       selected_index -= 8;
                }
                pixel = SDL_MapRGB(optimized_surface->format, colors[selected_index].r, colors[selected_index].g, colors[selected_index].b);
            }
            // repaint pixel
            pixels[row * optimized_surface->w + col] = pixel;
        }
    }

    // render sample image to window
    SDL_BlitSurface(optimized_surface, NULL, render_Surface, NULL);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);

    // build output filename
    std::string filename = argv[1];
    filename.erase(filename.end()-4, filename.end());
    filename.append("_ACF.bmp");

    // save output file
    SDL_SaveBMP(optimized_surface, filename.c_str());

    // bye
    SDL_FreeSurface(optimized_surface);
    SDL_FreeSurface(render_Surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

HSV getHSV(Uint8 r, Uint8 g, Uint8 b)
{
    HSV r_HSV;
    double r_prime = (double) r / 255;
    double g_prime = (double) g / 255;
    double b_prime = (double) b / 255;

    double maximum = fmax(fmax(r_prime, g_prime), b_prime);
    double minimum = fmin(fmin(r_prime, g_prime), b_prime);
    double delta = maximum - minimum;

    // calculate value (V)
    r_HSV.V = maximum;

    // calculate saturation (S)
    if(delta == 0)
        r_HSV.S = 0;
    else
        r_HSV.S = delta / maximum;

    // calculate hue (H)
    if(delta == 0)
        r_HSV.H = 0;
    else if(r_prime == maximum)
        r_HSV.H = 60 * fmod(((g_prime - b_prime) / delta), 6);
    else if(g_prime == maximum)
        r_HSV.H = 60 * ((b_prime - r_prime) / delta + 2);
    else
        r_HSV.H = 60 * ((r_prime - g_prime) / delta + 4);
    return r_HSV;
}

int dropHueRange(int hue)
{
	if(hue < 15 || hue >= 345)
		return red;
	else if(hue < 45)
		return red_yellow;
	else if(hue < 75)
		return yellow;
	else if(hue < 105)
		return yellow_green;
	else if(hue < 135)
		return green;
	else if(hue < 165)
		return green_cyan;
	else if(hue < 195)
		return cyan;
	else if(hue < 225)
		return cyan_blue;
	else if(hue < 255)
		return blue;
	else if(hue < 285)
		return blue_magenta;
	else if(hue < 315)
		return magenta;
	else if(hue < 345)
		return magenta_red;
	else
		return mystery;
}

int getDeltaXY(const SDL_Point* pixel_pair, const SDL_Point* opt1_pair, const SDL_Point* opt2_pair)
{
    double delta_1, delta_2;
    delta_1 = sqrt(pow(pixel_pair->x - opt1_pair->x, 2) + pow(pixel_pair->y - opt1_pair->y, 2));
    delta_2 = sqrt(pow(pixel_pair->x - opt2_pair->x, 2) + pow(pixel_pair->y - opt2_pair->y, 2));
    if(delta_1 < delta_2)
        return 0;
    return 1;
}
/*
bool isWithinArcBounds(int hue, int index)
{
    switch(index)
    {
        case 0x9: // RED < 60 and  >
                if(hue < 45 || hue >= 315)
                    return true;
            break;
        case 0xa: // GREEN
                if(hue >=75 && hue < 165)
                    return true;
            break;
        case 0xb: // YELLOW
                if(hue >= 45 && hue < 75)
                    return true;
            break;
        case 0xc: // BLUE
                if(hue >= 195 && hue < 285)
                    return true;
            break;
        case 0xd: // MAGENTA
                if(hue >= 285 && hue < 315)
                    return true;
            break;
        case 0xe: // CYAN
                if(hue >= 165 && hue < 195)
                    return true;
            break;
        default:
            break;
    }
    return false;
}
*/
