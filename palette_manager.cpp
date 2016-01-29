#include "palette_manager.h"

paletteManager::paletteManager(void)
{
    // initialize palette
    ANSIpalette[0x0] = new color( 0xff000000 ); // BLACK
    ANSIpalette[0x1] = new color( 0xffb21818 ); // RED
    ANSIpalette[0x2] = new color( 0xff18b218 ); // GREEN
    ANSIpalette[0x3] = new color( 0xffb26818 ); // DARK YELLOW
    ANSIpalette[0x4] = new color( 0xff1818b2 ); // BLUE
    ANSIpalette[0x5] = new color( 0xffb218b2 ); // MAGENTA
    ANSIpalette[0x6] = new color( 0xff18b2b2 ); // CYAN
    ANSIpalette[0x7] = new color( 0xffb2b2b2 ); // LIGHT GRAY

    ANSIpalette[0x8] = new color( 0xff686868 ); // DARK GRAY
    ANSIpalette[0x9] = new color( 0xffff5454 ); // LIGHT RED
    ANSIpalette[0xa] = new color( 0xff54ff54 ); // LIGHT GREEN
    ANSIpalette[0xb] = new color( 0xffffff54 ); // LIGHT YELLOW
    ANSIpalette[0xc] = new color( 0xff5454ff ); // LIGHT BLUE
    ANSIpalette[0xd] = new color( 0xffff54ff ); // LIGHT MAGENTA
    ANSIpalette[0xe] = new color( 0xff54ffff ); // LIGHT CYAN
    ANSIpalette[0xf] = new color( 0xffffffff ); // WHITE
}

paletteManager::~paletteManager(void)
{

}

Uint32 paletteManager::getANSIColor(Uint32 pixel_color)
{
    color sample = color(pixel_color);

    //calculate DeltaE 2000
    double lowestDelta = 10000;
    uint8_t best_index = 0;
    double deltaE00;
    for(uint8_t i = 0; i < 16; i++)
    {
            /*
            double Kl = 1;
            double Kc = 1;
            double Kh = 1;
            double K1 = 0.045; // graphic arts
            double K2 = 0.015; // graphic arts

            double C1 = sqrt(pow(sample.getCIE_a(), 2) + pow(sample.getCIE_b(), 2));
            double C2 = sqrt(pow(ANSIpalette[i]->getCIE_a(), 2) + pow(ANSIpalette[i]->getCIE_b(), 2));
            double delta_Cab = C1 - C2;

            double delta_a = sample.getCIE_a() - ANSIpalette[i]->getCIE_a();
            double delta_b = sample.getCIE_b() - ANSIpalette[i]->getCIE_b();
            double delta_L = sample.getCIE_L() - ANSIpalette[i]->getCIE_L();



            double Sl = 1;
            double Sc = 1 + K1 * C1;
            double Sh = 1 + K2 * C1;

            double delta_Hab = sqrt(pow(delta_a, 2) + pow(delta_b, 2) - pow(delta_Cab, 2));



            double a = delta_L / (Kl * Sl);
            double b = delta_Cab / (Kc * Sc);
            double c = delta_Hab / (Kh * Sh);

            double deltaE94 = sqrt(pow(a, 2) + pow(b, 2) + pow(c, 2));
            */
            //deltaE00 = getDeltaE2000(sample, *ANSIpalette[i]);

            double delta_R = sample.getR() - ANSIpalette[i]->getR();
            double delta_G = sample.getG() - ANSIpalette[i]->getG();
            double delta_B = sample.getB() - ANSIpalette[i]->getB();
            double A = sqrt(pow(delta_R, 2) + pow(delta_G, 2));
            double B = sqrt(pow(delta_R, 2) + pow(delta_B, 2));
            double C = sqrt(pow(delta_G, 2) + pow(delta_B, 2));

            deltaE00 = (A + B + C) / 3;

            if(deltaE00 <= lowestDelta)
            {
                lowestDelta = deltaE00;
                best_index = i;
            }
        }
    return ANSIpalette[best_index]->getColor();

}

