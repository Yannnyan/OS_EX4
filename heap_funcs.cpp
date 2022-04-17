#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock;


typedef struct block
{
    size_t size;
    intptr_t addr;
    block * next;
}block;

block * head = NULL;


// return NULL if the function fails or if size to alocate is 0
void * _malloc(size_t size)
{   
    pthread_mutex_lock(&lock);
    if (size == 0)
    {
        return NULL;
    }
    intptr_t * addr = (intptr_t *) sbrk(0);
    // head of the linked list is null then set the new head
    if (head == NULL)
    {
        head->addr = *addr;
        head->next = NULL;
        head->size = size; 
    }
    // head of the linked list is not null then add the new block to the linked list
    else
    {
        block newblock;
        newblock.addr = *addr;
        newblock.next = head;
        newblock.size = size;
        
    }
    if (sbrk(*addr) == (void *) -1 )
    {
        return NULL;
    }
    pthread_mutex_unlock(&lock);
    return addr;
}


void _free(void * ptr)
{
    intptr_t * addr = (intptr_t *)sbrk(0);
    (*addr) -= sizeof(ptr);
    if (brk(addr) == -1)
    {}
}


