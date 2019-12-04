#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "csapp.h"

struct requestConfig {
    unsigned short port;            // Port server is listening to.
    unsigned int secretKey;         // Current secret key server is using.
    char programName[40];            // New secret key for server to use.
    char machineName[40];   // Host name of server.
} requestInfo;

int main(int argc, char *argv[]) {
    /*
    int clientConnection;
    rio_t rio;
    char * response[64];
    char message[11];
    */
    if(argc != 5) {
        perror("Unable to make request with command line arguments.\n");
        return -1;
    }
    else{   // Retrieve request information from command line.
        requestInfo.port = atoi(argv[2]);
        strcpy(requestInfo.machineName, argv[1]);
        requestInfo.secretKey = atoi(argv[3]);
        strcpy(requestInfo.programName, argv[4]);

        /* Got some errors upon trying to merge. Might be better to use
           a series of if else statements?*/
        /*
        switch (*requestInfo.programName)
        {
        case "inet":
            *requestInfo.programName = "/bin/ip addresses";
            break;
        case "hosts":
            *requestInfo.programName = "/bin/cat /etc/hosts";
            break;
        case "service":
            *requestInfo.programName = "/bin/cat /etc/services";
            break;
        case "identity":
            *requestInfo.programName = "/bin/hostname";
            break;
        
        default:
        exit(1);
            break;
        }
        */
    }
    /*
    clientConnection = Open_clientfd(requestInfo.machineName, requestInfo.port);
    
    Rio_readinitb(&rio, clientConnection);
    sprintf(message, "%d",requestInfo.newKey);
    Rio_writen(clientConnection, message, sizeof(requestInfo.newKey) );
    Rio_readlineb(&rio, response, 64);
    printf("%s", *response);
    Close(clientConnection);
    */
    return 0;
}