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

#define BLOCK_SIZE sizeof(block)

// return NULL if the function fails or if size to alocate is 0
void * _malloc(size_t size)
{   
    pthread_mutex_lock(&lock);
    // malloc with size 0 returns NULL
    if (size == 0)
    {
        return NULL;
    }
    // get the current address
    intptr_t * addr = (intptr_t *) sbrk(0);
    // set current to the top of the list
    block * current = head;
    // iterate while current is not null
    while(current)
    {
        // if the block is free and the size is enough then return the address for the allocated memory
        if (current->free && current->size >= size)
        {
            // found a good block, set that the block is ocupied and return it's address
            current->free = 0;
            return (current + BLOCK_SIZE);
        }
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
    pthread_mutex_unlock(&lock);
    // lastly return the block
    return addr + BLOCK_SIZE;
}


// receiving the address the user wants to free
void _free(void * ptr)
{
    // storing variable with the address
    block * block_address = (block *) ptr;
    // getting the address of the block
    block_address -= 1;
    // if this block is already free then throw an exception
    if (block_address->free)
    {
        throw(std::invalid_argument("free already freed block."));
    }
    // else set the block to be freed
    block_address->free = 1;
}


