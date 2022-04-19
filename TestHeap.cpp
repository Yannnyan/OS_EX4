#include "doctest.h"
#include "sources/Heap/heap_funcs.hpp"



TEST_CASE("Good input")
{
    // check retrurns null for 0 
    CHECK(!_malloc(0));
    // check works for different types
    char * str;
    CHECK_NOTHROW(str = (char *)_malloc(1024));
    CHECK(str);
    int * numbers;
    CHECK_NOTHROW(numbers = (int *) _malloc(sizeof(int) * 100) );
    CHECK(numbers);
    size_t * numbers1;
    CHECK_NOTHROW(numbers1 = (size_t *)_malloc(sizeof(size_t) * 120));
    CHECK(numbers1);
    // check works for pointer
    char ** arr; 
    CHECK_NOTHROW(arr = (char **)_malloc(sizeof(char *) * 10));
    CHECK(arr);
    // try to dynamically alocate an array
    for(int i=0; i< 10; i++)
    {
        CHECK_NOTHROW(arr[i] = (char *)_malloc(sizeof(char) * 1));
        CHECK(arr[i]);
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

TEST_CASE("different sizes")
{
    char * arr = (char *) _malloc(100 * sizeof(char));
    CHECK(arr);
    CHECK_NOTHROW(_free(arr));
    char * arr1 = (char *) _malloc(99 * sizeof(char));
    CHECK(arr == arr1);
    char * arr2 = (char *) _malloc(100 * sizeof(char));
    CHECK(arr2);
    CHECK_NOTHROW(_free(arr2));
    char * arr3 = (char *) _malloc(200 * sizeof(char));
    CHECK(arr3);
    CHECK_NOTHROW(_free(arr3));


}
TEST_CASE("double free")
{
    char * arr = (char * ) _malloc(10 * sizeof(char));
    CHECK(arr);
    CHECK_NOTHROW(_free(arr));
    CHECK_THROWS(_free(arr));


    char ** array = (char **) _malloc(sizeof(char *) * 3);
    CHECK_NOTHROW(_free(array));
    CHECK_THROWS(_free(array));

}

