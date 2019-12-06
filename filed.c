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
}requestIn;

struct requestOut {
    char returnCode;
    char padOut[3];
    unsigned short int length;
    char fileData[100];
}requestOut;

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    unsigned int serverKey;

    if (argc != 3) {    // ./filed port
        fprintf(stderr,"Usage: ./filed port\n");
        exit(1);
    }
    serverKey = (unsigned int)atoi(argv[2]);
    printf("Starting secret key: %d\n", serverKey);
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

    printf("server: waiting for connections...\n");
    
    while(1) {  // main accept() loop
        char results[8];
        
        sin_size = sizeof(their_addr);
        //close(new_fd);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

            
            recv(new_fd, &requestIn, sizeof(requestIn), 0);
            printf("Current key: %d\n", serverKey);
            if(requestIn.keyIn != serverKey) {
                printf("invalid key\n");
                requestOut.returnCode = (char)-1;
                send(new_fd, &requestOut, sizeof(requestOut),0);
            }
            else {
            switch (requestIn.requestType) {
                
                case 0:
                printf("Trying to update with: %d\n", atoi(requestIn.requestData));
                serverKey = (unsigned int)atoi(requestIn.requestData);  
                requestOut.returnCode = (char)0;
                send(new_fd, &requestOut, sizeof(requestOut),0);
                break;
                case 1:

                break;

                default:
                    
                break;
            }
                strcpy(results, "Success");
                
            }
            
            //if (send(new_fd, "Hello, world!", 13, 0) == -1)
            //    perror("send");
            printf("Here 2\n");
            
            
        
        printf("Here 3\n");
        close(new_fd);  // parent doesn't need this
        
        
    }

    return 0;
}