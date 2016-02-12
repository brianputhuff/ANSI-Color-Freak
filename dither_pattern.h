#ifndef DITHER_PATTERN
#define DITHER_PATTERN

#include<iostream>
class DitherPattern
{
    public:
        DitherPattern(void);
        void nextRow(void);
        bool getBit(void);

    private:
        bool pattern[2];
        uint8_t itr;
        uint8_t row;
};

#endif
