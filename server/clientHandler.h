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
        std::thread _thread;
        
        void _threadListner();
    public:
        ClientHandler();
        ClientHandler(int port);
        // start listening on the specified port.
        void start();
        void setPortNumber(int port);
        void removeHandler();
        ~ClientHandler();

};

#endif
