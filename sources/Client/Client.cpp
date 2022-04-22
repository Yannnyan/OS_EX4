
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <string>

#include "Client.hpp"

using namespace std;

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

#define BUFFERSIZE 1024
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

string convertArrToString(char inp[BUFFERSIZE])
{
    string str;
    for (int i=0; i< BUFFERSIZE; i++)
    {
        str.push_back(inp[i]);
    }
    return str;
}

int main(int argc, char *argv[])
// argv[1] = "127.0.0.1"
// argv[2...] = "<command> <options>"

{
    int sockfd, numbytes;  
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc < 2) {
        fprintf(stderr,"PERROR: usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "PERROR: getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("PERROR: [Client] socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("PERROR: [Client] connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "PERROR: [Client] failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("[Client] connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    std::string temp;
    char buf[BUFFERSIZE];
    for (int arg_c = 2; arg_c < argc; arg_c++)
    {
        memset(buf, 0, BUFFERSIZE);
        printf("[Client] Sending %s to server. . .\n", argv[arg_c]);
        if (strlen(argv[arg_c]) > BUFFERSIZE)
        {
            fprintf(stdout, "PERROR: cant send more than %d\n",BUFFERSIZE);
            continue;
        }
        if ((numbytes = send(sockfd, argv[arg_c], strlen(argv[arg_c]) + 1, 0)) == 0 || numbytes == -1)
        {
            perror("PERROR: send");
            exit(1);
        }
        if ((numbytes = recv(sockfd, buf, BUFFERSIZE-1, 0)) == -1) {
            perror("PERROR: recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        fprintf(stdout, "OUTPUT: %s\n", buf);

    }

    
    


    close(sockfd);

    return 0;
}