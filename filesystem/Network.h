#ifndef NETWORK_H
#define NETWORK_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../Message.h"

class Network {
    private:
        int _port;
        FILE *f;
        // Singleton class.
        static Network * _instance;
        Network() {};
    public:
        static Network * getInstance() {
            if (_instance == 0)
                // TODO Open and close the files.
                _instance = new Network();
            return _instance;
        }      
        /* This do the initial handshale with the server and set up the 
         * port number.
         */
        int handShake();
        
        int send(Message * msg);
        /*
         * Send a message to the server.
         * Second parameters tells us if we want to wait for the response 
         * from the server. 
         */ 
        int send(Message * msg, bool wait, Message &retMsg);

        // TODO
        int startListening();

        // TODO: Install Handler.
};
#endif
