
CC = g++
CFLAGS = -Wall -g -std=gnu99 -pthread
DEPS = heap_funcs.hpp dectest.h Stack.hpp
OBJECTS = Server.o Stack.o
OBJECTS1 = Client.o Stack.o
OBJECTS_TEST_Server = Stack.o Server.o heap_funcs.o Test.o
OBJECTS_TEST_Client = Stack.o Client.o Test.o
OBJECTS_TEST_HEAP = heap_funcs.o Stack.o Test.o
OBJECTS_MAIN = main.o Stack.o



main : $(OBJECTS_MAIN)
	$(CC) -o $@ $(OBJECTS1) $(CFLAGS)

testServer : $(OBJECTS_TEST_Server)
	$(CC) -o $@ $(OBJECTS_TEST_Server) $(CFLAGS)

# add testClient

# add testHeap

myServer: $(OBJECTS) 
	$(CC) -o $@ $^ $(CFLAGS)

Client: $(OBJECTS1)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean: 
	rm -f myServer Client myServer main *.o


