#include "Network.h"

using namespace std;

#define log(...) \
                do { if (OUT) fprintf(stdout, ##__VA_ARGS__); \
                    else fprintf(f, ##__VA_ARGS__); } while (0)

Network * Network::_instance = 0;

int Network::handShake() {
	log("Trying initial handshake with the server %s port%d\n", SERVER, CONNECT_PORT);
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
        log("Error connecting\n");
        return -1;
    }

    stringstream ss;
    ss << CONNECT_CODE << endl;
    const char * buffer1 = ss.str().c_str();
    log("Writing %s\n", buffer1);
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
    return 0;
}

int Network::send(Message * msg) {
	Message *nullmsg = NULL;
    return send(msg, false, nullmsg);
}

int Network::send(Message *msg, bool wait , Message *retMsg) {
	int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[MAX_MSG_SIZE];
    
    // Connect port number
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
    serv_addr.sin_port = htons(_port);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { 
        log("ERROR connecting\n");
        return -1;
    }

    stringstream ss;
	// Convert message to a char *.
    const char *msgBuffer = msg -> serialize();
    // The size we pass here is important. We need to send entire message
    // which can contain NULL too. 
    n = write(sockfd,msgBuffer,strlen(msgBuffer));
    log("Wrote %s\n", msgBuffer); 
    if (n < 0) {
        log("ERROR writing to socket\n");
        return -1;
    }
   
    if(!wait) 
        return 0;

    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) {
        log("ERROR reading from socket\n");
        return -1;
    }

    close(sockfd);
	// Convert a buffer to a msg.
    log("Received buffer %s\n", buffer);
    Message::fillMessage(retMsg, buffer);
    log("Ret: %d Code: %d length: %d msg:%s \n", retMsg -> _ret, retMsg ->_code,
            retMsg -> _size, retMsg -> _buffer);
	return 0;
}
