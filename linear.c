#include "random_source.h"
#include <stdlib.h>
#include <errno.h>
#define CONV(ops) ((RandomSourceOperations *) (ops))
enum
{
	A = (long long) 1103515245,
	C = (long long) 12345,
	MOD = (long long) 1 << 31,
	BASE = 10
};
double linear_next(RandomSource *src) {
	src->linear_x = (A * (src->linear_x) + C) % MOD;
	return (src->x = (src->linear_x * 1.0) / (MOD * 1.0));
}
RandomSource *linear_destroy(RandomSource *src) {
	free(src->ops);
	free(src);
	return NULL;
}
RandomSource *random_linear_factory(const char *seed) {
	errno = 0;
	char *eptr = NULL;
	long long start = strtoll(seed, &eptr, BASE);
	if (errno || !eptr || *eptr || eptr == seed || start <= 0 || start != (int) start) {
		return NULL;
	}
	RandomSource *rr = calloc(1, sizeof(*rr));
	if (rr == NULL) {
		return NULL;
	}
	rr->linear_x = start;
	rr->x = (start * 1.0) / (MOD * 1.0);
	rr->ops = calloc(1, sizeof(*(CONV(rr->ops))));
	if (rr->ops == NULL) {
		free(rr);
		return NULL;
	}
	CONV(rr->ops)->next = linear_next;
	CONV(rr->ops)->destroy = linear_destroy;
	return rr;
}
