CC=gcc
CFLAGS = -I.

DEPS = 
OBJS = map.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJS)
	$(CC) -g -o $@ $^ $(CFLAGS) -lncurses

clean:
	rm -f test *.o
