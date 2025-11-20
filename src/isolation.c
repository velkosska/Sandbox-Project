#include <stdio.h>
#include <unistd.h>
#include <sys/mount.h>
#include "isolation.h"
#include "utils.h"
#include <string.h>

void setup_hostname(const char *name) {
    if (sethostname(name, strlen(name)) == -1)
        log_error("Failed to set hostname.\n");
    else
        log_info("Hostname set successfully.");
}

void setup_filesystem(const char *rootfs) {
    if (chdir(rootfs) == -1) {
        log_error("Failed to change to rootfs directory.\n");
        return;
    }

    if (chroot(rootfs) == -1)
        log_error("Failed to chroot.\n");
    else
        log_info("Filesystem isolated with chroot.");
}
