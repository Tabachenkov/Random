#include "random_source.h"
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#define CONV(ops) ((RandomSourceOperations *) (ops))
double
random_next(RandomSource *src)
{
    unsigned long long num;
    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, &num, sizeof(num));
    close(fd);
    double new = num ? (1.0 * (num - 1)) / (1.0 * ULLONG_MAX) : 0.0;
    return (src->x = new);
}
RandomSource *
random_destroy(RandomSource *src)
{
    free(src->ops);
    free(src);
    return NULL;
}
RandomSource *
random_random_factory(void)
{
    RandomSource *rr = calloc(1, sizeof(*rr));
    if (rr == NULL) {
        return NULL;
    }
    rr->x = 0.0;
    rr->linear_x = 0;
    rr->ops = calloc(1, sizeof(*(CONV(rr->ops))));
    if (rr->ops == NULL) {
        free(rr);
        return NULL;
    }
    CONV(rr->ops)->next = random_next;
    CONV(rr->ops)->destroy = random_destroy;
    return rr;
}
