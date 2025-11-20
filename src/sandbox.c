#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sched.h>
#include <unistd.h>
#include "sandbox.h"
#include "isolation.h"
#include "limits.h"
#include "utils.h"
#include <string.h>
#include <sys/mount.h>
#include <sys/wait.h>

#define STACK_SIZE (1024 * 1024)

static int child_fn(void *arg) {
    log_info("Inside sandboxed process.");

    setup_hostname("sandbox");
    setup_filesystem("../rootfs");

    if (mount("proc", "/proc", "proc", 0, "") == -1)
        perror("mount /proc");
    else
        log_info("/proc mounted inside sandbox.");

    setup_limits();

    char **cmd = (char **)arg;
    pid_t pid = fork();

    if (pid == 0) {
        execvp(cmd[0], cmd);
        perror("execvp");
        _exit(1);
    } else if (pid > 0) {
        waitpid(pid, NULL, 0);

        if (umount("/proc") == -1)
            perror("umount /proc");
        else
            log_info("/proc unmounted cleanly.");

        log_info("Sandboxed command finished.");
    } else {
        perror("fork");
    }

    return 0;
}

int start_sandbox(char **cmd, int memory_limit) {
    (void)memory_limit;  // Prevent unused variable warning for now
    char *stack = malloc(STACK_SIZE);
    if (!stack) {
        log_error("Failed to allocate stack.\n");
        return -1;
    }

    char *stack_top = stack + STACK_SIZE;

    log_info("Creating isolated container process...");
    pid_t pid = clone(child_fn, stack_top,
                      CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD,
                      cmd);

    if (pid < 0) {
        perror("clone");
        free(stack);
        return -1;
    }

    waitpid(pid, NULL, 0);
    log_info("Sandboxed process exited.");
    free(stack);
    return 0;
}
