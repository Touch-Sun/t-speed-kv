//
// Created by lee on 2024/7/30. 主入口文件
//

#include <stdio.h>
#include <stdlib.h>
#include "./includes/server.h"
#include "./includes/log_util.h"

int main(int argc, char *argv[]) {
    int port = 1204; // 默认端口号

    if (argc > 1) {
        port = atoi(argv[1]);
        if (port == 0) {
            LOG_INFO("Invalid port number provided, defaulting to 1204");
            port = 1204;
        } else {
            LOG_INFO("Using provided port number: %d", port);
        }
    } else {
        LOG_INFO("No port number provided, defaulting to 1204");
    }

    start_server(port);

    return 0;
}
