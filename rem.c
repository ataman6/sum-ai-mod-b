#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int64_t i64;

static inline i64 gcd(i64 a, i64 b)
{
	if(!a || !b) return a + b;

	return a > b ? gcd(a - b, b) : gcd(a, b - a);
}

static inline i64 rem2(i64 b, i64 a)
{
	i64 r = b % a;
	if(r < 0) r += a;
	return r;
}

static inline i64 div2(i64 a, i64 b)
{
	if(a >= 0) {
		return a / b;
	} else if(a >= -b) {
		return -1;
	}

	assert(0);
}

static inline i64 min_i64(i64 a, i64 b)
{
	return a < b ? a : b;
}

static inline i64 f(i64 a, i64 b, i64 n);
static inline i64 f2(i64 a, i64 b, i64 n);
static inline i64 f3(i64 a, i64 b, i64 n);
static inline i64 f4(i64 a, i64 b, i64 n);

void test(void)
{
	int nbad = 0;

	for(i64 a = 1; a < 100; ++a) {
		for(i64 b = 1; b < 200; ++b) {
			for(i64 n = 1; n < 100; ++n) {
				i64 x1 = f(a, b, n);
				i64 x2 = f2(a, b, n);

				if(x2 != x1) {
					printf("a=%lu b=%lu n=%lu exp=%lu act=%lu\n", a, b, n, x1, x2);

					if(++nbad > 10) exit(1);
				}
			}
		}
	}

	printf("all well\n");
}

void tester(i64 (*f)(i64, i64, i64), const char *name);

int main(void)
{
	test();

	tester(f, "base");
	tester(f2, "impr");

	return 0;
}

static inline i64 f(i64 a, i64 b, i64 n)
{
	i64 s = 0;
	for(i64 i = 0; i <= n; ++i) {
		s += (a * i) % b;
	}
	return s;
}

static inline i64 f2(i64 a, i64 b, i64 n)
{
	if(n == -1) return 0;
	assert(n >= 0);

	i64 g = gcd(a, b);

	a /= g;
	b /= g;

	return f3(a, b, n) * g;
}

static inline i64 f2_div(i64 a, i64 b, i64 n)
{
	i64 s = a * n * (n + 1) / 2 - f2(a, b, n);
	i64 res = s / b;

	return res;
}

static inline i64 f3(i64 a, i64 b, i64 n)
{
	return (n / b) * b * (b - 1) / 2 + f4(a % b, b, n % b);
}

static inline i64 f4_block_sum(i64 a, i64 n, i64 w, i64 h)
{
	i64 s = 0;

	i64 j_bar = n % w + 1;

	i64 c2 = n / w;

	s += h * w * (c2 * (c2 - 1) / 2);

	s += a * (j_bar * (j_bar - 1) / 2);

	s += c2 * a * (w * (w - 1) / 2);
	s += c2 * h * j_bar;

	return s;
}

static inline i64 f4(i64 a, i64 b, i64 n)
{
	if(a == 0) return 0;
	if(a == 1) {
		return n * (n + 1) / 2;
	}

	assert(gcd(a, b) == 1);

	i64 w = (b / a) + 1;
	i64 h = a - (b % a);

	if(2 * h > a) {
		h -= a;
		w -= 1;
	}

	assert(b == a * w - h);

	assert(h != 0);

	i64 abs_h = h < 0 ? -h : h;

	i64 rounds = abs_h * n / b;

	// printf("recursive %li < %li\n", abs_h, b);
	assert(abs_h < b);
	i64 s3 = f2_div(a, abs_h, rounds);
	s3 -= rounds / (abs_h / gcd(a, abs_h));

	i64 s = f4_block_sum(a, n, w, h);
	i64 base;

	if(h >= 1) {
		base = n;
	} else if(h <= -1) {
		base = n - rounds;
	}

	assert(w < b);
	// printf("recursive %li < %li\n", w, b);

	i64 s2 = 0;

	{
		i64 wh1 = (base + rounds + 1) / w;
		i64 re1 = (base + rounds + 1) % w;
		i64 cus1 = wh1 * (wh1 - 1) / 2 * w + wh1 * re1;

		i64 wh2 = (base + 1) / w;
		i64 re2 = (base + 1) % w;
		i64 cus2 = wh2 * (wh2 - 1) / 2 * w + wh2 * re2;

		s2 += cus1 - cus2;
	}

	if(h >= 1) {
		s -= (s2 - s3) * b;
	} else if(h <= -1) {
		s2 += (n - rounds) / w;

		s += (s2 - s3) * b;
	}

	return s;
}

