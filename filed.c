#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "csapp.h"

struct requestType {
    short unsigned int requestType; // 0: newKey, 1: fileGet, 2: fileDigest, 3: fileRun
    int requestStatus;              // Simple success or failure of request.

} clientRequest;

struct connectionConfig {
    int port;
    char machineName[40];       // IPv4 or DNS Name of machine that server is running on.
    unsigned int secretKey;     // unsigned, positive, secret request key.
} configs;

void result(int connfd) // Example echo from echoclient
{
    size_t n; 
    char buffer[MAXLINE]; 
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    n = Rio_readlineb(&rio, buffer, MAXLINE); //line:netp:echo:eof
	printf("server received %d bytes\n", (int)n);
    printf("Rio buf: %s\n", rio.rio_buf);
	Rio_writen(connfd, buffer, n);
    
}

int main(int argc, char *argv[]) {
    socklen_t clientSock;
    struct sockaddr_in clientaddress;
    struct hostent *host;
    char * homeAddress;
    int listener, connector;
    //rio_t requestKey[4];        // Secret key used by client for request.
    if(argc != 3) {
        perror("Could not start server based on command line argruments.\n");
        return 0;
    }
    else {  // Set the port number and secret key
        configs.port = atoi(argv[1]);
        configs.secretKey = atoi(argv[2]);
        
    }
    
    listener = Open_listenfd(configs.port);

    
    while(1) {
        clientSock = sizeof(clientaddress);
        connector = Accept(listener, (SA*)&clientaddress, &clientSock);
        host = Gethostbyaddr((const char *)&clientaddress.sin_addr.s_addr,
                            sizeof(clientaddress.sin_addr.s_addr), AF_INET);
        homeAddress = inet_ntoa(clientaddress.sin_addr);
        printf("Secret key = %d\n",configs.secretKey );
        printf("Request type = %s\n", argv[0] );
        printf("Detail = add newkey here\n");
        printf("--------------------------");
        printf("port: %d\n", configs.port);
        
        printf("Connected to: %s from: %s\n", host->h_name, homeAddress);

        result(connector);
        Close(connector);
    }

    


    exit(0);
}