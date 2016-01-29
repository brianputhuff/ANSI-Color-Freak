#include<iostream>
#include<SDL2/SDL.h>


//enum { GRAYSCALE, NORMAL, UNDEFINED };
class color
{
    public:
        color(Uint32 argb_color);
        Uint32 getColor(void);
        double getR(void);
        double getG(void);
        double getB(void);
        double getCIE_L(void);
        double getCIE_a(void);
        double getCIE_b(void);
        //double getDeltaE(void);

/*
        double getCIE_L(void);
        double getCIE_a(void);
        double getCIE_b(void);
        double getHue(void);
        double getSaturation(void);
        double getValue(void);
        bool isWithinRange(int h);

        uint8_t getType(void);

        */

    private:
        Uint32 argb_color;
        double r;
        double g;
        double b;
        double CIE_L;
        double CIE_a;
        double CIE_b;

/*
        double hue;
        double sat;
        double val;
        double delta;
        double c_max;
        double c_min;
        int h_max;
        int h_min;
        //uint8_t escape_code;
        //uint8_t type;
*/


};
