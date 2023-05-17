#include "main.h"

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
    float distance2;
    float distance3;
    int rayon;
    int pointAnimationX;
    int pointAnimationX2;
    int pointAnimationX3;
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

    rayon = (min/9)*4;

    RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
	if (canvas == NULL) return 1;

    for(int i = 0; i < rayon; i++) {
        DrawCircle(canvas, x, y, i, color);
        DrawCircle(canvas, x*3, y, i, color);
    }

    int i = 0;

    while(1) {
        pointAnimationX = rand() % matrix_options.rows*(matrix_options.chain_length-1);
        pointAnimationX2 = pointAnimationX * (rayon/2);
        pointAnimationX3 = rand() % matrix_options.rows + (matrix_options.rows*(matrix_options.chain_length-1));
        pointAnimationY = rand() % matrix_options.cols;
        distance = sqrt(pow(pointAnimationX - x, 2) + pow(pointAnimationY - y, 2));
        distance2 = sqrt(pow(pointAnimationX2 - x, 2) + pow(pointAnimationY - y, 2));
        distance3 = sqrt(pow(pointAnimationX3 - x*3, 2) + pow(pointAnimationY - y, 2));

        randomme = rand() % 100;
        r = (float)color.b * (randomme/100);
        g = (float)color.g * (randomme/100);
        b = (float)color.r * (randomme/100);

        if (distance <= rayon) {
            canvas->SetPixel(pointAnimationX, pointAnimationY, b, g, r);
        }

        if (distance2 <= rayon) {
            canvas->SetPixel(pointAnimationX2, pointAnimationY, b, g, r);
        }

        if (distance3 <= rayon) {
            canvas->SetPixel(pointAnimationX3, pointAnimationY, b, g, r);
        }
        usleep(500);
    }
    /*
    i = 0;
    while (i < (max*5)) {
        pointAnimationX = rand() % matrix_options.rows*(matrix_options.chain_length-1);
        pointAnimationY = rand() % matrix_options.cols;
        distance = sqrt(pow(pointAnimationX - x, 2) + pow(pointAnimationY - y, 2));
        if (distance <= rayon) {
            randomme = rand() % 100;
            r = (float)color.b * (randomme/100);
            g = (float)color.g * (randomme/100);
            b = (float)color.r * (randomme/100);

            canvas->SetPixel(pointAnimationX, pointAnimationY, b, g, r);
            i++;
        }
    }

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

            canvas->SetPixel(pointAnimationX, pointAnimationY, b, g, r);
            i++;
        }
    }
    */

    sleep(50);
    delete canvas;
    return 0;
}

int gifAfficher(RuntimeOptions runtime_opt, RGBMatrix::Options matrix_options, RGBMatrix *canvas, const char *chemin, int startX, int startxY) {
    int width, height, size;
    unsigned char info[54];
    unsigned char* data;

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

    int tp = 0;
    for(int x = width-1 + startX; x != -1 + startX; --x) {
        for (int y = startxY; y != height + startxY; ++y)
        {
            int b = (int)data[tp];
            int g = (int)data[tp + 1];
            int r = (int)data[tp + 2];
            tp += 3;
            if(b + g + r != 0)            canvas->SetPixel(y, x, b, g, r);
        }
    }

    sleep(500);
    //canvas->Clear();

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
    Color colorCPU(0, 255, 0);    //BGR
    Color colorFont(0, 0, 255);   //BGR

    for(int i = 0; i < argc; i++) {
        if(!strcmp(argv[i], "--identification_matrix"))      identification(runtime_opt, matrix_options);
    }

    RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
	if (canvas == NULL) {
        cerr << "canvas not" << endl;
        return 1;
    }

    //circleCPU(runtime_opt, matrix_options, 64, 0, colorCPU);

    //gifAfficher(runtime_opt, matrix_options, canvas, "./img/64Cpu/cpu (1).bmp", 0, 0);

    Image *img = new Image(canvas, "./img/cpu/cpu (1).bmp");
    img->setBruit(18);
    img->setFiltre(colorCPU);
    //img->setRotation(0);
    //img->printImage(0, 0);



    Image *img2 = new Image(canvas, "./img/64Cpu/cpu (1).bmp");
    img2->setBruit(18);
    img2->setFiltre(colorFont);
    //img2->setRotation(90);
    /*img2->printImage(0, -64);
    img2->printImage(128, -64);*/


    //img->printImage(-64, 128);
    //img2->printImage(-64, 128);


    int cpuInt = 1;
    int gpuInt = 1;
    string s;
    while (1) {
        canvas->Clear();

        s = "./img/gpu/gpu (" + to_string(gpuInt) + ").bmp";
        img2->setChemin(s.c_str());
        img2->setRotation(0);
        img2->printImage(-64, 0);
        img2->setRotation(180);
        img2->printImage(-64, 128);

        //usleep(500);

        s = "./img/cpu/cpu (" + to_string(cpuInt) + ").bmp";
        img->setChemin(s.c_str());
        img->setRotation(0);
        img->printImage(-64, 0);
        img->setRotation(180);
        img->printImage(-64, 128);

        if (cpuInt >= 90) cpuInt = 1;
        else cpuInt++;

        if (gpuInt >= 50) gpuInt = 1;
        else gpuInt++;

        usleep(300000);
    }


    /*
    string s;
    while (1) {
        for (int i = 1; i < 44; i++) {
            s = "./img/64font/font (" + to_string(i) + ").bmp";
            img->setChemin(s.c_str());
            img->printImage(0, 0);
            usleep(100000);
            canvas->Clear();
        }
    }
    */

    /*
    canvas->SetPixel(0, 0, 0, 0, 255);
    canvas->SetPixel(63, 0, 0, 255, 0);
    canvas->SetPixel(0, 63, 255, 0, 0);
    canvas->SetPixel(63, 63, 255, 255, 255);*/


    sleep(30);
    delete img;
    delete canvas;
    return 0;
}
