flags =-Wall -O2 -std=c2x
ldflags=

all: clean tree cache

tree: tree.o
	cc ${flags} $^ -o $@ ${ldflags}

tree.o: tree.c
	cc ${flags} -c $^

clean:
	rm -f *.o cache tree

cache: cache22.o
	cc ${flags} $^ -o $@ ${ldflags}

cache22.o: cache22.c
	cc ${flags} -c $^