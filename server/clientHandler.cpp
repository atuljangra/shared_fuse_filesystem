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

    // Initialize the map.
    initMapdirp();
    log("%d:Entering the loop\n", _portNumber);
    while (true) {
        int newSockFD = accept(_listenSocketFD, (struct sockaddr *)&clientAddress, &clientLen);
        if (newSockFD < 0) {
            log("%d:Error on accept", _portNumber);
            break;
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
            char *path = (char *)malloc(msg -> _size + 1);
            memset(path, 0, msg->_size + 1);
            memcpy(path, msg -> _buffer, msg -> _size);
            int result = file_getAttr(fpath(path), statbuf);
            log("Getattr %s size:%d\n", path, msg -> _size);
            retMsg -> _code = GETATTR;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            log("GetAttr ret:%d path:%s\n", result, path); 
            // This is only needed if we succeed
            if (result >= 0) {
                retMsg -> _size = sizeof(struct stat);
                memcpy(retMsg -> _buffer, statbuf, retMsg -> _size);
                log("getAttr returning %d %s\n", retMsg -> _size, retMsg -> _buffer);
            }
            break;
        }

        case STATFS: {
            struct statvfs *statInfo;
            statInfo = (struct statvfs *) malloc (sizeof(struct statvfs));
            char *path = (char *)malloc(msg -> _size + 1);
            memset(path, 0, msg->_size + 1);
            memcpy(path, msg -> _buffer, msg -> _size);
            int result = file_statfs(fpath(path), statInfo);
            retMsg -> _code = STATFS;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            log("Statfs ret:%d path:%s\n", result, path);
            if (result >= 0) {
                retMsg -> _size = sizeof(struct statvfs);
                memcpy(retMsg -> _buffer, statInfo, sizeof(struct statvfs));
                log("statfs returning %d %s\n", retMsg -> _size, retMsg -> _buffer);
            }
            free(statInfo);
            free(path);
            break;
        }
        
        case OPENDIR:
        {
            char *path = (char *)malloc(msg -> _size + 1);
            memset(path, 0, msg->_size + 1);
            memcpy(path, msg -> _buffer, msg -> _size);
            log("opendir path %s\n", fpath(path));
            int result = file_opendir(fpath(path));
            retMsg -> _code = OPENDIR;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            log("Opendir path:%s, size: %d ret:%d\n", path, msg-> _size, result);
            free(path);
            break;
        }

        case READDIR:
        {
            char *path = (char *)malloc(msg -> _size + 1);
            memset(path, 0, msg->_size + 1);
            memcpy(path, msg -> _buffer, msg -> _size);
            bool finish = false; 
            struct dirent *de = (struct dirent *)malloc(sizeof(struct dirent)); 
            int result = file_readdir(fpath(path), de, finish);
            retMsg -> _code = READDIR;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            // Struct can be NULL also.
            if (finish) {
                log("Readdir NULL\n");
            }
            else if (result == 0) {
                retMsg -> _size = sizeof(struct dirent);
                memcpy(retMsg -> _buffer, de, sizeof(dirent));
            }
            log("Readdir path:%s, ret:%d size:%d\n", path, result, retMsg->_size);
            free(path);
            free(de);
            break;
        }

        case CLOSEDIR:
        {
            char *path = (char *)malloc(msg -> _size + 1);
            memset(path, 0, msg->_size + 1);
            memcpy(path, msg -> _buffer, msg -> _size);
            int result = file_closedir(fpath(path));
            retMsg -> _code = CLOSEDIR;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            log("Closedir path:%s, ret:%d\n", path, result);
            free(path);
            break;
        }

        case OPEN:
        {
            char *path = (char *)malloc(msg -> _size + 1);
            memset(path, 0, msg->_size + 1);
            memcpy(path, msg -> _buffer, msg -> _size);
            int result = file_open(fpath(path));
            retMsg -> _code = OPEN;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            log("Open file %s ret: %d\n", path, result);
            free(path);
            break;
        }
        
        case CLOSE:
        {
            char *path = (char *)malloc(msg -> _size + 1);
            memset(path, 0, msg->_size + 1);
            memcpy(path, msg -> _buffer, msg -> _size);
            int result = file_close(fpath(path));
            retMsg -> _code = CLOSE;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            log("Close file %s ret: %d\n", path, result);
            free(path);
            break;
        }

        case READ:
        {
           int size, offset;
           memcpy(&size, msg -> _buffer, sizeof(int));
           memcpy(&offset, msg -> _buffer + sizeof(int), sizeof(int));
           int pathSize = msg -> _size - (2*sizeof(int));
           char *path = (char *) malloc(pathSize + 1);
           memset(path, 0, pathSize + 1);
           memcpy(path, msg -> _buffer + 2*sizeof(int), pathSize);
           log("Open file:%s, size:%u, offset:%u\n", path, size, offset);
           char * buf = (char *)malloc(size);
           int result = file_read(fpath(path), buf, size, offset);
           retMsg -> _code = READ;
           retMsg -> _ret = result;
           retMsg -> _size = 0;
           if (result == 0) {
               memcpy(retMsg -> _buffer, buf, result);
               log("Read %d bytes , %s\n", result, buf);
               retMsg -> _size = result;
           }
           log("Read %s ret: %d\n", path, result);
           free(buf);
           free(path);
           break;
        }

        case MKNOD:
        {
            mode_t mode;
            dev_t dev;
            memcpy(&mode, msg -> _buffer, sizeof(mode_t));
            memcpy(&dev, msg -> _buffer + sizeof(mode_t), sizeof(dev_t));
            int pathSize = msg->_size - (sizeof(mode_t) + sizeof(dev_t));
            char *path = (char *)malloc(pathSize + 1);
            memset(path, 0, pathSize + 1);
            memcpy(path, msg -> _buffer + sizeof(mode_t) + sizeof(dev_t), pathSize);
            log("Mknod on %s\n", path);
            int result = file_mknod(path, mode, dev);
            retMsg -> _code = MKNOD;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            log("Mknod %s ret: %d\n", path, result);
            break;
        }
        
        case ACCESS:
        {
            int mode;
            memcpy(&mode, msg -> _buffer, sizeof(mode_t));
            int pathSize = msg -> _size - sizeof(mode_t);
            char *path = (char *)malloc(pathSize + 1);
            memset(path, 0, pathSize + 1);
            memcpy(path, msg -> _buffer + sizeof(mode_t), pathSize);
            log("Access on %s with mode %d\n", path, mode);
            int result = file_access(path, mode);
            retMsg -> _code = ACCESS;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            if (result != 0) {
                log("Access Error %s\n", path);
            }
            log("Access %s ret %d\n", path, result);
            break;
        }
        
        case UNLINK:
        {
            char *path = (char *)malloc(msg -> _size + 1);
            memset(path, 0, msg->_size + 1);
            memcpy(path, msg -> _buffer, msg -> _size);
            int result = file_unlink(fpath(path));
            retMsg -> _code = UNLINK;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            log("Unlink file %s ret: %d\n", path, result);
            free(path);
            break;

        }

        case RENAME:
        {
            int len1, len2;
            memcpy(&len1, msg -> _buffer, sizeof(int));
            char *path = (char *)malloc(len1 + 1);
            memset(path, 0, len1 + 1);
            memcpy(path, msg -> _buffer + sizeof(int), len1);
            
            memcpy(&len2, msg -> _buffer + sizeof(int) + len1, sizeof(int));
            char *to = (char *) malloc(len2 + 1);
            memset(to, 0, len2 + 1);
            memcpy(to, msg -> _buffer + len1 + 2 *sizeof(int), len2);

            log("Rename file %s to %s\n", path, to);
            int result = file_rename(fpath(path), fpath(to));
            retMsg -> _code = RENAME;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            log("Rename file %s to %s ret: %d\n", path, to, result);
            free(path);
            free(to);
            break;

        }

        case WRITE:
        {
            int offset, size, pathSize;
            memcpy(&offset, msg -> _buffer, sizeof(int));
            memcpy(&size, msg -> _buffer + sizeof(int), sizeof(int));
            memcpy(&pathSize, msg -> _buffer + 2 * sizeof(int), sizeof(int));
            char *path = (char *)malloc(pathSize + 1);
            memset(path, 0, pathSize + 1);
            memcpy(path, msg -> _buffer + 3 * sizeof(int), pathSize);

            char *buf = (char *) malloc(size);
            memcpy(buf, msg -> _buffer + 3 * sizeof(int) + pathSize, size);

            log("Writing %s size:%d off:%d\n", path, size, offset);
            int result = file_write(path, buf, size, offset);
            retMsg -> _code = WRITE;
            retMsg -> _ret = result;
            retMsg -> _size = 0;
            log("Write %s size:%d offset:%d result:%d\n", path, size, offset, result);
            break;
        }

    }

    if (retMsg -> _code != -1) {
        // Send the retMsg back to the socket.
        const char * writeBuffer = retMsg -> serialize();
        int writtenBytes = write(socket, writeBuffer, 3 * sizeof(int) + retMsg -> _size);
        if (writtenBytes < 0)
            log("Error sending response\n");
        log("Wrote %d bytes %s \n", writtenBytes, writeBuffer);
    }
}


void ClientHandler::removeHandler() {

}

ClientHandler::~ClientHandler() {
    if(!OUT) 
        fclose(f);
}
