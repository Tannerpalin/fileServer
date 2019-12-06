/*
** client.c -- a stream socket client demo
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

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

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
}clientRequest;


int main(int argc, char *argv[])
{

    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
        

    if (argc != 5) {
        fprintf(stderr,"Usage: client hostname port\n");
        exit(1);
    }
    clientRequest.requestType = 0;
    clientRequest.secretKey = atoi(argv[3]);
    for (int i = 0; i < strlen(argv[3]); i++)
    {
        if(isdigit(argv[3][i])==0){fprintf(stderr,"invalid\n"); exit(EXIT_FAILURE);}  
    }
    for (int j = 0; j < strlen(argv[4]); j++)
        {
            if(isdigit(argv[4][j])==0){fprintf(stderr,"invalid\n");exit(EXIT_FAILURE);}
        }
    
    
    strcpy(clientRequest.requestData,argv[4]);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
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

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
    write(sockfd, &clientRequest, sizeof(clientRequest));
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);


    close(sockfd);

    return 0;
}
















// struct requestConfig {
//     unsigned short port;            // Port server is listening to.
//     char *secretKey;         // Current secret key server is using.
//     char *newKey;            // New secret key for server to use.
//     char hostName[40];   // Host name of server.
//     char *requestType;
// } requestInfo;


// int main(int argc, char *argv[]) {
//     initConfigs();
//     int clientConnection;
//     char *status = malloc(16);
    
//     if(argc != 5) {
//         perror("Unable to make request with command line arguments.\n");
//         return -1;
//     }
//     else{   // Retrieve request information from command line.
//         requestInfo.port = atoi(argv[2]);
//         strcpy(requestInfo.hostName, argv[1]);
//         strcpy(requestInfo.secretKey, argv[3]);
//         strcpy(requestInfo.newKey, argv[4]);
//         *requestInfo.requestType = (unsigned short int)0;
//     }

    
//     //Close(clientConnection);
//     return 0;
// }