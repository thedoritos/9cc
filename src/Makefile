CFLAGS=-std=c11 -g -static

9cc: 9cc.c

test: 9cc
		./test.sh

clean:
		rm -f 9cc *.o *~ tmp*

format:
		clang-format -i 9cc.c

.PHONY: test clean
