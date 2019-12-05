#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct requestConfig {
    unsigned short port;            // Port server is listening to.
    unsigned int secretKey;         // Current secret key server is using.
    char fileName[100];            // New secret key for server to use.
    char machineName[40];   // Host name of server.
} requestInfo;

int main(int argc, char *argv[]) {
    if(argc != 5) {
        perror("Unable to make request with command line arguments.\n");
        return -1;
    }
    else{   // Retrieve request information from command line.
        requestInfo.port = atoi(argv[2]);
        strcpy(requestInfo.machineName, argv[1]);
        requestInfo.secretKey = atoi(argv[3]);
        strcpy(requestInfo.fileName, argv[4]);
    }
    // Test retrieval of command line arguments.
    printf("MachineName: %s\n", requestInfo.machineName);
    printf("Port: %d\n", requestInfo.port);
    printf("Secret Key: %d\n", requestInfo.secretKey);
    printf("File name: %s\n", requestInfo.fileName);
}