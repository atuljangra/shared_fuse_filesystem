#include <iostream>
#include <sstream>

#include "clientHandler.h"

using namespace std;

ClientHandler::ClientHandler() {

}

ClientHandler::ClientHandler(int port) {
    _portNumber = port;
    ClientHandler();
}

void ClientHandler::setPortNumber(int port) {
    _portNumber = port;
}

// Prepare and start the new thread.
void ClientHandler::start() {
    _thread = thread(&ClientHandler::_threadListner, this);    
}

void ClientHandler::_threadListner() {

}

void ClientHandler::removeHandler() {

}

ClientHandler::~ClientHandler() {

}
