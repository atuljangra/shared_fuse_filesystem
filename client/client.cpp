#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sstream>
#include "../utils.h"


using namespace std;

/*
 * Returns the new port number.
 */
int sendConnectRequest() {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    
    // Connect port number
    portno = CONNECT_PORT;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) {
        cout << "ERROR opening socket" << endl;
        return -1;
    }
    
    server = gethostbyname(SERVER);
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { 
        cout << "ERROR connecting" << endl;
        return -1;
    }

    stringstream ss;
    ss << CONNECT_CODE << " YOLO" << endl;
    const char * buffer1 = ss.str().c_str();
    cout << "Writing " << buffer1 << endl;
    n = write(sockfd,buffer1,strlen(buffer1));
    
    if (n < 0) {
        cout << "ERROR writing to socket" << endl;
        return -1;
    }
   
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) {
        cout << "ERROR reading from socket" << endl;
        return -1;
    }
    close(sockfd);
    return atoi(buffer);
}

int main()
{
    cout << "----Welcome to the shared fuse based filesystem---" << endl;
    cout << "Lemme first initialize the client" << endl;
    cout << "Sending Request ...... " << endl;

    /* Protocol:
     * Client will send connect request and get back the new port number that is
     * assigned to the client.
     * All further communicaitons will be done on that port.
     */
    int newPort = sendConnectRequest();
    if (newPort < 0) {
        cout << "Unable to send request! Exiting" << endl;
        return 0;
    }
    cout << "Request successful. We got port number " << newPort << endl;
    return 0;
}
