#include "Network.h"
#include "../utils.h"

Network * Network::_instance = 0;

void Network::handShake() {
	log("Trying initial handshake with the server %s\n", SERVER);
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    
    // Connect port number
    portno = CONNECT_PORT;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) {
        log("Error opening socket\n");
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
        log("ERROR connecting\n");
        return -1;
    }

    stringstream ss;
    ss << CONNECT_CODE << " YOLO" << endl;
    const char * buffer1 = ss.str().c_str();
    log("Writing %s\n" buffer1);
    n = write(sockfd,buffer1,strlen(buffer1));
    
    if (n < 0) {
        log("ERROR writing to socket\n");
        return -1;
    }
   
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) {
        log("ERROR reading from socket\n");
        return -1;
    }
    close(sockfd);
    _port = atoi(buffer);
	log("Setting port to %d\n", _port);
}

