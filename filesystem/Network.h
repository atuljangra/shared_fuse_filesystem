#ifndef NETWORK_H
#define NETWORK_H

#include "../Message.h"

class Network {
    private:
        int _port;
        // Singleton class.
        static Network * _instance;
        Network() {};
    public:
        static Network * getInstance() {
            if (_instance == 0)
                _instance = new Network();
            return _instance;
        }      
        int send(Message *msg);
        int startListening();
};
#endif
