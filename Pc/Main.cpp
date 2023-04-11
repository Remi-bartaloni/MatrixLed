#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <sstream>

using namespace std;

struct information
{
    short int cpuUtilisation = -1;
    short int gpuUtilisation = -1;
    short int ramUtilisation = -1;
};

string windowsCommande(const char *text) {
    string result = "";
    char buffer[1024];
    FILE* pipe = popen(text, "r");                  // exécute la commande "dir" et ouvre un flux de fichier pour lire le résultat

    if (!pipe) {                                    // vérifie si la commande s'est bien exécutée
        return "Erreur";
    }

    while (!feof(pipe)) {                           // lit le flux de fichier tant qu'il n'est pas fini
        if (fgets(buffer, 1024, pipe) != NULL) {    // récupère une ligne du flux de fichier
            result += buffer;                         // affiche la ligne à l'écran
        }
    }
    pclose(pipe);                                   // ferme le flux de fichier
    return result;
}

void cpuCalculUtilisation(struct information *info) {
    short int nbSicle = 3;
    short int sum = 0;
    short int i = nbSicle;
    
    do {
        --i;
        sum += stoi(windowsCommande("wmic cpu get loadpercentage | findstr [0-9]"));
        Sleep(100);
    } while (i != 0);

    info->cpuUtilisation = sum / nbSicle;
}

void gpuCalculUtilisation(struct information *info, const char *commande) {
    short int nbSicle = 3;
    short int sum = 0;
    short int i = nbSicle;
    
    do {
        --i;
        sum += stoi(windowsCommande(commande));
        Sleep(100);
    } while (i != 0);

    info->gpuUtilisation = sum / nbSicle;
}

void ramCalculUtilisation(struct information *info, long long int maxRam) {
    short int nbSicle = 3;
    long long int sum = 0;
    short int i = nbSicle;
    
    do {
        --i;
        sum += stoi(windowsCommande("wmic OS get FreePhysicalMemory | findstr [0-9]"));
        Sleep(100);
    } while (i != 0);
    
    info->ramUtilisation = ((maxRam - (sum / nbSicle)) * 100) / maxRam;
}

int main(int argc, char const *argv[])
{
    struct information *info = new information;
    
    string gpuCommande;
    string gpuMarque = windowsCommande("wmic path win32_videocontroller get name");

    string maxRamString = windowsCommande("wmic ComputerSystem get TotalPhysicalMemory | findstr [0-9]");
    stringstream ss;
    long long int maxRamInt = 0;
    ss << maxRamString;
    ss >> maxRamInt;
    maxRamInt /= 1024;


    if (gpuMarque.find("NVIDIA") != string::npos) {
        gpuCommande = "nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits";
    }
    /*
    ----------------------- A tester ------------------------------------

    else if (gpuMarque.find("Radeon") != string::npos) {
        gpuCommande = "rocm-smi -u –showuse";
    }
    else if (gpuMarque.find("Intel") != string::npos) {
        gpuCommande = "";
    }
    */
    

    
    cpuCalculUtilisation(info);
    gpuCalculUtilisation(info, gpuCommande.c_str());
    ramCalculUtilisation(info, maxRamInt);

    cout << "CPU : " << info->cpuUtilisation << endl;
    cout << "GPU : " << info->gpuUtilisation << endl;
    cout << "RAM : " << info->ramUtilisation << endl;
    
    delete info;
    return 0;
}
