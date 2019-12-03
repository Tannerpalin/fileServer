#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <csapp.h>

struct requestType {
    unsigned int secretKey;         // unsigned, positive, secret request key.
    int requestStatus;              // Simple success or failure of request.
    char machineName[40];

} clientRequest;

int main(/* command line arguments here? */) {

}