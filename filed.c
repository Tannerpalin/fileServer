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
#include <sys/stat.h>


#define BACKL 50   // Number of pending connections to hold in queue.

struct requestIn {  // Struct for information coming in from client.
    unsigned int keyIn;
    unsigned short int requestType;
    char padding[2];
    char requestData[100];
};

struct requestOut { // Struct for information going out to client.
    char returnCode;      
    char padOut[3];
    unsigned short int length;
    char fileData[100];
};

void sigchld_handler(int s) // Handling of signals received by child during execution.
{
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
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
    struct stat st;
    int yes=1;
    int rv;
    unsigned int serverKey;

    if (argc != 3) {    // ./filed port secretKey
        fprintf(stderr,"Usage: ./filed port secretKey\n");
        return -1;
    }
    serverKey = (unsigned int)atoi(argv[2]);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through all the results and bind to the first we can.
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
        fprintf(stderr, "server: failed to set port\n");
        return -1;
    }

    if (listen(sockfd, BACKL) == -1) {
        perror("Could not listen to connection requests.");
        return -1;
    }

    sa.sa_handler = sigchld_handler; // reaps all dead processes.
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return -1;
    }
    
    while(1) {  // main accepting loop.
        char results[8];
        char typeOf[16];
        sin_size = sizeof(their_addr);
        int fdPipe[2];      // Pipe for cryptographic digest output.
        pipe(fdPipe);
        close(fdPipe[1]);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("failure in accepting request");
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
            else {  // If request meets authorization, begin execution of request.
            switch (requestIn.requestType) {
                
                case 0: // New Key
                strcpy(typeOf, "newKey");
                serverKey = (unsigned int)atoi(requestIn.requestData);  
                requestOut.returnCode = (char)0;
                strcpy(results, "Success");
                send(new_fd, &requestOut, sizeof(requestOut),0);
                break;

                case 1: //File Get
                strcpy(typeOf, "fileGet");
                FILE *filePtr;
                long fileLength;
                filePtr = fopen(requestIn.requestData, "r");
                if(filePtr == 0) {
                    strcpy(results,"Failure");
                    requestOut.returnCode = (char)-1;
                    send(new_fd, &requestOut, sizeof(requestOut),0);
                    break;
                }
                int fdSize;
                fdSize = fileno(filePtr);
                fstat(fdSize, &st);
                fileLength = (int)st.st_size;
                
                if(fileLength > 100) {
                    requestOut.length = 100;
                    fread(requestOut.fileData, 100, 1, filePtr);
                    requestOut.returnCode = (char)0;
                }
                else {
                    requestOut.length = (unsigned short int)fileLength;
                    fread(requestOut.fileData, fileLength, 1, filePtr);
                    requestOut.returnCode = (char)0;
                }
                strcpy(results, "Success");
                send(new_fd, &requestOut, sizeof(requestOut),0);
                break;

                case 2: // File Digest
                strcpy(typeOf, "fileDigest");
                int saveOut;
                int saveErr;
                char systemCall[121];
                strcpy(systemCall, "/usr/bin/sha256sum ");
                FILE *filePtr2;
                filePtr2 = fopen(requestIn.requestData, "r");
                if(filePtr2 == 0) {
                    strcpy(results,"Failure");
                    requestOut.returnCode = (char)-1;
                    send(new_fd, &requestOut, sizeof(requestOut),0);
                    break;
                }
                saveOut = dup(1);
                saveErr = dup(2);
                strcat(systemCall, requestIn.requestData);
                int pipeFd[2];
                pipe(pipeFd);       // Pipe to receive the std output and err from digest.
                dup2(pipeFd[1], 1);
                dup2(pipeFd[1], 2);
                // Using system() instead of the fork() -> exec() solution.
                system(systemCall);
                read(pipeFd[0], requestOut.fileData, 100);
                
                close(pipeFd[1]);
                close(pipeFd[0]);
                dup2(saveErr, 2);      // Restoration of standard output and err
                dup2(saveOut, 1);
                close(saveErr);
                close(saveOut);
                strcpy(results,"Success");
                requestOut.returnCode = (char)0;
                send(new_fd, &requestOut, sizeof(requestOut),0);
                break;

                case 3:
                // File Run
                strcpy(typeOf, "fileRun");
                int saveOutRun;
                int saveErrRun;
                char systemCallRun[101];

                if(strcmp(requestIn.requestData, "inet") == 0) {
                    strcpy(systemCallRun, "/bin/ip address");
                }
                else if(strcmp(requestIn.requestData, "hosts") == 0) {
                    strcpy(systemCallRun, "/bin/cat /etc/hosts");
                }
                else if(strcmp(requestIn.requestData, "service") == 0) {
                    strcpy(systemCallRun, "/bin/cat /etc/services");
                }
                else if(strcmp(requestIn.requestData, "identity") == 0) {
                    strcpy(systemCallRun, "/bin/hostname");
                }
                else {
                    strcpy(results,"Failure");
                    requestOut.returnCode = (char)-1;
                    send(new_fd, &requestOut, sizeof(requestOut),0);
                    break;
                }
                saveOutRun = dup(1);
                saveErrRun = dup(2);
               
                int pipeFdRun[2];
                pipe(pipeFdRun);        // Piping to receive output from child process.
                dup2(pipeFdRun[1], 1);
                dup2(pipeFdRun[1], 2);
                
                system(systemCallRun); // TODO: Should probably error check system call.
                read(pipeFdRun[0], requestOut.fileData, 100);   // Get data from pipe.
                
                close(pipeFdRun[1]);
                close(pipeFdRun[0]);
                dup2(saveErrRun, 2);    // Restoring standard output and error from pipe.
                dup2(saveOutRun, 1);
                close(saveErrRun);
                close(saveOutRun);
                strcpy(results,"Success");
                requestOut.returnCode = (char)0;
                send(new_fd, &requestOut, sizeof(requestOut),0);
                break;

                default:
                    
                break;
            }
        }
        // Server-side display of request status.
        printf("Secret key = %d\n", requestIn.keyIn);
        printf("Request type = %s\n", typeOf);
        printf("Detail = %s\n", requestIn.requestData);
        printf("Completion = %s\n", results);
        printf("-----------------------\n");
        close(new_fd);  // Close out the requesting connecting.
        
    }

    return 0;
}