#include "client_tcp.h"

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


Client_tcp::Client_tcp(short port, string ip):port(port), ip(ip)
{
    struct sockaddr_in serverAddr;

    if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        cout<<"[+]Socket Creer."<<endl;

        bzero(&serverAddr, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());

        if((connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr))) != -1)
        {
            cout<<"[+]Connecter au Server."<<endl;
        }
        else
        {
            cout<<"connect erreur"<<endl;
        }

    }
    else
    {
        cout<<"sock erreur"<<endl;
    }

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


void Client_tcp::envoyer(string &rq)
{
    write(clientSocket, rq.c_str(), rq.size());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------


void Client_tcp::recevoir(string &tram)
{
    char bef;
    fd_set fd;
    int fin=0, err=0;


    FD_ZERO(&fd);

    while(!fin)
    {
        ti.tv_sec=1;
        ti.tv_usec=0;
        FD_SET(clientSocket,&fd);
        if(select(clientSocket+1, &fd, NULL, NULL, &ti) == 1)
        {
            err=0;  
            if(read(clientSocket, &bef, 1) == 1)
            {
                tram.append(&bef, 1);
                //cout<< "recevoir : " << bef <<endl;
            }
            else fin=1;
        }
        else if(++err > 5) fin=1; 
    }
    cout<<"Deconecter"<<endl;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

Client_tcp::~Client_tcp()
{
    close (clientSocket);
}
