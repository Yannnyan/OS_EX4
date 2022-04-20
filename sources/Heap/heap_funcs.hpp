
#include <stdio.h>
#include <stdlib.h>

typedef struct block
{
    size_t size; // 8
    int free; // 4
    block * next; // 8
}block;

// thread safety functions
void * _malloc(size_t size);
void _free(void * addr);
void * _calloc(size_t member_num, size_t member_size);
// the actual implementation for the functions
void * _malloc1(size_t size);
void _free1(void * addr);
void * _calloc1(size_t member_num, size_t member_size);
