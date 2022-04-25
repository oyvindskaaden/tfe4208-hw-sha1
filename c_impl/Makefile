CFLAGS+=-Isrc -Iinclude
CFLAGSDBG+=-pg -Wa,--gstabs -save-temps
CFLAGSVALGRIND+=-DVALGRIND=callgrind

CC=gcc

test_sha: test/test_sha.c src/sha1.c
	$(CC) $(CFLAGS) test/test_sha.c src/sha1.c -o test_sha

debug:
	$(CC) $(CFLAGS) $(CFLAGSDBG) test/test_sha.c src/sha1.c -o test_sha

profile:
	$(CC) $(CFLAGS) $(CFLAGSDBG) $(CFLAGSVALGRIND) test/test_sha.c src/sha1.c -o test_sha
	valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes --collect-atstart=no --instr-atstart=no ./test_sha
# valgrind --tool=callgrind --dump-instr=yes 

perf: test_sha
	perf record -g -F 999 ./test_sha 1
	perf report

clean: 
	-rm -f src/*.o *.o *.i *.s
	-rm -f callgrind.out.*
	-rm -f gmon.out
purge: clean
	-rm -f test_sha