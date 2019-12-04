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
    char * response[64];
    
    char *message = malloc(12);
    char padding[2] = "pa";
    if(argc != 5) {
        perror("Unable to make request with command line arguments.\n");
        return -1;
    }
    else{   // Retrieve request information from command line.
        requestInfo.port = atoi(argv[2]);
        strcpy(requestInfo.machineName, argv[1]);
        *requestInfo.secretKey = (unsigned int)atoi(argv[3]);
        *requestInfo.newKey = (unsigned int)atoi(argv[4]);
        *requestInfo.requestType = (unsigned short int)0;
    }

    clientConnection = Open_clientfd(requestInfo.machineName, requestInfo.port);

    Rio_readinitb(&rio, clientConnection);
    
    
    //sprintf(message, "%d%hu%c%c%d",requestInfo.secretKey,requestInfo.requestType,padding[0],
    //                    padding[1],requestInfo.newKey);
    //memcpy
    size_t two = 2;
    size_t four = 4;
    memcpy(message,requestInfo.secretKey,four);
    memcpy(message +4, requestInfo.requestType,two);
    memcpy(message +6, padding, two);
    memcpy(message +8, requestInfo.newKey, four);
    
    //printf("size of message: %ld\n",sizeof(requestInfo.secretKey) + sizeof(requestInfo.requestType)
    //            + sizeof(padding[0]) + sizeof(padding[1]) + sizeof(requestInfo.newKey));
    printf("message contents: %.12s\n", message);
    printf("message size: %ld\n", sizeof(message));
    Rio_writen(clientConnection, message, sizeof(message) );
    Rio_readlineb(&rio, response, 64);
    printf("%s", *response);
    close(clientConnection);
    return 0;
}