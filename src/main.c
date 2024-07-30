//
// Created by lee on 2024/7/30. 主入口文件
//

#include <stdio.h>
#include <stdlib.h>
#include "./includes/server.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    start_server(port);

    return 0;
}


