#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <iterator>
#include <cstdio>
#include <cmath>
#include <thread>
#include <chrono>

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <semaphore.h>

#include "led-matrix.h"
#include "graphics.h"
#include "image.h"
#include "meteo.h"

using namespace std;
using namespace rgb_matrix;

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;
