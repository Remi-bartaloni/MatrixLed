#include "image.h"

int Image::map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Image::Image(RGBMatrix *canvas) : canvas(canvas) {
    Color c(255, 255, 255);
    filtre = c;
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

void Image::setFiltre(Color value, bool redressage) {
    if(redressage == true) {
        uint8_t r = value.r, g=value.g, b = value.b;

        if(((r * 100) / 255) >= ((g * 100) / 255) && ((r * 100) / 255) >= ((b * 100) / 255)) {
            g = ((float)g / (float)r) * 255.0;
            b = ((float)b / (float)r) * 255.0;
            r = 255;
        }
        else if(((g * 100) / 255) >= ((b * 100) / 255) && ((g * 100) / 255) >= ((r * 100) / 255)) {
            r = ((float)r / (float)g) * 255.0;
            b = ((float)b / (float)g) * 255.0;
            g = 255;
        }
        else if(((b * 100) / 255) >= ((r * 100) / 255) && ((b * 100) / 255) >= ((g * 100) / 255)) {
            r = ((float)r / (float)b) * 255.0;
            b = ((float)b   / (float)b) * 255.0;
            b = 255;
        }

        filtre = Color(b, g, r);
    }
    else {
        filtre = value;
    }
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


/*
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
    */

    rotation += 180;
    if (rotation >= 360) {
        rotation = 0;
    }
    else if (rotation >= 0 && rotation < 90) {
        rotation = 0;
    }
    else if (rotation >= 90 && rotation < 180) {
        rotation = 1;
    }
    else if (rotation >= 180 && rotation < 270) {
        rotation = 2;
    }
    else if (rotation >= 270 && rotation < 360) {
        rotation = 3;
    }

    int widthRotated = width;
    int heightRotated = height;

    if (rotation == 1 || rotation == 3) {
        widthRotated = height;
        heightRotated = width;
    }

    for (int y = startY; y != heightRotated + startY; ++y) {
        for (int x = startX; x != widthRotated + startX; ++x) {
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
            tp += 3;

            int rotatedX = x;
            int rotatedY = y;

            if (rotation == 1) {
                // Rotation à 90 degrés
                rotatedX = width - 1 + startX - (y - startY);
                rotatedY = x - startX;
            } else if (rotation == 2) {
                // Rotation à 180 degrés
                rotatedX = width - 1 + startX - (x - startX);
                rotatedY = height - 1 + startY - (y - startY);
            } else if (rotation == 3) {
                // Rotation à 270 degrés
                rotatedX = y - startY;
                rotatedY = height - 1 + startY - (x - startX);
            }

            if (b + g + r != 0)     canvas->SetPixel(rotatedX, rotatedY, r, g, b);
        }
    }

    return 0;
}

Image::~Image() {
}
