//
// Created by lee on 2024/7/30. 客户端源文件
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../includes/client.h"
#include "../includes/log_util.h"

void start_client(const char *ip, int port) {
    int client_fd;
    struct sockaddr_in server_address;
    char buffer[1024];

    // 创建客户端套接字
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR("Client socket creation failed, please try again");
        exit(EXIT_FAILURE);
    }
    LOG_INFO("Client socket created successfully");

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    // 将IP地址转换为二进制形式
    if (inet_pton(AF_INET, ip, &server_address.sin_addr) <= 0) {
        LOG_ERROR("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // 连接到服务器
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        LOG_ERROR("Connection to the server failed");
        exit(EXIT_FAILURE);
    }
    LOG_INFO("Connected to the server at %s:%d", ip, port);

    // 进入交互式命令行环境
    while (1) {
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);

        // 去除末尾的换行符
        buffer[strcspn(buffer, "\n")] = '\0';

        // 如果用户输入了 'exit'，则退出循环
        if (strcmp(buffer, "exit") == 0) {
            LOG_INFO("Exiting client");
            break;
        }

        // 发送用户输入的命令到服务器
        send(client_fd, buffer, strlen(buffer), 0);
        LOG_INFO("Message sent to the server: %s", buffer);

        // 清空缓冲区并接收服务器的响应
        memset(buffer, 0, sizeof(buffer));
        int valread = read(client_fd, buffer, sizeof(buffer) - 1);
        if (valread > 0) {
            LOG_INFO("Message received from server: %s", buffer);
            printf("%s\n", buffer);
        }
    }

    // 关闭套接字
    close(client_fd);
    LOG_INFO("Client socket closed");
}
