flags =-Wall -O2 -std=c2x
ldflags=

all: clean cache

clean:
	rm -f *.o cache

cache: cache22.o
	cc ${flags} $^ -o $@ ${ldflags}

cache22.o: cache22.c
	cc ${flags} -c $^