#include <string>
#include <iostream>
#include "Stack.hpp"
#include <pthread.h>

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
    pthread_mutex_lock(&(this->lock));
    str_Node * h = this->head;
    this->head = h->getNext();
    string str = h->getData();
    free(h);
    pthread_mutex_unlock(&(this->lock));
    return str;
}
void ex4::Stack::PUSH(string & inp)
{
    pthread_mutex_lock(&(this->lock));
    if( inp.size() > 1024)
    {
        throw(invalid_argument("ERROR: string is larger than 1025 bytes."));
    }
    str_Node * node = new str_Node(inp);
    node-> setNext(this->head);
    this->head = node;
    pthread_mutex_unlock(&(this->lock));

}

string ex4::Stack::TOP()
{
    pthread_mutex_lock(&(this->lock));
    return this->get_head()->getData();
    pthread_mutex_unlock(&(this->lock));
}





