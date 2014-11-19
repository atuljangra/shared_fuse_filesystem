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
#include <string>
#include <sstream>

#include "clientHandler.h"

#include "../Message.h"
using namespace std;

#define log(...) \
    do { if (OUT) fprintf(stdout, ##__VA_ARGS__); \
        else fprintf(f, ##__VA_ARGS__); } while (0)

ClientHandler::ClientHandler() {

}

ClientHandler::ClientHandler(int port, int socketFD) {
    _portNumber = port;
    ClientHandler();
    _initSocket = socketFD;
    stringstream ss;
    ss << port << ".log";

    if(OUT) 
        f = fopen(ss.str().c_str(), "w");
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
        log("Cannot bind the socket\n");
    }

    listen(_listenSocketFD, 5);
    socklen_t clientLen = sizeof (clientAddress);

    // Send port number to the client.
    // TODO This is a race condition.
    stringstream portMsgStream;
    portMsgStream << _portNumber;
    const char * msg = portMsgStream.str().c_str();
    int writeBytes = write(_initSocket, msg, sizeof(msg));
    log("Return port number %d\n", _portNumber);
    if (writeBytes < 0) {
        log("Error replying back to client.\n");
    }


    while (true) {
        int newSockFD = accept(_listenSocketFD, (struct sockaddr *)&clientAddress, &clientLen);
        if (newSockFD < 0) {
            cerr << "Error on accept" << endl;
        }
        char buffer[MAX_MSG_SIZE];
        int readBytes = read(newSockFD, buffer, 256);
        log("Received: %d msg: %s\n", readBytes, buffer);
        _handleMessage(buffer);
    }

}

void ClientHandler::_handleMessage(char *amsg) {
   // Message * msg = Message::toMessage(amsg);

}

void ClientHandler::removeHandler() {

}

ClientHandler::~ClientHandler() {
    fclose(f);
}
