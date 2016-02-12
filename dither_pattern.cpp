#include "dither_pattern.h"

DitherPattern::DitherPattern(void)
{
    pattern[0] = false;
    pattern[1] = true;
    row = 0; // even row
    itr = 0; // normal direction
};

void DitherPattern::nextRow(void)
{
    if(row == 0)
    {
        row = 1;
        itr = 1;
    }
    else
    {
        row = 0;
        itr = 0;
    }
};

bool DitherPattern::getBit(void)
{
    //
    bool rbit = pattern[itr];
    if(row == 0)
    {
        // set to next bit
        if(itr < 1)
            itr++;
        else
            itr = 0;
    }
    else
    {
        if(itr < 1)
            itr = 1;
        else
            itr--;
    }
    return rbit;
};

