#ifndef CLIENT_TCP_H
#define CLIENT_TCP_H

#include <string>
using namespace std;

class Client_tcp
{
        int clientSocket;
        short port;
        string ip;
        struct timeval ti;

    public:
        Client_tcp(short, string);

        void envoyer(string&);

        void recevoir(string&);

        ~Client_tcp();
};

#endif
