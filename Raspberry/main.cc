#include "main.h"

struct information
{
    short int cpuUtilisation = -1;
    short int gpuUtilisation = -1;
    short int ramUtilisation = -1;
};

int mapInt(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int identification(rgb_matrix::RuntimeOptions runtime_opt, RGBMatrix::Options matrix_options) {
    rgb_matrix::Font font;
    Color color(255, 255, 255);

    const char bdf_font_file[] = "fonts/10x20.bdf";
    if (!font.LoadFont(bdf_font_file)) {
		cerr << "Couldn't load font" << endl;
		return 1;
	}

    RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
	if (canvas == NULL) return 1;

    for (int i = 0; i < matrix_options.chain_length; i++) {
        DrawText(canvas, font, matrix_options.rows * i , font.height(), color, to_string(i+1).c_str());
    }

    //this_thread::sleep_for(std::chrono::milliseconds(10));
    sleep(10);
    delete canvas;
    return 0;
}

void afficherGifStatus(RGBMatrix *canvas, information &info, bool &stop) {
    int cpuInt = 1;
    int gpuInt = 1;

    Color colorCPU(0, 0, 0);    //RGB
    Color colorFont(255, 0, 0);   //RGB
    uint8_t rColorCPU, gColorCPU, bColorCPU;
    uint8_t rColorGPU, gColorGPU, bColorGPU;
    Image *img = new Image(canvas);
    Image *img2 = new Image(canvas);
    string chemin;

    img->setBruit(18);
    img2->setBruit(18);

    while (!stop) {
        rColorCPU = mapInt(info.cpuUtilisation, 0, 100, 0, 255);
        gColorCPU = abs(mapInt(info.cpuUtilisation, 0, 100, -128, 255));
        bColorCPU = mapInt(info.cpuUtilisation, 0, 100, 128, 255);
        colorCPU = Color(rColorCPU, gColorCPU, bColorCPU);


        rColorGPU = mapInt(info.gpuUtilisation, 0, 100, 0, 255);
        gColorGPU = mapInt(info.gpuUtilisation, 100, 0, 0, 255);
        bColorGPU = 0;
        colorFont = Color(rColorGPU, gColorGPU, bColorGPU);

        img->setFiltre(colorCPU, true);
        img2->setFiltre(colorFont, true);

        canvas->Clear();

        chemin = "./img/gpu/gpu (" + to_string(gpuInt) + ").bmp";
        img2->setChemin(chemin.c_str());
        img2->setRotation(0);
        img2->printImage(0, -64);
        img2->setRotation(0);
        img2->printImage(128, 0);

        chemin = "./img/cpu/cpu (" + to_string(cpuInt) + ").bmp";
        img->setChemin(chemin.c_str());
        img->setRotation(0);
        img->printImage(0, -64);
        img->setRotation(0);
        img->printImage(128, 0);

        if (cpuInt >= 90) cpuInt = 1;
        else cpuInt++;

        if (gpuInt >= 50) gpuInt = 1;
        else gpuInt++;

        //this_thread::sleep_for(std::chrono::milliseconds(300000));
        usleep(300000);
    }

    delete img;
    delete img2;
}

void afficherMeteo(RGBMatrix *canvas, bool &stop, sem_t* semaphore, string &apiKey) {
    if(apiKey == "") return;

    Image *imgMeteo = new Image(canvas);
    string linImage;
    string temp;
    string hum;
    Color colorClear(0, 0, 0);

    rgb_matrix::Font fontMeteo;
    Color colorText(32, 0, 0);
    Color colorFiltre(32, 32, 32);

    const char *bdf_fontMeteo_file = strdup("fonts/6x10.bdf");
    if (!fontMeteo.LoadFont(bdf_fontMeteo_file)) {
        cerr << "Couldn't load fontMeteo" << endl;
        return;
	}

    string evoi = "GET http://api.openweathermap.org/data/2.5/forecast/?q=Allauch,FR&units=metric&appid=" + apiKey + " HTTP/1.1\r\nHost: api.openweathermap.org\r\nConnection: keep-alive\r\nsec-ch-ua: \"Chromium\";v=\"88\", \"Google Chrome\";v=\"88\", \";Not A Brand\";v=\"99\"\r\nCache-Control: no-cache\r\nsec-ch-ua-mobile: ?0\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.96 Safari/537.36\r\nPostman-Token: 101075e9-86b9-248d-4cb9-d40df9ad4f1d\r\nAccept: */*\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: cors\r\nSec-Fetch-Dest: empty\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n";
	string serveur_meteo = "api.openweathermap.org";

    while (!stop) {
        cerr << "stop:" << stop << endl;
        Meto *meteo = new Meto(serveur_meteo, evoi);
    	meteo->parsed();

        linImage = "./img/meteo/" + meteo->get_icon(0) + ".bmp";
        temp = meteo->get_temp(0) + "c";
        hum = meteo->get_humidity(0) + "%";

        imgMeteo->setChemin(linImage.c_str());
        imgMeteo->setRotation(0);
        imgMeteo->setFiltre(colorFiltre, false);

        sem_wait(semaphore);
        for (int i = 64; i < 128; i++) {
            DrawLine(canvas, i, 0, i, 64, colorClear);
        }

        imgMeteo->printImage(80, 0);
        DrawText(canvas, fontMeteo, 64, 64 - fontMeteo.height(), colorText, temp.c_str());
        DrawText(canvas, fontMeteo, 64*2 - 6*3, 64 - fontMeteo.height(), colorText, hum.c_str());
        sem_post(semaphore);

        delete meteo;
        sleep(30);
    }
    delete imgMeteo;
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
    information info;
    thread threadGif;
    thread threadMeteo;
    Color colorText(32, 0, 0);
    rgb_matrix::Font font;
    string apiKey = "";
    string str;

    const char *bdf_font_file = strdup("fonts/10x20.bdf");
    if (!font.LoadFont(bdf_font_file)) {
        cerr << "Couldn't load font" << endl;
		return 1;
	}

    for(int i = 0; i < argc; i++) {
        str = argv[i];

        if(!strcmp(argv[i], "--identification_matrix")) {
            identification(runtime_opt, matrix_options);
            return 0;
        }

        if(str.find("apiKey=") != string::npos) {
            apiKey = argv[i];
            apiKey.erase(0, 7);
        }
    }

    RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
	if (canvas == NULL) {
        cerr << "canvas not" << endl;
        return 1;
    }

    // Création du socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Erreur lors de la création du socket." << endl;
        return 1;
    }

    // Configuration de l'adresse du serveur
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(12345); // Port de votre choix

    // Liaison du socket à l'adresse du serveur
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "Erreur lors de la liaison du socket à l'adresse du serveur." << endl;
        return 1;
    }

    // Mise en mode non bloquant
    int flags = fcntl(serverSocket, F_GETFL, 0);
    fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);

    // Attente de connexions entrantes
    if (listen(serverSocket, 1) == -1) {
        cerr << "Erreur lors de l'attente des connexions entrantes." << endl;
        return 1;
    }

    cerr << "Serveur en attente de connexions..." << endl;

    bool client = false;
    bool stop = false;
    bool stopMeteo = false;
    Color colorClear(0, 0, 0);
    time_t curr_time;
    tm *tm_local;
    char time_string[9];
    sem_t semaphore;
    sem_init(&semaphore, 0, 1);

    while (true) {
        // Accepter la connexion entrante
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket;
        if(client == false) clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket != -1) {
            cerr << "Client connecté." << endl;
            stop = false;
            stopMeteo = true;

            // Réception des données du client
            int bytesRead = recv(clientSocket, reinterpret_cast<char*>(&info), sizeof(information), 0);
            if (bytesRead == -1) {
                // Fermeture du socket client
                client = false;
                close(clientSocket);
                cerr << "Erreur lors de la réception des données du client." << endl;
            } else if (bytesRead == 0) {
                // Fermeture du socket client
                client = false;
                close(clientSocket);
                cerr << "Client déconnecté." << endl;
            } else {
                client = true;
                cerr << "CPU: " << info.cpuUtilisation << "%\n";
                cerr << "GPU: " << info.gpuUtilisation << "%\n";
                cerr << "RAM: " << info.ramUtilisation << "%\n";

                if (threadMeteo.joinable()) {
                    threadMeteo.join();
                }

                if (!threadGif.joinable()) {
                    threadGif = thread(afficherGifStatus, canvas, ref(info), ref(stop));
                }
            }
        }

        if (client == false) {
            stop = true;
            stopMeteo = false;
            if (threadGif.joinable()) {
                threadGif.join();
                canvas->Clear();
            }

            if (!threadMeteo.joinable()) {
                threadMeteo = thread(afficherMeteo, canvas, ref(stopMeteo), &semaphore, ref(apiKey));
            }

            curr_time = time(NULL);
            tm_local = localtime(&curr_time);
            strftime(time_string, sizeof(time_string), "%H:%M", tm_local);

            sem_wait(&semaphore);
            for (int i = 0; i < 64; i++) {
                DrawLine(canvas, 0, i, 64, i, colorClear);
            }
            DrawText(canvas, font, 64 / 2 - 25, 64 / 2 + (font.height() / 2.5), colorText, time_string);
            sem_post(&semaphore);

            close(clientSocket);
            sleep(1);
        }
    }

    sem_destroy(&semaphore);
    close(serverSocket);
    delete canvas;
    return 0;
}
