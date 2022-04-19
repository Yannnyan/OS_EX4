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

string convertArrToString(char inp[BUFFERSIZE])
{
    string str;
    for (int i=0; i< BUFFERSIZE; i++)
    {
        str.push_back(inp[i]);
    }
    return str;
}
char * extract_value(char * str)
{
    // define an char array with 1024 bytes
    char * value = (char *) calloc(BUFFERSIZE , sizeof(char));
    // define indexes
    size_t i =0;
    size_t index_value = 0;
    size_t len = strlen(str);
    // loop untill you find special character
    while(i < len && str[i] != '\n' && str[i] != '\0' && str[i] != ' ')
    {
        i ++;
    }
    // if we found enter or null terminator, then it means that the user didn't put any value after the instruction or the instruction is wrong
    if (str[i] == '\n' || str[i] == '\0')
    {
        return NULL;
    }
    // skip the space
    i++;
    // loop untill we reached the end of the value
    while(i< len && str[i] != '\n' && str[i] != '\0' && str[i])
    {
        value[index_value++] = str[i];
    }
    // add null terminator to the string
    value[index_value] = '\0';
    return value;
}
char * extract_instruction(char * str)
{
    // instruction must be at least 3 bytes before the null terminator
    if (strlen(str) < 3)
    {
        return (char *)"ERROR: illigal instruction";
    }
    // define instruction with 5 bytes
    char  *instruction = (char *) calloc(5, sizeof(char));
    size_t index = 0;
    // loop untill we found the first special character
    while (str[index] != ' ' && str[index] != '\0' && str[index] != '\n')
    {
        // if the instruction is longer than 5 it's not a ligal instruction 
        if (index >= 5)
        {
            return (char *)"ERROR: illigal instruction";
        }
        instruction[index] = str[index];
        index++;
    }
    // null terminator
    instruction[index] = '\0';
    return instruction;
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

void handleCall(int client_sockfd, char inp[BUFFERSIZE])
{
    string str = convertArrToString(inp);
    cout << str << endl;
    char send_buffer[BUFFERSIZE] = "Instruction: ";
    memset(send_buffer, 0, BUFFERSIZE);
    char * instruction = extract_instruction(inp);
    int r = 0;
    // we want to send a buffer to the client, that says the instruction with the value afterwards,
    // we need to determine how much bytes to add into send_buffer
    size_t new_buf_size = strlen(inp) + strlen(send_buffer);
    // if the new buf size bigger than buffersize than set to the new buffer to display some of the old buffer with the "Instruction" prefix
    size_t n = new_buf_size >= BUFFERSIZE ? BUFFERSIZE - strlen(send_buffer) - 1 : new_buf_size;
    if (startsWith(instruction, (char *)"PUSH"))
    {
        free(instruction);
        stack.PUSH(str);
        cout << inp << endl;
        strncat(send_buffer, inp, n);
    }
    else if (startsWith(instruction,  (char *)"TOP"))
    {
        free(instruction);
        string ret = stack.TOP();
        // print the returned value
        cout << ret << endl;
        char arr[BUFFERSIZE];
        // cast the top string to an array
        strcpy(arr, ret.c_str());
        new_buf_size = strlen(arr) + strlen(send_buffer);
        if (new_buf_size >= BUFFERSIZE)
        {
            if( (r = send(client_sockfd, arr, BUFFERSIZE, 0)) == 0 || r == -1)
            {
                perror("ERROR: recvd 0 or -1 from send ");
                exit(1);
            }
            return;
        }
        strncat(send_buffer, arr, strlen(arr));
    }   
    else if (startsWith(instruction,  (char *)"POP"))
    {
        free(instruction);
        string ret = stack.POP();
        // print the returned value
        cout << ret << endl;
        char arr[BUFFERSIZE];
        // cast the top string to an array
        strcpy(arr, ret.c_str());
        new_buf_size = strlen(arr) + strlen(send_buffer);
        if (new_buf_size >= BUFFERSIZE)
        {
            if( (r = send(client_sockfd, arr, BUFFERSIZE + 1, 0)) == 0 || r == -1)
            {
                perror("ERROR: recvd 0 or -1 from send ");
                exit(1);
            }
            return;
        }
        strncat(send_buffer, arr, strlen(arr));
    }
    else if (startsWith(instruction, (char *)"ERROR"))
    {
        if ((r = send(client_sockfd, send_buffer, BUFFERSIZE, 0)) == 0 || r == -1)
        {
            perror("ERROR: recved 0 or -1 from send ");
            exit(1);
        }
    }
    else{
        if (instruction != NULL)
        {
            free(instruction);
        }
       
    }
    // send the value stored in send_buffer
    if ( (r = send(client_sockfd, send_buffer, BUFFERSIZE, 0)) == 0  || r == -1 )
    {
        perror("ERROR: recved 0 or -1 from send ");
        exit(1);
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
            perror("ERROR: recved 0 or -1 from recv");
            exit(1);
        }
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
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
        pthread_create(&buffer_thread, NULL, &thread_func, &new_fd);
    }

    return 0;
}