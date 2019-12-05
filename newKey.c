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
    
    char message[12];
    char padding[2] = "pa";
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
    printf("secret key used: %d\n", atoi(requestInfo.secretKey));
    clientConnection = Open_clientfd(requestInfo.machineName, requestInfo.port);

    Rio_readinitb(&rio, clientConnection);
    

    char secretKeyOut[4];
    memcpy(secretKeyOut, requestInfo.secretKey, 4);
    
    Rio_writen(clientConnection, requestInfo.secretKey, strlen(requestInfo.secretKey));

    
    Rio_readlineb(&rio, response, 4);
    printf("%s", rio.rio_buf);
    Close(clientConnection);
    return 0;
}