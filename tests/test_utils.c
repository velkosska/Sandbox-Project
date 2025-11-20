#include <assert.h>
#include <stdio.h>
#include "../include/utils.h"

int main(void) {
    printf("Running utils tests...\n");

    log_info("This is an info message test.");
    log_error("This is an error message test.");

    printf("Utils tests passed.\n");
    return 0;
}
