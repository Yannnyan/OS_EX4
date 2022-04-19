#include <string>
#define BUFFERSIZE 1024

void sigchld_handler(int s);
void *get_in_addr(struct sockaddr *sa);
std::string convertArrToString(char inp[BUFFERSIZE]);
void handleCall(char inp[BUFFERSIZE]);
void * thread_func(void * args);
