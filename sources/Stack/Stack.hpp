
#include <iostream>
#include <string>


using namespace std;


namespace ex4
{
    // This class is a node inside a linked list of strings.
    class str_Node
    {
        private:
            string data;
            str_Node * next;
            // functions
            str_Node * getNext1()
            {
                return this->next;
            }
            
        public:
            // construct an empty node and set the next to null
            str_Node();
            
            // construct a node and set its data to str
            // set next to null.
            str_Node(const string & str);
            
            // get the next node.
            str_Node * getNext(); 
            // set the next node.
            void setNext(string & next); 
            
            void setNext(str_Node * next);
            // get the data of the node
            string & getData();
            // set the data of the node
            void setData(string data);
    };
    class Stack
    {
        private:
            str_Node * head;
            size_t size;

            string TOP1();
            string POP1();

        public:
            // constructor initializes stack size to 10
            Stack();
            // destructor, destroys the array
            ~Stack();
            // push text into the stack
            void PUSH(string text); 
            // pop text from the stack
            string POP(); 
            // return the first string from the stack 
            // - The only command that generates "OUTPUT: " 
            string TOP();

            // operators
            // outputs the Stack to ostream for printing purposes.
            friend ostream & operator<<(ostream & os, Stack stack);
            // getters and setters
            // get the size of the stack
            size_t get_size(){return this->size;}
            // get pointer to the head of the stack
            str_Node * get_head(){return this->head;}
            
    };
}


