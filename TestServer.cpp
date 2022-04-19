#include "doctest.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void makeToChildren(char *const arr1 [],char *const arr2 []){
  int i=0;    
  if ((i = fork()) == 0) // father forks child
  { 
    if ((i = fork()) == 0) // child forks child
    { 
      if (execv("Client", arr1)==-1) // run second child on client, if execv failed terminate.
      {
          CHECK (1==-1);
          exit(1);
      }
    }
    else if (i==-1) // if fork for the second child failed, terminate.
    {
      CHECK(1==-1);
      exit(1);
    }
    else
    {
      if (execv("Client", arr2)==-1) // run first child of the client, if execv failed terminate.
      {
        CHECK (1==-1);
        exit(1);
      }
      wait(NULL);
    }
  }
  else if (i==-1) // if fork for the first child failed terminate.
  {
    CHECK(1==-1);
    exit(1);
  }
  wait(NULL);
}


TEST_CASE("Good input")
{
  char* const arr1 []= {(char*)"Client", (char*)"127.0.0",(char*)"PUSH hello",(char*)"TOP",(char*)"POP he", NULL};
  char*  const arr2 []= {(char*)"Client", (char*)"127.0.0",(char*)"PUSH hello",(char*)"TOP",(char*)"POP he", NULL};
  makeToChildren(arr1, arr2);
  
}

TEST_CASE("Bad Input")
{

}


TEST_CASE("double free")
{

}

TEST_CASE("alot mallocs")
{


}
