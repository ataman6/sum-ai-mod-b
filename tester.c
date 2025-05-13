#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include <sys/time.h>

typedef int64_t i64;

void tester(i64 (*f)(i64, i64, i64), const char *name)
{
	i64 a = 234, b = 823, n = 419;
	i64 times = 10000;

	i64 ans = 0;

	struct timeval start, end;

	gettimeofday(&start, 0);
	for(int i = 0; i < times; ++i) {
		ans = f(a, b, n);
	}
	gettimeofday(&end, 0);

	i64 us = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);

	printf("%s: a=%li b=%li n=%li ans=%li times=%li takes %li.%06lis\n", name, a, b, n, ans, times, us / 1000000, us % 1000000);
}

