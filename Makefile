CC=gcc

all: hash.o main.o 	
	$(CC) hash.o main.o -o tst_hash

main.o: main.c
	$(CC) -c -g main.c

hash.o: hash.c
	$(CC) -c -g hash.c

clean:
	rm *.o tst_hash
