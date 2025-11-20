#include <sys/resource.h>
#include "limits.h"
#include "utils.h"

void setup_limits(void) {
    struct rlimit rl;
    rl.rlim_cur = 64 * 1024 * 1024; 
    rl.rlim_max = 128 * 1024 * 1024;

    if (setrlimit(RLIMIT_AS, &rl) != 0)
        log_error("Failed to set memory limit.\n");
    else
        log_info("Memory limit applied.");
}
