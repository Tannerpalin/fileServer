#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "csapp.h"

struct requestType {
    
    int requestStatus;              // Simple success or failure of request.

} clientRequest;

struct connectionConfig {
    int port;
    char machineName[40];       // IPv4 or DNS Name of machine that server is running on.
    unsigned int secretKey;     // unsigned, positive, secret request key.
} configs;

void echo(int connfd) 
{
    size_t n; 
    char buf[MAXLINE]; 
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
	printf("server received %d bytes\n", (int)n);
	Rio_writen(connfd, buf, n);
    }
}

int main(int argc, char *argv[]) {
    socklen_t clientSock;
    struct sockaddr_in clientaddress;
    struct hostent *host;
    char * homeAddress;
    int listener, connector;

    if(argc != 3) {
        perror("Could not start server based on command line argruments.\n");
        return 0;
    }
    else {  // Set the port number and secret key
        configs.port = atoi(argv[1]);
        configs.secretKey = atoi(argv[2]);
        // TODO: Need to get IPv4 or DNS of the machine server is running on.
        // inet_ntoa(configs.machineName);
    }
    
    listener = Open_listenfd(configs.port);

    
    while(1) {
        clientSock = sizeof(clientaddress);
        connector = Accept(listener, (SA*)&clientaddress, &clientSock);
        host = Gethostbyaddr((const char *)&clientaddress.sin_addr.s_addr,
                            sizeof(clientaddress.sin_addr.s_addr), AF_INET);
        homeAddress = inet_ntoa(clientaddress.sin_addr);
    
        printf("port: %d\n", configs.port);
        printf("Secret key: %d\n", configs.secretKey);
        printf("Connected to: %s from: %s\n", host->h_name, homeAddress);
        echo(connector);
    }

    


    exit(0);
}