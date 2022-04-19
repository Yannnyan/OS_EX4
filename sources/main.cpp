

#include "Stack.hpp"
#include <string>
#include <iostream>

void handleCall( string & str);
using namespace std;

ex4::Stack stack;
int main()
{
    
    string str;
    // cin divides input by space
    while (1)
    {
        cin >> str;
        handleCall(str);
        str.clear();
    }
    return 0;

}

void handleCall(string & str)
{
    cout << str << endl;
    if (str.at(0) == 'p')
    {
        stack.PUSH(str);
        cout << "pushed into stack" << endl;
    }
    else if (str.at(0) == 't')
    {
        cout << stack.TOP() << endl;
    }
    else if (str.at(0) == 'P')
    {
        cout << stack.POP() << endl;
    }
}

