#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sandbox.h"
#include "utils.h"
#include "threading.h"
#include "server.h"

extern volatile sig_atomic_t server_running;

static void print_help(void) {
    printf("Usage: sandbox [command] [options]\n\n");
    printf("Commands:\n");
    printf("  run [--memory <MB>] <command>      Run a command in an isolated sandbox\n");
    printf("  run-parallel <cmd1> <cmd2> ...     Run multiple sandboxes concurrently\n");
    printf("  server                             Start a TCP control server (port 8080)\n");
    printf("  help                               Show this help message\n");
}

void handle_sigint(int sig) {
    (void)sig;
    log_info("Graceful shutdown requested (SIGINT).");
    server_running = 0;
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

        
        optind = 2; 
        while ((opt = getopt_long(argc, argv, "m:", long_options, NULL)) != -1) {
            switch (opt) {
                case 'm':
                    memory_limit = atoi(optarg);
                    break;
                default:
                    print_help();
                    return EXIT_FAILURE;
            }
        }

        if (optind >= argc) {
            log_error("No command provided for sandbox run.\n");
            return EXIT_FAILURE;
        }

        char **cmd = &argv[optind];
        log_info("Launching sandbox...");
        start_sandbox(cmd, memory_limit);
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "run-parallel") == 0) {
        if (argc < 3) {
            log_error("Usage: sandbox run-parallel <cmd1> <cmd2> ...");
            return EXIT_FAILURE;
        }
        run_parallel(&argv[2], argc - 2);
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "server") == 0) {
        signal(SIGINT, handle_sigint);
        log_info("Starting sandbox server. Press Ctrl+C to stop.");

        start_server();  

        log_info("Server stopped.");
        return EXIT_SUCCESS;
    }

    print_help();
    return EXIT_SUCCESS;
}
