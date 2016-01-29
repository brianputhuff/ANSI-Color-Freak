#include <iostream>
#include <stdint.h>
#include <math.h>

// HSV (hue | saturation | value) object definition
class hsv
{
    public:
        hsv(double r, double g, double b);
        double getHue(void);
        double getSaturation(void);
        double getValue(void);
        double getDelta(void);

    private:
        double H, S, V;
        double c_max, c_min, delta;
        double findMax(double x, double y, double z);
        double findMin(double x, double y, double z);
};
