#ifndef FUSEFS_H
#define FUSEFS_H

#include "../Message.h"
#include "Network.h"
class FuseFS {
    private:
        const char *_root;
        static FuseFS *_instance;
        
        Network * _network;
        
        FuseFS() {
            _network = Network::getInstance(); 
        };
        int send(Message *msg);
    public:
        static FuseFS* getInstance() {
            if (_instance == 0)
                _instance = new FuseFS();
            return _instance;
        }


    
};

#endif
