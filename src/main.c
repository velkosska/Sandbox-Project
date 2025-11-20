#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "sandbox.h"
#include "utils.h"

static void print_help(void) {
    printf("Usage: sandbox [command] [options]\n\n");
    printf("Commands:\n");
    printf("  run [--memory <MB>] <command>  Run a command in an isolated sandbox\n");
    printf("  help                           Show this help message\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "help") == 0) {
        print_help();
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "run") == 0) {
        int memory_limit = 0;
        int opt;
        static struct option long_options[] = {
            {"memory", required_argument, 0, 'm'},
            {0, 0, 0, 0}
        };

        // Parse options
        while ((opt = getopt_long(argc - 1, argv + 1, "m:", long_options, NULL)) != -1) {
            switch (opt) {
                case 'm':
                    memory_limit = atoi(optarg);
                    break;
                default:
                    print_help();
                    return EXIT_FAILURE;
            }
        }

        if (optind >= argc - 1) {
            log_error("No command provided for sandbox run.\n");
            return EXIT_FAILURE;
        }

        char **cmd = &argv[optind + 1];
        log_info("Launching sandbox...");
        start_sandbox(cmd, memory_limit);
        return EXIT_SUCCESS;
    }

    print_help();
    return EXIT_SUCCESS;
}
