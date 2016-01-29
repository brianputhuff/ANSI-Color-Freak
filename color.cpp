#include "color.h"
#include <iomanip>

color::color(Uint32 argb_color)
{
    this->argb_color = argb_color;
    //this->type = type;

    // extract rgb
    Uint32 c = argb_color;
    b = (c & 255);
    c >>= 8;

    g = (c & 255);

    c >>= 8;
    r = (c & 255);

    // convert to one scale
    double cR, cG, cB;
    cR = r / (double) 0xff;
    cG = g / (double) 0xff;
    cB = b / (double) 0xff;

    // convert to XYZ
    if(cR > 0.04045)
        cR = pow(((cR + 0.055) / 1.055), 2.4);
    else
        cR /= 12.92;

    if(cG > 0.04045)
        cG = pow(((cG + 0.055) / 1.055), 2.4);
    else
        cG /= 12.92;

    if(cB > 0.04045)
        cB = pow(((cB + 0.055) / 1.055), 2.4);
    else
        cB /= 12.92;

    cR *= 100;
    cG *= 100;
    cB *= 100;

    double X = (cR * 0.4124) + (cG * 0.3576) + (cB * 0.1805);
    double Y = (cR * 0.2126) + (cG * 0.7152) + (cB * 0.0722);
    double Z = (cR * 0.0193) + (cG * 0.1192) + (cB * 0.9505);

    // convert XYZ to CIE-L*ab
    X /= 95.047;
    Y /= 100.000;
    Z /= 108.883;

    if(X > 0.008856)
        X = cbrt(X);
    else
        X = (7.787 * X) + (double) (16 / 116);

    if(Y > 0.008856)
        Y = cbrt(Y);
    else
        Y = (7.787 * Y) + (double) (16 / 116);

    if(Z > 0.008856)
        Z = cbrt(Z);
    else
        Z = (7.787 * Z) + (double) (16 / 116);



    CIE_L = (116 * Y) - 16;
    CIE_a = 500 * (X - Y);
    CIE_b = 200 * (Y - Z);
}
    /*
    uint8_t grab;
    grab = (c & 0x000000ff);
    b = (double) grab / (double) 0xff;

    c >>= 8;
    grab = (c & 0x000000ff);
    g = (double) grab / (double) 0xff;

    c >>= 8;
    grab = (c & 0x000000ff);
    r = (double) grab / (double) 0xff;

    // convert to 0 - 1 scale
    //r /= (double) 0xff;
    //g /= (double) 0xff;
    //b /= (double) 0xff;



}

    c_max = fmax(fmax(r, g), b);
    c_min = fmin(fmin(r, g), b);
    delta = c_max - c_min;

    // determine hue
    if(delta == 0)
        hue = 0;
    else if(c_max == r)
        hue = 60 * fmod(((g - b) / delta), 6);
    else if(c_max == g)
        hue = 60 * (((b - r) / delta) + 2);
    else
        hue = 60 * (((r - g) / delta) + 4);

    // correct negative results
    if(hue < 0)
        hue = 360 + hue;
        // determine saturation
    if(c_max == 0)
        sat = 0;
    else
        sat = delta / c_max;

        // determine value
    val = c_max;

    h_max = hue + 30;
    if(h_max > 359)
        h_max -= 360;
    h_min = hue - 30;
    if(h_min < 0)
        h_min += 360;
*/


Uint32 color::getColor(void)
{
    return argb_color;
}

double color::getR(void)
{
    return r;
}

double color::getG(void)
{
    return g;
}

double color::getB(void)
{
    return b;
}

double color::getCIE_L(void)
{
    return CIE_L;
}

double color::getCIE_a(void)
{
    return CIE_a;
}

double color::getCIE_b(void)
{
    return CIE_b;
}

/*
bool color::isWithinRange(int h)
{
    if(h > h_min && h < h_max)
        return true;
    return false;
}

double color::getHue(void)
{
    return hue;
}

double color::getSaturation(void)
{
    return sat;
}

double color::getValue(void)
{
    return val;
}

double color::getDelta(void)
{
    return delta;
}

uint8_t color::getType(void)
{
    return type;
}
*/

/*
    color32 = c;
    escape_number = esc_num;
    group = type;

    // Get RGB
    double r = getR(color32);
    double g = getG(color32);
    double b = getB(color32);

    c_max = findMax(r, g, b);
    c_min = findMin(r, g, b);

    delta = c_max - c_min;

    // Hue
    if(delta == 0)
        H = 0;
    else if(c_max == rP)
        H = 60 * ((uint16_t)((g - b) / delta) % 6);
    else if(c_max == gP)
        H = 60 * ((uint16_t)((b - r) / delta) + 2);
    else
        H = 60 * ((uint16_t)((r - g) / delta) + 4);

    // Saturation
    if(c_max == 0)
        S = 0;
    else
        S = delta / c_max;

    // Value
    V = c_max;
}

Uint32 color::getColor(void)
{
    return color32;
}

uint8_t color::getDelta(void)
{
    return delta;
}

uint8_t color::getHMin(void)
{
    return h_min;
}

uint8_t color::getHMax(void)
{
    return h_max;
}

double color::getHue(void)
{
    return H;
}

double color::getSaturation(void)
{
    return S;
}

double color::getValue(void)
{
    return V;
}

double color::findCMax(double x, double y, double z)
{
    double m;
    if(x > y)
        m = x;
    else
        m = y;

    if(m < z)
        m = z;

    return m;
}

double color::findCMin(double x, double y, double z)
{
    double m;
    if(x < y)
        m = x;
    else
        m = y;

    if(m > z)
        m = z;

    return m;
}

uint8_t color::getR(Uint32 c)
{
    c = (color & 0x00ff0000);
    c >>= 16;
    return (uint8_t) color;
}

uint8_t color::getG(Uint32 c)
{
    c = (color & 0x0000ff00);
    c >>= 8;
    return (uint8_t) color;
}

uint8_t color::getB(Uint32 c)
{
    c = (color & 0x000000ff);
    return (uint8_t) color;
}
*/
