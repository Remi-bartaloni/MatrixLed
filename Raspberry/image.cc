#include "image.h"

int Image::map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Image::Image(RGBMatrix *canvas, const char *chemin) : canvas(canvas), chemin(chemin) {
    Color c(255, 255, 255);
    filtre = c;
}

void Image::setChemin(const char *value) {
    chemin = value;
}

void Image::setBruit(int value) {
    bruit = value;
}

void Image::setFiltre(Color value) {
    filtre = value;
}

void Image::setRotation(int value) {
    rotation = value;
}

int Image::printImage(int startX, int startY) {
    int width, height, size;
    unsigned char info[54];
    unsigned char* data;
    int tp = 0;
    int r, g, b;

    int x, y, finX, finY;

    FILE* file = fopen(chemin, "rb");
    if (file == NULL) {
        cerr << "FILE not" << endl;
        return 1;
    }

    fread(info, sizeof(unsigned char), 54, file);
    width = *(int*)&info[18];
    height = *(int*)&info[22];
    size = 3 * width * height;

    data = new unsigned char[size];
    fread(data, sizeof(unsigned char), size, file);
    fclose(file);

    rotation = rotation + 90;
    if (rotation == 360) {
        rotation = 0;
    }

    if(rotation == 0) {
        x = width + startX - 1;
        finY = startY;
        finX = startX;
        y = height + startY - 1;
    }
    else if (rotation == 90) {
        x = startX;
        finY = startY;
        finX = width + startX - 1;
        y = height + startY - 1;
    }
    else if (rotation == 180) {
        x = startX;
        finY = height + startY;
        finX = width + startX;
        y = startY;
    }
    else if (rotation == 270) {
        x = width - 1 + startX;
        finY = height + startY;
        finX = startX;
        y = startY;
    }
    else {
        x = startX;
        finY = startY;
        finX = width + startX - 1;
        y = height + startY - 1;
        rotation = 90;
    }

    while (x != finX) {
        while (y != finY) {
            r = (int)data[tp];
            g = (int)data[tp + 1];
            b = (int)data[tp + 2];

            if(r < bruit && g < bruit && b < bruit) {
                r = 0;
                g = 0;
                b = 0;
            }

            r = map(r, 0, 255, 0, (int)filtre.r);
            g = map(g, 0, 255, 0, (int)filtre.g);
            b = map(b, 0, 255, 0, (int)filtre.b);

            if(r + g + b != 0) {
                if(rotation == 0 || rotation == 180) canvas->SetPixel(x, y, r, g, b);
                else if(rotation == 90 || rotation == 270) canvas->SetPixel(y, x, r, g, b);
            }
            tp += 3;
            if(rotation == 0 || rotation == 90) --y;
            else if (rotation == 180 || rotation == 270) ++y;
        }
        if(rotation == 0) {
            y = height + startY;
            --x;
        }
        else if (rotation == 90) {
            y = height + startY;
            ++x;
        }
        else if (rotation == 180) {
            y = startY;
            ++x;
        }
        else if (rotation == 270) {
            y = startY;
            --x;
        }
    }
    return 0;
}

Image::~Image() {
}
