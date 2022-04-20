#include <string>
#include <iostream>
#include "Stack.hpp"
#include <mutex>


using namespace ex4;
using namespace std;

pthread_mutex_t  lock1 = PTHREAD_MUTEX_INITIALIZER;

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

ex4::str_Node::str_Node()
{
    this->next = NULL;
}
// construct a node and set its data to str
// set next to null.
ex4::str_Node::str_Node(const string & str)
{
    this->data = str;
    this->next = NULL;
}
str_Node * ex4::str_Node::getNext()
{
    pthread_mutex_lock(&lock1);
    str_Node * ret = this->getNext1();
    pthread_mutex_unlock(&lock1);
    return ret;
}
// set the next node.
void ex4::str_Node::setNext(string & next)
{
    if (this -> next != NULL)
    {
        
    }
    else
    {
        this -> next = new str_Node(next);
    }
}
void ex4::str_Node::setNext(str_Node * next)
{
    if (this -> next != NULL)
    {
        
    }
    else
    {
        this -> next = next;
    } 
}
// get the data of the node
string & ex4::str_Node::getData()
{
    return this->data;
}
// set the data of the node
void ex4::str_Node::setData(string data)
{
    this -> data = data;
}

string ex4::Stack::POP1()
{
    pthread_mutex_lock(&lock1);
    string temp = this->POP();
    pthread_mutex_unlock(&lock1);
    return temp;
}

string ex4::Stack::POP()
{
    str_Node * h = this->head;
    this->head = h->getNext();
    string str = h->getData();
    free(h);
    this->size -=1;
    return str;
}
void ex4::Stack::PUSH(string inp)
{
    pthread_mutex_lock(&lock1);
    if( inp.size() > 1024)
    {
        pthread_mutex_unlock(&lock1);
        throw(invalid_argument("ERROR: string is larger than 1025 bytes."));
    }
    str_Node * node = new str_Node(inp);
    node-> setNext(this->head);
    this->head = node;
    this->size+=1;
    pthread_mutex_unlock(&lock1);
}

string ex4::Stack::TOP()
{
    pthread_mutex_lock(&lock1);
    string temp = this -> TOP1();
    pthread_mutex_unlock(&lock1);
    return temp;
}

string ex4::Stack::TOP1()
{
    return this->get_head()->getData();
}





