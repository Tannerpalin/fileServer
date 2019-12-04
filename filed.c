#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <csapp.h>

struct requestType {
    
    int requestStatus;              // Simple success or failure of request.

} clientRequest;

struct connectionConfig {
    int port;
    char machineName[40];       // IPv4 or DNS Name of machine that server is running on.
    unsigned int secretKey;     // unsigned, positive, secret request key.
} configs;

int main(int argc, char *argv[]) {
    if(argc != 3) {
        perror("Could not start server based on command line argruments.\n");
        return 0;
    }
    else {  // Set the port number and secret key
        configs.port = atoi(argv[1]);
        configs.secretKey = atoi(argv[2]);
        // TODO: Need to get IPv4 or DNS of the machine server is running on.
        // inet_ntoa(configs.machineName);
    }
    printf("port: %d\n", configs.port);
    printf("Secret key: %d\n", configs.secretKey);
    printf("Machine name: \n");
}