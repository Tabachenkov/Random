#include "random_source.h"
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CONV(ops) ((RandomSourceOperations *) (ops))
enum
{
    BASE = 10,
    MAX = 100000
};
int
main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Few parameters\n");
        return 1;
    }
    void *handle = dlopen(NULL, RTLD_LAZY);
    RandomSource *rr = NULL;
    if (!strcmp(argv[1], "random")) {
        RandomSource *(*f_factory)(void) = dlsym(handle, "random_random_factory");
        rr = f_factory();
    } else if (!strcmp(argv[1], "linear")) {
        if (argc < 4) {
            fprintf(stderr, "No seed\n");
            dlclose(handle);
            return 1;
        }
        RandomSource *(*f_factory)(const char *) = dlsym(handle, "random_linear_factory");
        rr = f_factory(argv[3]);
    } else {
        fprintf(stderr, "Wrong type of generator\n");
        dlclose(handle);
        return 1;
    }
    if (rr == NULL) {
        fprintf(stderr, "random_factory error\n");
        dlclose(handle);
        return 1;
    }
    errno = 0;
    char *eptr = NULL;
    long i, num_l = strtol(argv[2], &eptr, BASE);
    if (errno || !eptr || *eptr || eptr == argv[2] || num_l < 0 || num_l > MAX || (int) num_l != num_l) {
        fprintf(stderr, "Wrong number\n");
        rr = CONV(rr->ops)->destroy(rr);
        dlclose(handle);
        return 1;
    }
    for (i = 0; i < num_l; i++) {
        printf("%.10g\n", CONV(rr->ops)->next(rr));
    }
    rr = CONV(rr->ops)->destroy(rr);
    dlclose(handle);
    return 0;
}
