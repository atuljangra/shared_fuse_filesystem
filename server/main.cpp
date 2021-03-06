/*
 * Contains the code that handles the network communication over TCP Sockets.
 * TODO Move this code to a dedicated file/class.
 * TODO Add the handler cleanup service that will remove the unused clients that
 * have nom communicated with us for a long time.
 */
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <strings.h>
#include <cstdlib>
#include <string>
#include <pthread.h>

#include "../utils.h"
#include "clientHandler.h"
#include "../Message.h"

using namespace std;

#define MAX_CLIENTS 100

/*
 * A thread per client model.
 * Check if the message is valid and if it is valid, then spawn a new thread
 * that will handle the new client.
 */
void handleConnectionRequest(int socket) {
    char buffer[256];
    stringstream ss;
    static int portNumber = CONNECT_PORT; 
    // Read data.
    int readBytes = read(socket, buffer, 256);
    if (readBytes < 0) {
        cout << "Socket read error: " << socket << endl;
        return;
    }
    ss.str(buffer);
    int msgCode;
    ss >> msgCode;
    cout << "Message code is " << msgCode << endl;
    if (msgCode == CONNECT_CODE) {
        socklen_t len;
        struct sockaddr_storage addr;
        char ipstr[INET6_ADDRSTRLEN];
        int port;

        len = sizeof addr;
        getpeername(socket, (struct sockaddr*)&addr, &len);
        
        // deal with both IPv4 and IPv6:
        if (addr.ss_family == AF_INET) {
            struct sockaddr_in *s = (struct sockaddr_in *)&addr;
            port = ntohs(s->sin_port);
            inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
        } 
        else {
            // AF_INET6
            struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr; 
            port = ntohs(s->sin6_port);
            inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
        }
        
        cout << "Socket " << socket << " spawning new thread for " << ipstr
            << ":" << port << endl;
    }
    else {
        cout << "Socket " << socket << " unhandled message " << buffer << endl;  
    }
    
    // Send back the new port number as a reply.
    
    // One important thing is that we should not send the reply until we have a
    // new thread up and running and ready to handle connections on the
    // specified port. This can be a possible race condition.
    portNumber ++;
    ClientHandler *ch = new ClientHandler(portNumber, socket);
    ch -> start();
    cout << "Socket " << socket << " handled" << endl;
}

int main () {
    int listenFD;

    // Create the shared folder if it's not already there.
    stringstream ss;
    ss << "mkdir -p " << SERVER_ROOT;
    system(ss.str().c_str());

    struct sockaddr_in serverAddress, clientAddress;


    listenFD = socket(AF_INET, SOCK_STREAM, 0);
    // Zeroing out the serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(CONNECT_PORT);
    int yes = 1;
    if (setsockopt(listenFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        cerr << "error while setting options on socket" << endl;
    }

    if (bind(listenFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
         cerr << "Cannot bind the socket port:" << CONNECT_PORT << endl;
    }

    listen(listenFD, 5);
    socklen_t clientLen = sizeof (clientAddress);
    while (true) {
        int newSockFD = accept(listenFD, (struct sockaddr *)&clientAddress, &clientLen);
        if (newSockFD < 0) {
            cerr << "Error on accept" << endl;
        }
        handleConnectionRequest(newSockFD); 
    }

    return 0;
}

