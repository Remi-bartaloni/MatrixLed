#ifndef METEO_H
#define METEO_H

#include <string>
#include "client_tcp.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#include <iostream>
#include <sstream>
#include <thread>
#include <ctime>

#include<json-c/json.h>

using namespace std;

class Meto
{
	Client_tcp *client;

	string tram;

    public:
        Meto(string&, string&);

        string get_tram();

        void parsed();

        string get_temp(int);

        string get_humidity(int);

        string get_icon(int);

        ~Meto();

        string donner[1024][6];
        size_t n_list;
};

#endif // METEO_H
