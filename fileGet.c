/*
** fileGet.c -- retrieve 100 bytes of a file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ctype.h>
#include <arpa/inet.h>

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

struct clientRequest
{
    unsigned int secretKey;
    unsigned short int requestType;
    char padding[2];
    char requestData[100];
};

struct serverReturn {
    char returnCode;
    char padOut[3];
    unsigned short int length;
    char fileData[100];
};

int main(int argc, char *argv[])
{
    struct clientRequest clientRequest;
    struct serverReturn serverReturn;
    int sockfd;  
    
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
        
    //
    if (argc != 5) {
        fprintf(stderr,"Usage: client hostname port secretKey newKey\n");
        return -1;
    }
    // Populating fileGet request struct with proper information.
    clientRequest.requestType = 1;
    clientRequest.secretKey = atoi(argv[3]);
    // Ensure file name is less than or equal to 100 characters.
    if(strlen(argv[4]) > 100) {
        fprintf(stderr, "File name is more than 100 characters.\n");
        return -1;
    }
    
    strcpy(clientRequest.requestData,argv[4]);
    memset(&hints, 0, sizeof hints );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Populate connection struct based on command line input.
    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through linked list of results and connect to the first acceptable result.
    for(p = servinfo; p != NULL; p = p->ai_next) {

        // If not acceptable connection, loop again.
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) { // Error checking for connection.
        fprintf(stderr, "client: failed to connect\n");
        return -1;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);   // Storing address from socket if needed.

    freeaddrinfo(servinfo); // all done with this structure
    write(sockfd, &clientRequest, sizeof(clientRequest));
    recv(sockfd, &serverReturn, sizeof(serverReturn), 0);
    switch (serverReturn.returnCode)    // Print status of request.
    {
    case 0:

        printf("%.*s\n", (int)strlen(serverReturn.fileData),serverReturn.fileData);
        break;
    case -1:
        printf("failure\n");
        break;
    
    default:
        // Do not want to get stuck in case statement for random edge case.
    break;
    }

    close(sockfd); // Close connection upon exit.

    return 0;
}
