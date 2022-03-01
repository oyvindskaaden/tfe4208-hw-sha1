CFLAGS+=-std=c99 -g -Isrc -Iinclude

test_sha: src/test_sha.c src/sha1.o

clean:
    -rm -f src/*.o
purge: clean
    -rm -f test_sha