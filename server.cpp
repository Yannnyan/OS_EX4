/*
** server.c -- a stream socket server demo
*/

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
#include <pthread.h>
#include <stdbool.h>
#include "../Stack/Stack.hpp"

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10   // how many pending connections queue will hold
 
#define BUFFERSIZE 1024

using namespace std;

ex4::Stack stack;


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
char * extract_value(char * str)
{
    // define an char array with 1024 bytes
    char * value = (char *) calloc(strlen(str) + 1 , sizeof(char));
    // define indexes
    size_t i =0;
    size_t index_value = 0;
    size_t len = strlen(str);
    // loop untill you find special character
    while(i < len && str[i] != '\n' && str[i] != '\0' && str[i] != ' ')
    {
        i ++;
    }
    i++;
    // loop untill we reached the end of the value
    while(i< len && str[i] != '\n' && str[i] != '\0')
    {
        value[index_value++] = str[i++];
    }
    // add null terminator to the string
    value[index_value] = '\0';
    return value;
}
char * convert_string_to_array(string & str)
{
    char * arr = (char *)calloc(BUFFERSIZE, sizeof(char));
    for(size_t i=0; i< str.size(); i++)
    {
        arr[i] = str.at(i);
    }
    arr[str.size()] = '\0';
    return arr;
}
string convertArrToString(char inp[BUFFERSIZE])
{
    string str;
    for (size_t i=0; i< strlen(inp); i++)
    {
        str.push_back(inp[i]);
    }
    return str;
}
bool startsWith(char * target, char * prefix)
{
    // if the prefix is longer than the target then return false
    if (strlen(prefix) > strlen(target))
    {
        return false;
    }
    size_t len = strlen(prefix);
    // loop through the first characters untill we reached the length of the prefix and if one character is not equal return false
    for(size_t i=0; i< len; i++)
    {
        if (target[i] != prefix[i])
        {
            return false;
        }
    }
    return true;
}

void send_message(int client_sockfd, char * buffer, size_t size_buffer, int flags)
{
    if (send(client_sockfd, buffer, size_buffer, flags) == -1)
    {
        perror("ERROR: send returned -1");
    }
}

void handleCall(int client_sockfd, char inp[BUFFERSIZE])
{
    char * sendBuffer;
    if (startsWith(inp, (char *)"TOP"))
    {
        string str = stack.TOP();
        cout << "OUTPUT: " + str << endl;
        sendBuffer = convert_string_to_array(str);
        send_message(client_sockfd,sendBuffer ,strlen(sendBuffer) + 1, 0);
        free(sendBuffer);
    }
    else if (startsWith(inp,(char *)"POP"))
    {
        string str = stack.POP();
        sendBuffer = convert_string_to_array(str);
        send_message(client_sockfd, sendBuffer, strlen(sendBuffer) + 1, 0);
        free(sendBuffer);
    }
    else if (startsWith(inp, (char *)"PUSH"))
    {
        sendBuffer = (char *)"Pushed into stack";
        char * value = extract_value(inp);
        string str = convertArrToString(value);
        stack.PUSH(str);
        send_message(client_sockfd, sendBuffer, strlen(sendBuffer) + 1, 0);
        free(value);
    }
    else{
        sendBuffer = (char *)"ERROR: cannot interprate message";
        send_message(client_sockfd, sendBuffer, strlen(sendBuffer) + 1, 0);
    }
}

void * thread_func(void * args)
{
    int new_sockfd = *(int*) args;
    char recv_buffer[BUFFERSIZE];
    
    memset(recv_buffer, 0, BUFFERSIZE);
    int r = 0;
    while(1)
    {
        if ((r = recv(new_sockfd, recv_buffer , BUFFERSIZE, 0)) == 0  || r == -1)
        {
            perror("ERROR: recved 0 or -1 from recv\n");
            break;
        }
        printf("[Server] Received msg from client.\n");
        handleCall(new_sockfd, recv_buffer);
        memset(recv_buffer, 0, BUFFERSIZE);
    }
    
    close(new_sockfd);
    pthread_exit(NULL);

}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    pthread_t buffer_thread;
    pthread_t thread_pool[BACKLOG]; // TID array by index, 10 members
    bool active_threads[BACKLOG]; // array of booleans
    //int thread_cursor = 0;
    memset(thread_pool, 0, sizeof(thread_pool)); // zero out all cells
    memset(active_threads, 0, sizeof(active_threads));


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "ERROR: getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("ERROR: [Server]  socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("ERROR: setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("ERROR: [Server]  bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "ERROR: [Server] failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("ERROR: listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("ERROR: sigaction");
        exit(1);
    }

    printf("[Server] Waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("ERROR: accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof(s));
        printf("[Server] Got connection from %s\n", s);
        pthread_create(&buffer_thread, NULL, &thread_func, &new_fd);
    }

    return 0;
}