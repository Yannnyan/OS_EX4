
#include <stdio.h>
#include <stdlib.h>

typedef struct block
{
    size_t size; // 8
    int free; // 4
    block * next; // 8
}block;

void * _malloc(size_t size);
void _free(void * addr);
void * _calloc(size_t member_num, size_t member_size);
