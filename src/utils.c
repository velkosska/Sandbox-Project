#include <stdio.h>
#include <time.h>
#include "utils.h"

static void timestamp(void) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    printf("[%02d:%02d:%02d] ", tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void log_info(const char *msg) {
    timestamp();
    printf("[INFO] %s\n", msg);
}

void log_error(const char *msg) {
    timestamp();
    fprintf(stderr, "[ERROR] %s\n", msg);
}
