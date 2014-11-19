#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <string>
#include <thread>
#include <condition_variable>
#include "../utils.h"

/*
 * A single theaded handler for the clients. 
 */
class ClientHandler {
    private:
        FILE *f;
        int _portNumber;
        std::thread _thread;
        int _initSocket;
        int _listenSocketFD;
        std::condition_variable _cv;        
        void _threadListner();
        void _handleMessage(char *msg);
    public:
        ClientHandler();
        ClientHandler(int port, int socketFD);
        // start listening on the specified port.
        void start();
        void setPortNumber(int port);
        void removeHandler();
        ~ClientHandler();

};

#endif
