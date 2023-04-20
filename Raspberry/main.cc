#include "led-matrix.h"
#include "graphics.h"

#include <iostream>
#include <cstdio>
#include <cstring>

#include <unistd.h>
#include <math.h>

using namespace std;
using namespace rgb_matrix;

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int identification(rgb_matrix::RuntimeOptions runtime_opt, RGBMatrix::Options matrix_options) {
    rgb_matrix::Font font;
    Color color(255, 255, 255);

    const char *bdf_font_file = strdup("fonts/10x20.bdf");
    if (!font.LoadFont(bdf_font_file)) {
		fprintf(stderr, "Couldn't load font '%s'\n", bdf_font_file);
		return 1;
	}

    RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
	if (canvas == NULL) return 1;

    for (int i = 0; i < matrix_options.chain_length; i++) {
        DrawText(canvas, font, matrix_options.rows * i , font.height(), color, to_string(i+1).c_str());
    }

    sleep(50);
    delete canvas;
    delete bdf_font_file;
    return 0;
}

int circleCPU(rgb_matrix::RuntimeOptions runtime_opt, RGBMatrix::Options matrix_options, int x, int y, Color color) {
    int min, max;
    float randomme;
    float distance;
    int rayon;
    int pointAnimationX;
    int pointAnimationY;

    int r = (int)color.b;
    int g = (int)color.g;
    int b = (int)color.r;

    if(matrix_options.rows > matrix_options.cols) {
        min = matrix_options.cols;
        max = matrix_options.rows;
    }
    else {
        min = matrix_options.rows;
        max = matrix_options.cols;
    }

    rayon = (min/3);

    RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
	if (canvas == NULL) return 1;

    for(int i = 0; i < rayon; i++)
        DrawCircle(canvas, x, y, i, color);

    for(int i = 0; i < rayon; i++)
        DrawCircle(canvas, x*3, y, i, color);

    int i = 0;
    while (i < (max*5)) {
        pointAnimationX = rand() % matrix_options.rows*(matrix_options.chain_length-1);
        pointAnimationY = rand() % matrix_options.cols;
        distance = sqrt(pow(pointAnimationX - x, 2) + pow(pointAnimationY - y, 2));
        if (distance <= rayon) {
            randomme = rand() % 100;
            r = (float)color.b * (randomme/100);
            g = (float)color.g * (randomme/100);
            b = (float)color.r * (randomme/100);

            canvas->SetPixel(pointAnimationX, pointAnimationY, r, g, b);
            i++;
        }
    }

    cout << x*3 << endl;
    cout << y << endl;

    i = 0;
    while (i < (max*2)) {
        pointAnimationX = rand() % matrix_options.rows + (matrix_options.rows*(matrix_options.chain_length-1));
        pointAnimationY = rand() % matrix_options.rows;
        distance = sqrt(pow(pointAnimationX - x*3, 2) + pow(pointAnimationY - y, 2));
        if (distance <= rayon) {
            randomme = rand() % 100;
            r = (float)color.b * (randomme/100);
            g = (float)color.g * (randomme/100);
            b = (float)color.r * (randomme/100);

            canvas->SetPixel(pointAnimationX, pointAnimationY, r, g, b);
            i++;
        }
    }

    sleep(50);
    delete canvas;
    return 0;
}

int main(int argc, char **argv)
{
    RGBMatrix::Options matrix_options;
	matrix_options.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
	matrix_options.rows = 64;
	matrix_options.cols = 64;
	matrix_options.chain_length = 3;
	matrix_options.parallel = 1;
	matrix_options.show_refresh_rate = true;

    rgb_matrix::RuntimeOptions runtime_opt;
    Color colorCPU(0, 0, 255);    //BGR

    for(int i = 0; i < argc; i++) {
        if(!strcmp(argv[i], "--identification_matrix"))      identification(runtime_opt, matrix_options);
    }

    circleCPU(runtime_opt, matrix_options, 64, 0, colorCPU);

    return 0;
}
