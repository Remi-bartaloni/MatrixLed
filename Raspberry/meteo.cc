#include "meteo.h"

Meto::Meto(string &domaine, string &requet)
{
    string ip;
    
    struct hostent *host;

    host = gethostbyname(domaine.c_str());

    ip = inet_ntoa(*((struct in_addr * )host->h_addr));

    cout<< ip <<endl;
    
    client = new Client_tcp(80, ip);
    client->envoyer(requet);
    client->recevoir(tram);

    int position;
    string un = "{\"";
    position = tram.find(un);
    tram = tram.substr(position, tram.size());

    //cout<< tram <<endl;
    delete(client);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


string Meto::get_tram()
{
    return tram;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


void Meto::parsed()
{
    struct json_object *parsed_json;
    struct json_object *list;
    struct json_object *lists;
    struct json_object *dt_txt;

    string buffer = tram;

    parsed_json = json_tokener_parse(buffer.c_str());

    json_object_object_get_ex(parsed_json, "list", &list);

    n_list = json_object_array_length(list);

    string listString[n_list];

    for(size_t i = 0; i < n_list; i++) {
        lists = json_object_array_get_idx(list, i);
        listString[i] = json_object_get_string(lists);
    }

    string weathersString[n_list];

    struct json_object *weather;
    struct json_object *weathers;

    size_t weathtaille = 0;

    for(size_t i = 0; i < n_list; i++) {
        //cout << listString[i] << endl;

        parsed_json = json_tokener_parse(listString[i].c_str());
        json_object_object_get_ex(parsed_json, "weather", &weather);
        json_object_object_get_ex(parsed_json, "dt_txt", &dt_txt);

        size_t n_weather = json_object_array_length(weather);

        for(size_t o = 0; o < n_weather; o++) {
            weathers = json_object_array_get_idx(weather, o);
            weathersString[weathtaille] = json_object_get_string(weathers);

            donner[weathtaille][0] = json_object_get_string(dt_txt);
            ++weathtaille;
        }
    }

    struct json_object *icon;

    for(size_t i = 0; i < weathtaille; i++) {

        parsed_json = json_tokener_parse(weathersString[i].c_str());
        json_object_object_get_ex(parsed_json, "icon", &icon);
        //printf("icon: %s\n", json_object_get_string(icon));
        donner[i][1] = json_object_get_string(icon);
    }


    struct json_object *main;

    struct json_object *temp;
    struct json_object *temp_min;
    struct json_object *temp_max;
    struct json_object *humidity;


    for(size_t i = 0; i < n_list; i++) {
        //cout << listString[i] << endl;

        parsed_json = json_tokener_parse(listString[i].c_str());
        json_object_object_get_ex(parsed_json, "main", &main);

        parsed_json = json_tokener_parse(json_object_get_string(main));
        json_object_object_get_ex(parsed_json, "temp", &temp);
        json_object_object_get_ex(parsed_json, "temp_min", &temp_min);
        json_object_object_get_ex(parsed_json, "temp_max", &temp_max);
        json_object_object_get_ex(parsed_json, "humidity", &humidity);

        donner[i][2] = json_object_get_string(temp);
        donner[i][3] = json_object_get_string(temp_min);
        donner[i][4] = json_object_get_string(temp_max);
        donner[i][5] = json_object_get_string(humidity);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


string Meto::get_temp(int semaine)
{
    string date[7] = {"--", "--", "--", "--", "--", "--", "--"};
    size_t ii = 0;

    time_t rawtime;
    struct tm * ptm;

    time(&rawtime);

    ptm = localtime(&rawtime);
    string heur;

    if (ptm->tm_hour > 0 && ptm->tm_hour <= 3)  heur = "03:00:00";
    if (ptm->tm_hour > 3 && ptm->tm_hour <= 6)  heur = "06:00:00";
    if (ptm->tm_hour > 6 && ptm->tm_hour <= 9)  heur = "09:00:00";
    if (ptm->tm_hour > 9 && ptm->tm_hour <= 12)  heur = "12:00:00";
    if (ptm->tm_hour > 12 && ptm->tm_hour <= 15)  heur = "15:00:00";
    if (ptm->tm_hour > 15 && ptm->tm_hour <= 18)  heur = "18:00:00";
    if (ptm->tm_hour > 18 && ptm->tm_hour <= 21)  heur = "21:00:00";
    if (ptm->tm_hour > 21 && ptm->tm_hour <= 23)  heur = "0:00:00";

    for(size_t i = 0; i < n_list; ++i) {
        if (donner[i][0].substr(11, 19) == heur.c_str())
        {
            int nb = donner[i][2].find(".");
            if(nb != -1)    date[ii] = donner[i][2].substr(0, nb+2);
            else date[ii] = donner[i][2];
            //cout << date[ii] << endl;
            ++ii;
        }
    }

    return date[semaine];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


string Meto::get_humidity(int semaine)
{
    string date[7] = {"--", "--", "--", "--", "--", "--", "--"};
    size_t ii = 0;

    time_t rawtime;
    struct tm * ptm;

    time(&rawtime);

    ptm = localtime(&rawtime);
    string heur;

    if (ptm->tm_hour > 0 && ptm->tm_hour <= 3)  heur = "03:00:00";
    if (ptm->tm_hour > 3 && ptm->tm_hour <= 6)  heur = "06:00:00";
    if (ptm->tm_hour > 6 && ptm->tm_hour <= 9)  heur = "09:00:00";
    if (ptm->tm_hour > 9 && ptm->tm_hour <= 12)  heur = "12:00:00";
    if (ptm->tm_hour > 12 && ptm->tm_hour <= 15)  heur = "15:00:00";
    if (ptm->tm_hour > 15 && ptm->tm_hour <= 18)  heur = "18:00:00";
    if (ptm->tm_hour > 18 && ptm->tm_hour <= 21)  heur = "21:00:00";
    if (ptm->tm_hour > 21 && ptm->tm_hour <= 23)  heur = "0:00:00";

    for(size_t i = 0; i < n_list; ++i) {
        if (donner[i][0].substr(11, 19) == heur.c_str())
        {
            date[ii] = donner[i][5];
            //cout << date[ii] << endl;
            ++ii;
        }
    }

    return date[semaine];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


string Meto::get_icon(int semaine)
{
    string date[7] = {"--", "--", "--", "--", "--", "--", "--"};
    size_t ii = 0;

    time_t rawtime;
    struct tm * ptm;

    time(&rawtime);

    ptm = localtime(&rawtime);
    string heur;

    if (ptm->tm_hour > 0 && ptm->tm_hour <= 3)  heur = "03:00:00";
    if (ptm->tm_hour > 3 && ptm->tm_hour <= 6)  heur = "06:00:00";
    if (ptm->tm_hour > 6 && ptm->tm_hour <= 9)  heur = "09:00:00";
    if (ptm->tm_hour > 9 && ptm->tm_hour <= 12)  heur = "12:00:00";
    if (ptm->tm_hour > 12 && ptm->tm_hour <= 15)  heur = "15:00:00";
    if (ptm->tm_hour > 15 && ptm->tm_hour <= 18)  heur = "18:00:00";
    if (ptm->tm_hour > 18 && ptm->tm_hour <= 21)  heur = "21:00:00";
    if (ptm->tm_hour > 21 && ptm->tm_hour <= 23)  heur = "0:00:00";

    for(size_t i = 0; i < n_list; ++i) {
        if (donner[i][0].substr(11, 19) == heur.c_str())
        {
            date[ii] = donner[i][1];
            //cout << date[ii] << endl;
            ++ii;
        }
    }

    return date[semaine];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


Meto::~Meto()
{
}
