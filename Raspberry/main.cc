#include "led-matrix.h"
#include "graphics.h"

#include <iostream>
#include <cstdio>
#include <cstring>

#include <unistd.h>

using namespace std;
using namespace rgb_matrix;

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

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

    for(int i = 0; i < argc; i++) {
        if(!strcmp(argv[i], "--identification_matrix"))      identification(runtime_opt, matrix_options);
    }

    return 0;
}
