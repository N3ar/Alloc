all: test1 test2 test3
.PHONY: all

test%: test%.c alloc.o
	gcc -ggdb -Wall -Werror $< -o $@ alloc.o

%.o: %.c %.h 
	gcc -ggdb -Wall -Werror -c $< -o $@

clean:
	rm -f *.o test1 test2 test3
.PHONY: clean
