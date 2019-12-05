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

} request;

struct connectionConfig {
    int port;
    char machineName[40];       // IPv4 or DNS Name of machine that server is running on.
    unsigned int secretKey;     // unsigned, positive, secret request key.
} configs;



int main(int argc, char *argv[]) {
    
}