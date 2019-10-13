LDFLAGS+=-pthread -o

main:main.o pthreadpool.o mylist.o
	$(CC) $^ $(LDFLAGS) $@

main.o:main.c
	$(CC) $^ -c -o $@

pthreadpool.o:pthreadpool.c
	$(CC) $^ -c -o $@

mylist.o:mylist.c
	$(CC) $^ -c -o $@



all:
	vim mylist.h mylist.c pthreadpool.h pthreadpool.c main.c -p

clean:
	rm -r *.o



