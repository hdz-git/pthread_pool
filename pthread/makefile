LDFLAGS+=-pthread -o

main:main.o thread.o
	$(CC) $^ $(LDFLAGS) $@

main.o:main.c
	$(CC) $^ -c -o $@

thread.o:thread.c
	$(CC) $^ -c -o $@

all:
	vim thread.h thread.c main.c -p

clean:
	rm -r *.o