double paletteManager::getDeltaE2000(color color_1, color color_2)
{
    // delta E variables
    double C1, C2, C_bar, C_prime_1, C_prime_2;
    double delta_L_prime, delta_C_prime, delta_h_prime, delta_H_prime;
    double a_prime_1, a_prime_2;
    double h_prime_1, h_prime_2;
    double L_prime_bar, C_prime_bar, h_prime_bar;
    double G, T, delta_theta, delta_E2K;
    double RC, RT;
    double SL, SC, SH;
    double KL = 1;
    double KC = 1;
    double KH = 1;

    // First...
    C1 = sqrt((pow(color_1.getCIE_a(), 2)) + (pow(color_1.getCIE_b(), 2)));
    C2 = sqrt((pow(color_2.getCIE_a(), 2)) + (pow(color_2.getCIE_b(), 2)));
    C_bar = (C1 + C2) / 2;

    G = 0.5 * (1 - sqrt(pow(C_bar, 7) / (pow(C_bar, 7) + pow(25, 7))));

    a_prime_1 = color_1.getCIE_a() * (1 + G);
    a_prime_2 = color_2.getCIE_a() * (1 + G);

    C_prime_1 = sqrt((pow(a_prime_1, 2)) + (pow(color_1.getCIE_b(), 2)));
    C_prime_2 = sqrt((pow(a_prime_2, 2)) + (pow(color_2.getCIE_b(), 2)));;

    if((color_1.getCIE_b() == a_prime_1) && (a_prime_1 == 0))
        h_prime_1 = 0;
    else
        h_prime_1 = degrees(atan2(color_1.getCIE_b(), a_prime_1));

    if((color_2.getCIE_b() == a_prime_2) && (a_prime_2 == 0))
        h_prime_2 = 0;
    else
        h_prime_2 = degrees(atan2(color_2.getCIE_b(), a_prime_2));

    // Second///
    delta_L_prime = color_2.getCIE_L() - color_1.getCIE_L();
    delta_C_prime = C_prime_2 - C_prime_1;

    if(C_prime_1 * C_prime_2 == 0)
        delta_h_prime = 0;
    else
    {
        if(std::abs(h_prime_2 - h_prime_1) <= 180)
            delta_h_prime = h_prime_2 - h_prime_1;
        else if((h_prime_2 - h_prime_1) > 180)
            delta_h_prime = (h_prime_2 - h_prime_1) - 360;
        else if((h_prime_2 - h_prime_1) < -180)
             delta_h_prime = (h_prime_2 - h_prime_1) + 360;
        else;
    }

    delta_H_prime = 2 * sqrt(C_prime_1 *C_prime_2) * degrees(sin(radians(delta_h_prime / 2)));

    //Finally...
    L_prime_bar = (color_1.getCIE_L() + color_2.getCIE_L()) / 2;
    C_prime_bar = (C_prime_1 + C_prime_2) / 2;

    if((C_prime_1 * C_prime_2) == 0)
        h_prime_bar = (h_prime_1 + h_prime_2);
    else
    { // (C_prime_1 * C_prime_2) != 0
        if(std::abs(h_prime_1 - h_prime_2) <= 180)
                h_prime_bar = (h_prime_1 + h_prime_2) / 2;
        else
        { // (std::abs(h_prime_1 - h_prime_2) >180)
            if((h_prime_1 + h_prime_2) < 360)
                h_prime_bar = (h_prime_1 + h_prime_2 + 360) / 2;
            else
                h_prime_bar = (h_prime_1 + h_prime_2 - 360) / 2;
        }
    }

    T = 1;
    T-= (0.17 * degrees(cos(radians(h_prime_bar - 30))));
    T+= (0.24 * degrees(cos(radians(2 * h_prime_bar))));
    T+= (0.32 * degrees(cos(radians(3 * h_prime_bar + 6))));
    T-= (0.20 * degrees(cos(radians(4 * h_prime_bar - 63))));

    delta_theta = 30 * exp(-1 * pow(std::abs((h_prime_bar - 275) / 25), 2));

    RC = 2 * sqrt(pow(C_prime_bar, 7) / (pow(C_prime_bar, 7) + pow(25, 7)));
    SL = 1 + ((0.015 * pow(L_prime_bar - 50, 2)) / (sqrt(20 + pow(L_prime_bar - 50, 2))));
    SC = 1 + 0.045 * C_prime_bar;
    SH = 1 + 0.015 * C_prime_bar * T;
    RT = -1 * degrees(sin(radians(2 * delta_theta))) * RC;

    delta_E2K = sqrt(pow(delta_L_prime / KL * SL, 2) + pow(delta_C_prime / KC * SC, 2) + pow(delta_H_prime / KH * SH, 2) + (RT * (delta_C_prime / KC * SC) * (delta_H_prime / KH * SH)));

    return delta_E2K;

}

double paletteManager::radians(double degrees)
{
    return degrees * PI / 180;
}

double paletteManager::degrees(double radians)
{
    return radians * 180 / PI;
}

    // std::cout << "Lab: (" << sample.getCIE_L() << ' ' << sample.getCIE_a() << ' ' << sample.getCIE_b() <<
/*













    double lowest_delta = 300;
    double distance;
    uint8_t best_index = 0;

    double deltaR = 0;
    double deltaG = 0;
    double deltaB = 0;


    for(uint8_t i = 0; i < 16; i++)
    {
        deltaR = abs(sample.getR() - ANSIpalette[i]->getR());
        deltaG = abs(sample.getG() - ANSIpalette[i]->getG());
        deltaB = abs(sample.getB() - ANSIpalette[i]->getB());
        double d_avg = (deltaR + deltaG + deltaB) / 3;
        if(d_avg < lowest_delta)
        {
            lowest_delta = deltaG;
            best_index = i;
        }
    }
*/

    /*
    // check for grayscale
    color sample = color(pixel_color, UNDEFINED);
    if(sample.getDelta() < .10)
    { // GRAYSCALE
        if(sample.getValue() < .25)
            return ANSIpalette[0x0]->getColor();
        else if(sample.getValue() < .50)
            return ANSIpalette[0x8]->getColor();
        else if(sample.getValue() < .75)
            return ANSIpalette[0x7]->getColor();
        else
            return ANSIpalette[0xf]->getColor();
    }


    for(int i = 0; i < 16; i++)
    {
        if(ANSIpalette[i]->getType() == NORMAL)
        {
            if(ANSIpalette[i]->isWithinRange(sample.getHue()) == true)
            {
                selections.push_back(i);
            }
        }
    }

    double lowest_distance = 1000;
    double distance;
    Uint32 color_value;
    for(int i = 0; i < selections.size(); i++)
    {
        distance = sqrt((pow(sample.getSaturation() - ANSIpalette[selections[i]]->getSaturation(), 2)) + (pow(sample.getValue() - ANSIpalette[selections[i]]->getValue(), 2)));
        if(distance < lowest_distance)
        {
            lowest_distance = distance;
            color_value = ANSIpalette[selections[i]]->getColor();
        }
    }

    selections.clear();
    return color_value;
*/
