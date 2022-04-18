#include "doctest.h"
#include "heap_funcs.hpp"



TEST_CASE("Good input")
{
    // check retrurns null for 0 
    CHECK(!_malloc(0));
    // check works for different types
    char * str;
    CHECK_NOTHROW(str = (char *)_malloc(1024));
    int * numbers;
    CHECK_NOTHROW(numbers = (int *) _malloc(sizeof(int) * 100) );
    size_t * numbers1;
    CHECK_NOTHROW(numbers1 = (size_t *)_malloc(sizeof(size_t) * 120));
    // check works for pointer
    char ** arr; 
    CHECK_NOTHROW(arr = (char **)_malloc(sizeof(char *) * 10));
    // try to dynamically alocate an array
    for(int i=0; i< 10; i++)
    {
        CHECK_NOTHROW(arr[i] = (char *)_malloc(sizeof(char) * 1));
    }
    // free the array
    for(int i=0 ;i<10; i++)
    {
        CHECK_NOTHROW(_free(arr[i]));
    }
    CHECK_NOTHROW(_free(arr));
    // free all other members alocated
    CHECK_NOTHROW(_free(str));
    CHECK_NOTHROW(_free(numbers));
    CHECK_NOTHROW(_free(numbers1));
}

TEST_CASE("Bad Input")
{

    // CHECK_THROWS(_free(NULL));
    // int a;
    // CHECK_THROWS(_free(&a));
    // size_t b;
    // CHECK_THROWS(_free(&b));
    // char * array1;
    // CHECK_THROWS(_free(array1));
    // char ** array;
    // CHECK_THROWS(_free(array));
}


TEST_CASE("double free")
{
    char * arr = (char * ) _malloc(10 * sizeof(char));
    CHECK_NOTHROW(_free(arr));
    CHECK_THROWS(_free(arr));


    char ** array = (char **) _malloc(sizeof(char *) * 3);
    _free(array);
    _free(array);

}

