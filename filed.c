#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define BACKLOG 10   // how many pending connections queue will hold

struct requestIn {
    unsigned int keyIn;
    unsigned short int requestType;
    char padding[2];
    char requestData[100];
};

struct requestOut {
    char returnCode;
    char padOut[3];
    unsigned short int length;
    char fileData[100];
};

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get the socket address, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    struct requestIn requestIn;
    struct requestOut requestOut;
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    int rv;
    unsigned int serverKey;

    if (argc != 3) {    // ./filed port
        fprintf(stderr,"Usage: ./filed port secretKey\n");
        exit(1);
    }
    serverKey = (unsigned int)atoi(argv[2]);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    
    while(1) {  // main accepting loop.
        char results[8];
        char typeOf[16];
        sin_size = sizeof(their_addr);
        
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

            recv(new_fd, &requestIn, sizeof(requestIn), 0);
            if(requestIn.keyIn != serverKey) {
                strcpy(results,"Failure");
                requestOut.returnCode = (char)-1;
                send(new_fd, &requestOut, sizeof(requestOut),0);
                switch (requestIn.requestType) {
                    case 0:
                    strcpy(typeOf,"newKey");
                    break;
                    case 1:
                    strcpy(typeOf,"fileGet");
                    break;
                    case 2:
                    strcpy(typeOf,"fileDigest");
                    break;
                    case 3:
                    strcpy(typeOf,"fileRun");
                    break;
                }
            }
            else {
            switch (requestIn.requestType) {
                
                case 0:
                strcpy(typeOf, "newKey");
                serverKey = (unsigned int)atoi(requestIn.requestData);  
                requestOut.returnCode = (char)0;
                send(new_fd, &requestOut, sizeof(requestOut),0);
                break;

                case 1:
                strcpy(typeOf, "fileGet");
                requestOut.returnCode = (char)-1; // Haven't implemented yet so fail.
                send(new_fd, &requestOut, sizeof(requestOut),0);
               
                break;

                case 2:
                strcpy(typeOf, "fileDigest");
                requestOut.returnCode = (char)-1; // Haven't implemented yet so fail.
                send(new_fd, &requestOut, sizeof(requestOut),0);
                break;

                case 3:
                strcpy(typeOf, "fileRun");
                requestOut.returnCode = (char)-1; // Haven't implemented yet so fail.
                send(new_fd, &requestOut, sizeof(requestOut),0);
                break;

                default:
                    
                break;
            }
                strcpy(results, "Success");
                
            }
        printf("Secret key = %d\n", requestIn.keyIn);
        printf("Request type = %s\n", typeOf);
        printf("Detail = %s\n", requestIn.requestData);
        printf("Completion = %s\n", results);
        printf("-----------------------\n");
        close(new_fd);  // parent doesn't need this
        
    }

    return 0;
}