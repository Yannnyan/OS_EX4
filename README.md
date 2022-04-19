

# Description
## What we've done
In this excersice we implement a multi threaded and thread safe stack. 
</br> Also some standard functions as to manage the heap memory: malloc (calloc) function, free function.  
</br> In addition we've implemented a client server model, such that the server the stack we've implemented as an object.
## How the Server works
 The server listens to connections from multiple clients and opens a new connection and a new thread for each client and recvs information from the socket file descriptor associated with the client, untill the client closes connection. 
 </br> Then each thread handles calls from the client. 
 </br> The server receives instructions to ```POP```, ```PUSH``` and ```TOP``` at the Stack. 
 </br> The main purpose of the project is to make the stack thread safe such that the instructions and information is handled as expected. 
## How the client works
The client's main function receives as arguments the instructions to pass to the server, and passes it one by one. 
</br> Once the client finishes all the instructions, the client ends the connection to the server and the Server's thread dies afterwards.
# Notes
The first argument for the binary file of Client should be the local address : ``` 127.0.0.1 ```
</br> all other arguments for the client except for 0  and arg 1 should be the instructions for the stack. such as ```PUSH hello world ```, or ```POP```, or ```TOP``` .

# How to run
First open terminal and change directory into **OS_EX4** wherever it is located at.</br>
## Compilation:
Makefile targets: </br> 
General syntax:  ```make <target> ```</br>
``` Server ``` - To compile the Server to a binary,
</br>  ```Client``` - To compile the Client to a binary (the Client is the user interface),
</br>  ```testServer``` - To compile the Server test,
</br>  ```testHeap``` - To compile the tests for the malloc calloc and free functions, </br> ```testClient``` - to compile the Client tests,
</br>  ```clean``` - to remove all the binary files and the object files

## User interface
<ins>Compilation requirement:</ins> first compile with ```make Client``` && ``` make Server.```</br>
<ins>Enviroment:</ins> Open one process for the **Server**, (preferably on the terminal), and another process or more :smile: for the **Client** (preferably on the terminal). 
</br>
<ins>Syntax for the Client main function arguments:</ins>  ``` argv[0] = "Client", argv[1] = "127.0.0.1", argv[n >= 2] = "<command> <options>"``` . 
</br> <ins>Syntax for the available Client commands:</ins> 
</br> **Description**: the Client supposed to enter commands to alter the stack inside the server.</br>```PUSH <some string> ``` - pushed some string into the stack, 
</br> ``` POP ``` - pops the first string out of the stack, 
</br> ``` TOP ``` - peeks at the first string of the stack.

# Visual

## for devs
[malloc](https://man7.org/linux/man-pages/man3/free.3.html) [sbrk](https://man7.org/linux/man-pages/man2/sbrk.2.html)  </br> [free](https://man7.org/linux/man-pages/man3/free.3.html) [brk](https://man7.org/linux/man-pages/man2/sbrk.2.html) </br> [getrlimit](https://man7.org/linux/man-pages/man2/getrlimit.2.html) [setrlimit](https://man7.org/linux/man-pages/man2/getrlimit.2.html)




