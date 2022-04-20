#include "doctest.h"
#include <pthread.h>
#include "sources/Stack/Stack.hpp"
#include <stdbool.h>

using namespace ex4;
using namespace std;

Stack stack;
string inpT1 = "IM THREAD NUMBER 1";
string inpT1_1 = "THREAD NUMBER 1 IS THE BEST";
string inpT1_2 = "THREAD NUMBER 1 IS FIRST IN EVERYTHING";

string inpT2 = "IM THREAD NUMBER 2";
string inpT2_1 = "THREAD NUMBER 2 IS THE BEST";
string inpT2_2 = "THREAD NUMBER 2 IS FIRST IN EVERYTHING";

void *thread_func1(void * arg)
{
    
    stack.PUSH(inpT1);
    stack.PUSH(inpT1_1);
    stack.POP();
    stack.PUSH(inpT1_2);
    // should contains inpT1, and inpT1_2
    return NULL;
}
void * thread_func2(void * arg)
{
    stack.PUSH(inpT2);
    stack.PUSH(inpT2_1);
    stack.POP();
    stack.PUSH(inpT2_2);
    // should contains inpT2, and inpT2_2
    return NULL;
}
bool stack_contains(string * arr, int size, string target)
{
    for(int i=0; i< size; i++)
    {
        if (arr[i].compare(target))
        {
            return true;
        }
    }
    return false;
}
TEST_CASE("Good input")
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1, NULL, &thread_func1, NULL);
    pthread_create(&thread2, NULL, &thread_func2, NULL);
    printf("starting threads\n");
    
    printf("waiting for threads\n");
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    // check the side of the stack
    CHECK(stack.get_size() == 4);
    // check all the right elements are inside the stack
    string arr[4];
    for(int i=0; i< 4; i++)
    {
        arr[i] = stack.POP();
    }
    
    CHECK(stack_contains(arr, 4, inpT1));
    CHECK(stack_contains(arr, 4, inpT1_2));
    CHECK(stack_contains(arr, 4, inpT2));
    CHECK(stack_contains(arr, 4, inpT2_2));
}

TEST_CASE("TOP")
{
    string str = "hello world";
    stack.PUSH(str);
    cout << stack.TOP() << endl;
}




