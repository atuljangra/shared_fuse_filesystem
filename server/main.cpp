/*
 * Contains the code that handles the network communication over TCP Sockets.
 * TODO Move this code to a dedicated file/class.
 */


#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>

#define MAX_CLIENTS 100
#define PORT_NUMBER 5090

int main () {
    int pID, listenFD;
    socklen_t len;

    struct sockadd_in serverAddress, clientAddress;

    // Thread pool would have been so cool.
    pthread_t threads[MAX_CLIENTS];

    listenFD = socket(AF_INET, SOCK_STREAM, 0);
    // Zeroing out the serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_Addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT_NUMBER);

    if (bind(listenFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Cannot bind the socket" << endl;

    }
    return 0;
}

