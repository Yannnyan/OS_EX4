
CC = g++
CFLAGS = -Wall -g -std=gnu99 -pthread 



%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean

clean:
	rm *.o




