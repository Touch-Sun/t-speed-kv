//
// Created by lee on 2024/7/30. 主入口文件
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./includes/server.h"
#include "./includes/client.h"
#include "./includes/log_util.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server/client> <port> [ip]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[2]);

    if (strcmp(argv[1], "server") == 0) {
        if (port == 0) {
            LOG_INFO("Invalid port number provided, defaulting to 1204");
            port = 1204;
        } else {
            LOG_INFO("Using provided port number: %d", port);
        }
        start_server(port);
    } else if (strcmp(argv[1], "client") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Usage: %s client <port> <ip>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        const char *ip = argv[3];
        start_client(ip, port);
    } else {
        fprintf(stderr, "Invalid option: %s. Use 'server' or 'client'.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
