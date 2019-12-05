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



struct requestStruct {
    unsigned int key, newKey;
    short unsigned int requestType; // 0: newKey, 1: fileGet, 2: fileDigest, 3: fileRun
    int requestStatus;              // Simple success or failure of request.

} requestStruct;


int main(int argc, char *argv[]) {
    struct sockaddr_in sa;

    if(argc != 3) {
        perror("Could not start server based on command line argruments.\n");
        return 0;
    }
    else {  // Set the port number and secret key from command line.
        inet_pton(AF_INET,"127.0.0.1", &(sa.sin_addr)); // Save IP address to socket.
        
    }
    

    
    while(1) {
        
    }

    exit(0);
}