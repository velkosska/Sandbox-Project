#include <assert.h>
#include <stdio.h>
#include <sys/resource.h>
#include "../include/limits.h"

int main(void) {
    printf("Running limits tests...\n");

    setup_limits();

    struct rlimit rl;
    getrlimit(RLIMIT_AS, &rl);

    assert(rl.rlim_cur <= rl.rlim_max);

    printf("Limits tests passed.\n");
    return 0;
}
