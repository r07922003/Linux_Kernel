CFLAGS=-std=c99 -g -static

all: server client

server: server.c 
client: client.c

test:
		./test.sh
clean:
	rm -f server client *.o

.PHONY: test clean
