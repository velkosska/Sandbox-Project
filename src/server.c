#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include "server.h"
#include "sandbox.h"
#include "utils.h"

#define PORT 8080

volatile sig_atomic_t server_running = 1;

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);

    char buffer[256] = {0};
    read(client_fd, buffer, sizeof(buffer));
    log_info("Received client connection.");

    if (strncmp(buffer, "RUN", 3) == 0) {
        char *cmd = strtok(buffer + 4, "\n ");
        if (cmd) {
            log_info("Starting sandbox for client command.");
            char *args[] = { cmd, NULL };
            start_sandbox(args, 128);
        } else {
            log_error("Invalid RUN command format.");
        }
    } else {
        log_error("Unknown command received from client.");
    }

    close(client_fd);
    return NULL;
}

int start_server() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return -1;
    }

    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return -1;
    }

    log_info("Server listening on port 8080");

    
    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    while (server_running) {
        int *client_fd = malloc(sizeof(int));
        if (!client_fd) continue;

        *client_fd = accept(server_fd, NULL, NULL);
        if (*client_fd < 0) {
            
            usleep(100000); // 0.1s
            free(client_fd);
            continue;
        }

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, client_fd);
        pthread_detach(thread);
    }

    log_info("Server shutting down gracefully...");
    close(server_fd);
    return 0;
}
