CFLAGS=-std=c99 -g -static

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

9cc: $(OBJS)
		$(CC) -o 9cc $(OBJS) $(LDFLAGS)

$(OBJS): 9cc.h

test: 9cc
		./test.sh

doc:
	@doxygen

clean:
	rm -f 9cc *.o *~ temp*

distclean:
	rm -rf doc

.PHONY: all