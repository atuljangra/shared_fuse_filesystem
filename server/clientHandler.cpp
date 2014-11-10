#include <iostream>
#include <sstream>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>

#include "clientHandler.h"

using namespace std;

ClientHandler::ClientHandler() {

}

ClientHandler::ClientHandler(int port, int socketFD) {
    _portNumber = port;
    ClientHandler();
    _initSocket = socketFD;
}

void ClientHandler::setPortNumber(int port) {
    _portNumber = port;
}

// Prepare and start the new thread.
void ClientHandler::start() {
    _thread = thread(&ClientHandler::_threadListner, this); 
    cout << "Main thread: waiting for socket to listening on port " << _portNumber << endl;
            
}

void ClientHandler::_threadListner() {
    // Start listening.
    cout << "Started thread " << this_thread::get_id() << " on port " << _portNumber << endl; 
    struct sockaddr_in serverAddress, clientAddress;

    _listenSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    // Zeroing out the serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(_portNumber);

    if (bind(_listenSocketFD , (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Cannot bind the socket" << endl;
    }

    listen(_listenSocketFD, 5);
    socklen_t clientLen = sizeof (clientAddress);

    // Send port number to the client.
    // TODO This is a race condition.
    stringstream portMsgStream;
    portMsgStream << _portNumber;
    const char * msg = portMsgStream.str().c_str();
    int writeBytes = write(_initSocket, msg, sizeof(msg));
    cout << "Return port number " << _portNumber << endl;
    if (writeBytes < 0) {
        cout << "Error replying back to client." << endl;
    }


    while (true) {
        int newSockFD = accept(_listenSocketFD, (struct sockaddr *)&clientAddress, &clientLen);
        if (newSockFD < 0) {
            cerr << "Error on accept" << endl;
        }
        char buffer[256];
        int readBytes = read(newSockFD, buffer, 256);
        cout << "Received: " << readBytes << " " << buffer  << endl;
    }

}
void ClientHandler::removeHandler() {

}

ClientHandler::~ClientHandler() {

}
