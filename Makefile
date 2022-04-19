# compiler
CC = g++
# paths
SOURCE_STACK_PATH = sources/Stack
SOURCE_HEAP_PATH = sources/Heap
SOURCE_SERVER_PATH = sources/Server
SOURCE_CLIENT_PATH = sources/Client
OBJECT_PATH = objects/
# wildcards for .cpp files inside sources folder
# generate all the .cpp files for the stack folder inside sources
SOURCES_STACK = $(wildcard $(SOURCE_STACK_PATH)/*.cpp)
# generate all the .cpp for the heap folder inside sources
SOURCES_HEAP = $(wildcard $(SOURCE_HEAP_PATH)/*.cpp)
# generate all the .cpp for the server folder inside sources
SOURCES_SERVER = $(wildcard $(SOURCE_SERVER_PATH)/*.cpp)
# generate all the .cpp for the client folder inside sources
SOURCES_CLIENT = $(wildcard $(SOURCE_CLIENT_PATH)/*.cpp)
# wildcards for hpp files
# generate all the .hpp files for the stack folder inside sources
HEADERS_STACK = $(wildcard $(SOURCE_STACK_PATH)/*.hpp)
# generate all the .hpp files for the heap folder inside sources
HEADERS_HEAP = $(wildcard $(SOURCE_HEAP_PATH)/*.hpp)
# generate all the .hpp files for the server folder inside sources
HEADERS_SERVER = $(wildcard $(SOURCE_SERVER_PATH)/*.hpp)
# generate all the .hpp files for the client folder inside sources
HEADERS_CLIENT = $(wildcard $(SOURCE_CLIENT_PATH)/*.hpp)
# set up Object files
# set up .o files to link by swapping each .cpp to .o
# then swapping the folder location to objects for the destination of the object file
# take the file name from the stack .cpp files and change their ending to .o and then change the folder to be objects folder
OBJECTS_STACK = $(subst sources/Stack,objects/,$(subst .cpp,.o,$(SOURCES_STACK)))
# take the file name from the heap .cpp files and change their ending to .o and then change the folder to be objects folder
OBJECTS_HEAP = $(subst sources/Heap,objects/,$(subst .cpp,.o,$(SOURCES_HEAP)))
# take the file name from the server .cpp files and change their ending to .o and then change the folder to be objects folder
OBJECTS_SERVER = $(subst sources/Server,objects/,$(subst .cpp,.o,$(SOURCES_SERVER)))
# take the file name from the client .cpp files and change their ending to .o and then change the folder to be objects folder
OBJECTS_CLIENT = $(subst sources/Client,objects/,$(subst .cpp,.o,$(SOURCES_CLIENT)))
# flags
CFLAGS = -Wall -g -pthread $(SOURCE_PATH)
# headers
DEPS = heap_funcs.hpp dectest.h Stack.hpp Client.hpp Server.hpp

Server: $(OBJECTS_SERVER) $(OBJECTS_STACK)
	$(CC) $(CFLAGS) $^ -o $@

Client: $(OBJECTS_CLIENT)
	$(CC) $(CFLAGS) $^ -o $@

testServer : TestServer.o TestRunner.o
	$(CC) $(CFLAGS) $^ -o $@ 

testClient : TestRunner.o TestClient.o
	$(CC) $(CFLAGS) $^ -o $@ 

testHeap : TestRunner.o TestHeap.o $(OBJECTS_HEAP)
	$(CC) $(CFLAGS) $^ -o $@ 

%.o: %.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJECTS_STACK): $(SOURCES_STACK) $(HEADERS_STACK)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS_HEAP): $(SOURCES_HEAP) $(HEADERS_HEAP)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJECTS_SERVER): $(SOURCES_SERVER) $(HEADERS_SERVER)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS_CLIENT): $(SOURCES_CLIENT) $(HEADERS_CLIENT)
	$(CC) $(CFLAGS) -c $< -o $@
.PHONY: clean

clean: 
	rm -f $(OBJECTS_CLIENT) $(OBJECTS_HEAP) $(OBJECTS_SERVER) $(OBJECTS_STACK) Server Client testHeap testServer testClient *.o


