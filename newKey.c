#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct requestConfig {
    unsigned short port;            // Port server is listening to.
    unsigned int secretKey;         // Current secret key server is using.
    unsigned int newKey;            // New secret key for server to use.
    char machineName[40];   // Host name of server.
} requestInfo;

void main(int argc, char *argv[]) {
    if(argc != 5) {
        perror("Unable to make request with command line arguments.\n");
        return;
    }
    else{
        requestInfo.port = atoi(argv[2]);
        strcpy(requestInfo.machineName, argv[1]);
        requestInfo.secretKey = atoi(argv[3]);
        requestInfo.newKey = atoi(argv[4]);
    }
    // Test retrieval of command line arguments.
    printf("MachineName: %s\n", requestInfo.machineName);
    printf("Port: %d\n", requestInfo.port);
    printf("Secret Key: %d\n", requestInfo.secretKey);
    printf("New Key: %d\n", requestInfo.newKey);
}