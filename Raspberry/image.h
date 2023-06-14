#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <cmath>

#include "led-matrix.h"
#include "graphics.h"

using namespace std;
using namespace rgb_matrix;

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

class Image
{
    RGBMatrix *canvas;
    const char *chemin;
    int bruit = 15;
    Color filtre;
    int rotation = 0;

    int map(int, int, int, int, int);

    public :
        Image(RGBMatrix*);
        Image(RGBMatrix*, const char*);

        void setChemin(const char*);
        void setBruit(int);
        void setRotation(int);
        void setFiltre(Color, bool redressage = false);

        int printImage(int, int);

        ~Image();
};

#endif // IMAGE_H
