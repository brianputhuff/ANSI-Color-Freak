#include "hsv.h"

hsv::hsv(double r, double g, double b)
{

    c_max = findMax(r, g, b);
    c_min = findMin(r, g, b);

    delta = c_max - c_min;

    // Hue
    if(delta == 0)
        H = 0;
    else if(c_max == r)
        H = 60 * fmod(((g - b) / delta), 6);
    else if(c_max == g)
        H = 60 * (((b - r) / delta) + 2);
    else
        H = 60 * (((r - g) / delta) + 4);

    if(H < 0)
        H += 360;

    // Saturation
    if(c_max == 0)
        S = 0;
    else
        S = delta / c_max;

    // Value
    V = c_max;

}

double hsv::getDelta(void)
{
    return delta;
}

double hsv::getHue(void)
{
    return H;
}

double hsv::getSaturation(void)
{
    return S;
}

double hsv::getValue(void)
{
    return V;
}

double hsv::findMax(double x, double y, double z)
{
    double m = fmax(x, y);
    return fmax(m, z);
}

double hsv::findMin(double x, double y, double z)
{
    double m = fmin(x, y);
    return fmin(m, z);
}
