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
    char *secretKey;         // Current secret key server is using.
    char *newKey;            // New secret key for server to use.
    char machineName[40];   // Host name of server.
    char *requestType;
} requestInfo;


int main(int argc, char *argv[]) {
    
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

    
    //Close(clientConnection);
    return 0;
}