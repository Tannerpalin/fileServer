#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "csapp.h"

struct requestConfig {
    unsigned short port;            // Port server is listening to.
    unsigned int secretKey;         // Current secret key server is using.
    unsigned int newKey;            // New secret key for server to use.
    char machineName[40];   // Host name of server.
    unsigned short requestType;
} requestInfo;

int main(int argc, char *argv[]) {
    int clientConnection;
    rio_t rio;
    char * response[64];
    char contents[11];
    char *message = contents;
    char padding[2] = "pa";
    if(argc != 5) {
        perror("Unable to make request with command line arguments.\n");
        return -1;
    }
    else{   // Retrieve request information from command line.
        requestInfo.port = atoi(argv[2]);
        strcpy(requestInfo.machineName, argv[1]);
        requestInfo.secretKey = atoi(argv[3]);
        requestInfo.newKey = atoi(argv[4]);
        requestInfo.requestType = 0;
    }

    clientConnection = Open_clientfd(requestInfo.machineName, requestInfo.port);

    Rio_readinitb(&rio, clientConnection);
    
    
    sprintf(message, "%d%d%c%c%d\n",requestInfo.secretKey,requestInfo.requestType,padding[0],
                        padding[1],requestInfo.newKey);
    printf("size of message: %ld\n",sizeof(sizeof(requestInfo.secretKey) + sizeof(requestInfo.requestType)
                + sizeof(padding) + sizeof(requestInfo.newKey)));
    printf("message: %s\n", message);
    Rio_writen(clientConnection, message, sizeof(contents) );
    Rio_readlineb(&rio, response, 64);
    printf("%s", *response);
    close(clientConnection);
    return 0;
}