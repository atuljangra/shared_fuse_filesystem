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

void ClientHandler::removeHandler() {

}

ClientHandler::~ClientHandler() {

}
