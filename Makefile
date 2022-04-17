
CC = g++
CFLAGS = -Wall -g -std=gnu99 -pthread
DEPS = Stack.hpp
OBJECTS = Server.o Stack.o
OBJECTS1 = Client.o Stack.o
OBJECTS_MAIN = main.o Stack.o

main : $(OBJECTS_MAIN)
	$(CC) -o $@ $(OBJECTS1) $(CFLAGS)

myServer: $(OBJECTS) 
	$(CC) -o $@ $^ $(CFLAGS)

Client: $(OBJECTS1)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean: 
	rm -f myServer Client myServer main *.o


