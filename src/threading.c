#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "threading.h"
#include "sandbox.h"
#include "utils.h"

struct sandbox_task {
    char *cmd;
};

void *sandbox_thread(void *arg) {
    struct sandbox_task *task = (struct sandbox_task *)arg;
    char *cmd_args[] = { task->cmd, NULL };

    log_info("Starting sandbox thread.");
    start_sandbox(cmd_args, 128); 

    return NULL;
}

int run_parallel(char **cmds, int count) {
    pthread_t threads[count];
    struct sandbox_task tasks[count];

    for (int i = 0; i < count; i++) {
        tasks[i].cmd = cmds[i];
        pthread_create(&threads[i], NULL, sandbox_thread, &tasks[i]);
    }

    for (int i = 0; i < count; i++) {
        pthread_join(threads[i], NULL);
    }

    log_info("All sandbox threads finished.");
    return 0;
}
