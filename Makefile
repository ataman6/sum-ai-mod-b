
CFLAGS = -Wall -Wextra -Werror -O3 -fsanitize=undefined -g3

rem: rem.c tester.o
	$(CC) $(CFLAGS) -o $@ $^
tester.o: tester.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f rem tester.o

