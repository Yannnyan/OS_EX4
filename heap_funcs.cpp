#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

#include "heap_funcs.hpp"

pthread_mutex_t lock;


typedef struct block
{
    size_t size; // 8
    int free; // 4
    block * next; // 8
}block;

block * head = NULL;

#define BLOCK_SIZE sizeof(struct block)

// allocate size bytes in the heap, if function is successfull, returns address of the allocated mem
// return NULL if the function fails or if size to alocate is 0
void * _malloc(size_t size)
{   
    // pthread_mutex_lock(&lock);
    // malloc with size 0 returns NULL
    if (size == 0)
    {
        // pthread_mutex_unlock(&lock);
        return NULL;
    }
    // get the current address
    intptr_t * addr = (intptr_t *) sbrk(0);
    // set current to the top of the list
    block ** current = &head;
    // iterate while current is not null
    while((*current) != NULL)
    {
        // if the block is free and the size is enough then return the address for the allocated memory
        if ((*current)->free && (*current)->size >= size)
        {
            // found a good block, set that the block is ocupied and return it's address
            (*current)->free = 0;
            return ((*current) + 1);
        }
        current = &((*current)->next);
    }
    // if we couldn't find a block that is good, then alocate new block in the heap
    if (sbrk(BLOCK_SIZE + size) == (void *) -1 )
    {
        return NULL;
    }
    // set the new block fields, and add it to the linked list
    block * newhead = (block *)addr;
    newhead->next = head;
    newhead->size = size;
    newhead->free = 0;
    head = newhead;
    // pthread_mutex_unlock(&lock);
    // lastly return the block
    return newhead + 1;
}


// receiving the address the user wants to free
void _free(void * ptr)
{
    // trying to free null pointer
    if (ptr == NULL)
    {
        throw(std::invalid_argument("null pointer cannot be freed."));
    }
    
    // storing variable with the address
    block * block_address = (block *) ptr;
    // getting the address of the block
    block_address -= 1;
    // checking if the block exists inside the linked list, if not then it's probably defined on the stack so, cant free this block.
    int found = 0;
    block ** current = &head;
    while((*current) != NULL )
    {
        if ((*current) == block_address)
        {
            found = 1;
            break;
        }
        current = &((*current)-> next);
    }
    if (found == 0)
    {
        throw(std::invalid_argument("cannot free block that is not pointing to address on the heap."));
    }
    // if this block is already free then throw an exception
    if ((block_address->free))
    {
        throw(std::invalid_argument("free already freed block."));
    }
    // else set the block to be freed
    block_address->free = 1;
}


