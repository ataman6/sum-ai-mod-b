
CFLAGS = -Wall -Wextra -O3 -g3 -march=native
# CFLAGS += -fsanitize=undefined

rem: rem.c tester.o
	$(CC) $(CFLAGS) -o $@ $^
tester.o: tester.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f rem tester.o

