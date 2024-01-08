CFLAGS=-std=c99 -g -static

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

Oop: $(OBJS)
		$(CC) -o Oop $(OBJS) $(LDFLAGS)

$(OBJS): encapsulation.h

clean:
	rm -f Oop *.o *~ temp* 

.PHONY: all