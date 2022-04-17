
CC = g++
CFLAGS = -Wall -g -std=gnu99 -pthread
OBJECTS = myServer.o
OBJECTS2 = originServer.o
OBJECTS3 = originClient.o

myServer: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

originServer: $(OBJECTS2)
	$(CC) -o $@ $^ $(CFLAGS)

originClient: $(OBJECTS3)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean: 
	rm -f myServer originClient originServer *.o


