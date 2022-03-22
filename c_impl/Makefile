CFLAGS+=-pg -Isrc -Iinclude -Wa,--gstabs -save-temps
CC=gcc

test_sha: test/test_sha.c src/sha1.c
	$(CC) $(CFLAGS) test/test_sha.c src/sha1.c -o test_sha

profile: test_sha
	valgrind --tool=callgrind --dump-instr=yes ./test_sha

clean: 
	-rm -f src/*.o *.o *.i *.s
	-rm -f callgrind.out.*
	-rm -f gmon.out
purge: clean
	-rm -f test_sha