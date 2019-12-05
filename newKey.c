#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "csapp.h"

struct requestConfig {
    unsigned short port;            // Port server is listening to.
    char *secretKey;         // Current secret key server is using.
    char *newKey;            // New secret key for server to use.
    char machineName[40];   // Host name of server.
    char *requestType;
} requestInfo;

void initConfigs() {
    requestInfo.secretKey = malloc(sizeof(unsigned int));
    requestInfo.requestType = malloc(sizeof(unsigned short int));
    requestInfo.newKey = malloc(sizeof(unsigned int));
}

int main(int argc, char *argv[]) {
    initConfigs();
    int clientConnection;
    rio_t rio;
    char *status = malloc(4);
    
    if(argc != 5) {
        perror("Unable to make request with command line arguments.\n");
        return -1;
    }
    else{   // Retrieve request information from command line.
        requestInfo.port = atoi(argv[2]);
        strcpy(requestInfo.machineName, argv[1]);
        strcpy(requestInfo.secretKey, argv[3]);
        strcpy(requestInfo.newKey, argv[4]);
        *requestInfo.requestType = (unsigned short int)0;
    }

    // MUST SEND CHUNKS ONE AT A TIME, READ THEM IN USING READLINE?.
    // Send the client secret key chunk.
    printf("secret key used: %d\n", atoi(requestInfo.secretKey));
    clientConnection = Open_clientfd(requestInfo.machineName, requestInfo.port);
    Rio_readinitb(&rio, clientConnection);
    Rio_writen(clientConnection, requestInfo.secretKey, strlen(requestInfo.secretKey));
    
    // Send the client request type chunk.
    printf("Request type to be used: %d\n", atoi(requestInfo.requestType));
    Rio_readinitb(&rio, clientConnection);
    Rio_writen(clientConnection, requestInfo.requestType, strlen(requestInfo.requestType));
    
    // Send two bytes of padding.
    printf("Sending padding: %s\n", "p");
    Rio_readinitb(&rio, clientConnection);
    Rio_writen(clientConnection, "p", sizeof("p"));

    // Send parameters

    //Rio_readlineb(&rio, status, 16);
    //printf("%s", status);
    Close(clientConnection);
    return 0;
}