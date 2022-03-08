CFLAGS+=-g -Isrc -Iinclude
CC=gcc

test_sha: src/test_sha.c src/sha1.c
	$(CC) $(CFLAGS) src/test_sha.c src/sha1.c -o test_sha

clean: 
	-rm -f src/*.o
purge: clean
	-rm -f test_sha