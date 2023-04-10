#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <Windows.h>

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
    short int moyenne = 0;
    short int i = nbSicle;
    
    do {
        --i;
        moyenne += stoi(windowsCommande("wmic cpu get loadpercentage | findstr [0-9]"));
        Sleep(100);
    } while (i != 0);

    info->cpuUtilisation = moyenne / nbSicle;
}

int main(int argc, char const *argv[])
{
    struct information *info = new information;
    cpuCalculUtilisation(info);



    //info->cpuUtilisation = stoi(windowsCommande("wmic cpu get loadpercentage | findstr [0-9]"));

    cout << info->cpuUtilisation << endl;
    cout << info->gpuUtilisation << endl;
    cout << info->ramUtilisation << endl;
    
    delete info;
    return 0;
}
