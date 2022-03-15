CFLAGS+=-g -Isrc -Iinclude
CC=gcc

test_sha: test/test_sha.c src/sha1.c
	$(CC) $(CFLAGS) test/test_sha.c src/sha1.c -o test_sha

clean: 
	-rm -f src/*.o
purge: clean
	-rm -f test_sha