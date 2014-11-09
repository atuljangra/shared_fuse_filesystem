#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <string>
#include <thread>

/*
 * A single theaded handler for the clients. 
 */
class ClientHandler {
    private:
        int _portNumber;
    public:
        ClientHandler();
        ClientHandler(int port);
        void setPortNumber(int port);
        void removeHandler();
        ~ClientHandler();

};

#endif
