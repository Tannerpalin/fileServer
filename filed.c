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
    //ize_t n; // or whatver number is needed for request.
    char buffer[MAXLINE]; 
    rio_t rioR;

    Rio_readinitb(&rioR, connfd);
    //Rio_readnb(&rioR, buffer, n);
    //n = Rio_readlineb(&rioR, buffer, MAXLINE); //line:netp:echo:eof
	//printf("server received %d bytes\n", (int)n);

    
	//Rio_writen(connfd, buffer, n);
    printf("another attempt rio buf: %s\n", rioR.rio_buf);
    printf("buffer: %s\n", buffer);
    printf("size of rio buf: %ld\n", sizeof(rioR.rio_buf));
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

        // NEED TO READ IN CHUNKS OF DATA ONE LINE AT A TIME.
        size_t w;
        rio_t rio;
        
        
        char * keyIn = malloc(4);
        char *typeReq = malloc(2);
        char *padd = malloc(2);
        Rio_readinitb(&rio, connector);
        Rio_readlineb(&rio, keyIn, 32); // Receive the secret key chunk, check validity.
        Rio_readlineb(&rio, typeReq, 32); // Receive the request type chunk.
        Rio_readlineb(&rio, padd, 32); // Receive the two bytes of padding.

        printf("Secret key in: %d\n", atoi(keyIn));
        printf("server secret key: %d\n", (unsigned int)configs.secretKey);
        printf("Request type: %d\n", atoi(typeReq));
        printf("Pad: %d received\n", *padd);
        if(atoi(keyIn) != (unsigned int)configs.secretKey) {
            Rio_writen(connector, "failure\n",9);
        }


        
        
        
        
        

        
        
        
        
        
        
        // Receive parameters based on the request type.

        // Call function passing parameters depending on request type.
        // Write back to client success or information within its funcion.
        //char *results = malloc(4);
        //strcpy(results, "Suc\n");
        //Rio_writen(connector, results,16);         // Write back to client success/fail.
        
        
        Close(connector);   // Close client connection.
    }

    


    exit(0);
}