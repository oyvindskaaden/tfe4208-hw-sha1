CC=gcc
CFLAGS=-I.


sha_test:
	$(CC) -o sha_test test_sha.c sha1.c $(CFLAGS)

clean:
	-rm test_sha.o sha1.o
purge: clean
	-rm sha_test