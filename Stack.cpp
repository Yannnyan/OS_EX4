#include <string>
#include <iostream>
#include "Stack.hpp"

using namespace ex4;
ex4::Stack::Stack()
{
    this -> head = new str_Node();
    this -> size = 0;
}
ex4::Stack::~Stack()
{
    str_Node * current = this-> get_head();
    str_Node * prev = NULL;
    while(current != NULL) // iterate through the linked list and destroy each node untill we reach the end.
    {
        prev = current;
        current = current->getNext();
        delete(prev);
    }
}   

string ex4::Stack::POP()
{

}
void ex4::Stack::PUSH(const string & inp)
{

}

string ex4::Stack::TOP()
{

}





