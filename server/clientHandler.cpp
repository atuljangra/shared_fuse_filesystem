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

#define log(...) \
    do { if (OUT) { printf("%d: ", _portNumber); fprintf(stdout, ##__VA_ARGS__);} \
        else fprintf(f, ##__VA_ARGS__); } while (0)

// Following uses log
#include "Filehandler.h"

using namespace std;


ClientHandler::ClientHandler() {

}

const char * fpath(const char * x) {
    stringstream ss;
    ss << SERVER_ROOT << x;
    return ss.str().c_str();
}

ClientHandler::ClientHandler(int port, int socketFD) {
    _portNumber = port;
    ClientHandler();
    _initSocket = socketFD;
    stringstream ss;
    ss << port << ".log";
    // TODO CLose this file 
    if(!OUT) 
        f = fopen(ss.str().c_str(), "w");
}

void ClientHandler::setPortNumber(int port) {
    _portNumber = port;
}

// Prepare and start the new thread.
void ClientHandler::start() {
    cout << "Starting new thread for " << _portNumber << endl;
     _thread = thread(&ClientHandler::_threadListner, this); 
    cout << "Main thread: waiting for socket to listening on port " << _portNumber << endl;
            
}

void ClientHandler::_threadListner() {
    // Start listening.
    cout << _portNumber << ":Started thread " << this_thread::get_id() << " on port " << _portNumber << endl; 
    struct sockaddr_in serverAddress, clientAddress;
    _listenSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    // Zeroing out the serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(_portNumber);

    int yes = 1;
    if (setsockopt(_listenSocketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        cerr << "error while setting options on socket" << endl;
    }
    if (bind(_listenSocketFD , (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        log("%d:Cannot bind the socket\n", _portNumber);
        return;
    }

    listen(_listenSocketFD, 5);
    socklen_t clientLen = sizeof (clientAddress);

    // end port number to the client.
    // TODO This is a race condition.
    stringstream portMsgStream;
    portMsgStream << _portNumber;
    const char * msg = portMsgStream.str().c_str();
    int writeBytes = write(_initSocket, msg, sizeof(msg));
    log("Return port number %d\n", _portNumber);
    if (writeBytes < 0) {
        log("Error replying back to client.\n");
    }
    close(_initSocket);

    log("%d:Entering the loop\n", _portNumber);
    while (true) {
        int newSockFD = accept(_listenSocketFD, (struct sockaddr *)&clientAddress, &clientLen);
        if (newSockFD < 0) {
            log("%d:Error on accept", _portNumber);
        }
        char buffer[MAX_MSG_SIZE];
        bzero(buffer, MAX_MSG_SIZE);
        int readBytes = read(newSockFD, buffer, 256);
        log("%d:Received: %d msg: %s\n", _portNumber, readBytes, buffer);
        _handleMessage(buffer, newSockFD);
    }
    log("%d:Exiting loop\n", _portNumber);
}

void ClientHandler::_handleMessage(char *buffer, int socket) {
    Message * msg = Message::toMessage(buffer);
    int flag = msg->_code;
    Message * retMsg = new Message();
    log("Handling message %d\n", flag);
    switch(flag)
    {
        case GETATTR: {
            struct stat *statbuf;
            statbuf = (struct stat *) malloc(sizeof(struct stat));
            const char *path = msg->_msg.c_str();
            int result = file_getAttr(fpath(path), statbuf);
            retMsg -> _code = GETATTR;
            retMsg -> _ret = result;
            retMsg -> _msg = "";
            printf("GetAttr ret:%d path:%s\n", result, path); 
            // This is only needed if we succeed
            if (result >= 0) {
                char *statChar;
                statChar = (char *)malloc(sizeof(struct stat));
                memcpy(statChar, statbuf, sizeof(struct stat));
                retMsg -> _msg = string(statChar);
            }
            break;
        }

        case STATFS: {
            struct statvfs statInfo;
            
            const char *path = msg -> _msg.c_str();
            int result = file_statfs(fpath(path), &statInfo);
            retMsg -> _code = STATFS;
            retMsg -> _ret = result;
            retMsg ->  _msg = "";
            log("Statfs ret:%d path:%s\n", result, path);
            if (result >= 0) {
                char *statChar;
                statChar = (char *)malloc(sizeof(struct statvfs));
                memcpy(statChar, &statInfo, sizeof(struct statvfs));
                retMsg -> _msg = string(statChar);
            }
            break;
        }
    }

    // Send the retMsg back to the socket.
    const char * writeBuffer = retMsg -> serialize();
    int writtenBytes = write(socket, writeBuffer, sizeof(writeBuffer));
    if (writtenBytes < 0)
        log("Error sending response\n");

}


void ClientHandler::removeHandler() {

}

ClientHandler::~ClientHandler() {
    if(!OUT) 
        fclose(f);
}
