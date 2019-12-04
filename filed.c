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
    size_t n = 12; // or whatver number is needed for request.
    char buffer[MAXLINE]; 
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    Rio_readnb(&rio, buffer, n);
    //n = Rio_readlineb(&rio, buffer, MAXLINE); //line:netp:echo:eof
	printf("server received %d bytes\n", (int)n);

    
	Rio_writen(connfd, buffer, n);
    printf("another attempt rio buf: %s\n", rio.rio_buf);
    printf("buffer: %s\n", buffer);
    printf("size of rio buf: %ld\n", sizeof(rio.rio_buf));
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
        
    }
    
    listener = Open_listenfd(configs.port);

    
    while(1) {
        clientSock = sizeof(clientaddress);
        connector = Accept(listener, (SA*)&clientaddress, &clientSock);
        host = Gethostbyaddr((const char *)&clientaddress.sin_addr.s_addr,
                            sizeof(clientaddress.sin_addr.s_addr), AF_INET);
        homeAddress = inet_ntoa(clientaddress.sin_addr);
        printf("Secret key = %d\n",configs.secretKey ); // Needs to be key supplied by client.
        printf("Request type = %s\n", argv[0] );
        printf("Detail = add newkey here\n");
        printf("--------------------------");
        printf("port: %d\n", configs.port);
        
        printf("Connected to: %s from: %s\n", host->h_name, homeAddress);
        rio_t secretRio;
        char bufferIn[12];
        size_t p;
        Rio_readinitb(&secretRio, connector);
       

        p = Rio_readnb(&secretRio, bufferIn, 12);
        printf("Number of bytes read: %d\n", (int)p);
        printf("server secret key: %d\n", (unsigned int)configs.secretKey);
        
        char keyIn[4];
        char typeReq[2];
        printf("Buffer in: %.24x\n", *bufferIn);
        memcpy(keyIn, bufferIn, 4);
        memcpy(typeReq, bufferIn + 4, 2);
        printf("keyIn: %.8x\n", *keyIn);
        printf("Request In: %.4x\n", *typeReq);
        Rio_writen(connector,"Suc\n",4);         // Write back to client success/fail.
        
        
        Close(connector);   // Close client connection.
    }

    


    exit(0);
}