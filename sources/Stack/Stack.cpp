#include <string>
#include <iostream>
#include "Stack.hpp"
#include <mutex>

std::mutex _mutex1;

using namespace ex4;
using namespace std;
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
// get the next node.
str_Node * ex4::str_Node::getNext()
{
    lock_guard<mutex> lock(_mutex1);
    return this->next;
}
// set the next node.
void ex4::str_Node::setNext(string & next)
{
    lock_guard<mutex> lock(_mutex1);
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
    lock_guard<mutex> lock(_mutex1);
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
    lock_guard<mutex> lock(_mutex1);
    return this->data;
}
// set the data of the node
void ex4::str_Node::setData(string data)
{
    lock_guard<mutex> lock(_mutex1);
    this -> data = data;
}

string ex4::Stack::POP()
{
    lock_guard<mutex> lock(_mutex1);
    str_Node * h = this->head;
    this->head = h->getNext();
    string str = h->getData();
    free(h);
    return str;
}
void ex4::Stack::PUSH(string & inp)
{
    lock_guard<mutex> lock(_mutex1);
    if( inp.size() > 1024)
    {
        throw(invalid_argument("ERROR: string is larger than 1025 bytes."));
    }
    str_Node * node = new str_Node(inp);
    node-> setNext(this->head);
    this->head = node;
}

string ex4::Stack::TOP()
{
    lock_guard<mutex> lock(_mutex1);
    return this->get_head()->getData();
}





