#include "heap_funcs.hpp"



int main()
{

    char * h = (char *)_malloc(sizeof(char) * 100);
    for(int i=0; i< 10; i++)
    {
        h[i] = 'h';
    }
    h[10] = '\0';
    puts(h);
    _free(h);
    _free(h);
    return 0;
}