
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

char * getString()
{
    char * str = (char *) malloc(BUFFERSIZE * sizeof(char));
    int index = 0;
    char c;
    while((c = getc(stdin)) != EOF && c != 0 && c != '\n')
    {
        if (index >= BUFFERSIZE)
        {
            free(str);
            fprintf(stdout, "ERROR: buffer too long.");
            return NULL;
        }
        str[index++] = c;
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
        fprintf(stderr,"ERROR: usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "ERROR: getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("ERROR: [Client] socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("ERROR: [Client] connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "ERROR: [Client] failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("[Client] connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    char buf[BUFFERSIZE];
    char * temp;
    while(1)
    {
        temp = getString();
        numbytes = strlen((char *)temp) + 1;
        memset(buf, 0, BUFFERSIZE);
        if (temp == NULL)
        {
            fprintf(stdout, "ERROR: cant send more than %d\n",BUFFERSIZE);
            continue;
        }
        printf("[Client] Sending %s, %d bytes to server. . .\n", temp, numbytes);
        if ((numbytes = send(sockfd, temp, strlen(temp) + 1, 0)) == 0 || numbytes == -1)
        {
            perror("ERROR: send");
            exit(1);
        }
        free(temp);
        if ((numbytes = recv(sockfd, buf, BUFFERSIZE-1, 0)) == -1) {
            perror("ERROR: recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        fprintf(stdout, "OUTPUT: %s\n", buf);
    }
    close(sockfd);

    return 0;
}