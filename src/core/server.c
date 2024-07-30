//
// Created by lee on 2024/7/30. 服务端源文件
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../includes/server.h"
#include "../includes/log_util.h"

void start_server(int port) {
    // 定义服务端套接字描述符，新连接套接字描述符 创建套接字后返回的文件描述符 用于标识这个套接字
    int server_fd, new_socket;
    // 定义sockaddr_in结构体，用于存储地址信息
    struct sockaddr_in address;
    int opt = 1;
    // 地址长度
    int addrlen = sizeof(address);

    // 创建socket文件描述符 AF_INET: IPv4协议族 SOCK_STREAM: 流式套接字 0: 默认协议: TCP协议
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        LOG_ERROR("Server socket creation failed, please try again");
        exit(EXIT_FAILURE);
    }
    LOG_INFO("Server socket created successfully");

    // 强制将端口与socket绑定 SOL_SOCKET: 套接字级别 SO_REUSEADDR: 允许重用本地地址和端口, 存在程序在此地址此端口上也可以绑定
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        LOG_ERROR("Server setsockopt failed, please try again");
        exit(EXIT_FAILURE);
    }
    LOG_INFO("Server setsockopt succeeded");

    // 设置地址族为 IP protocol family.
    address.sin_family = AF_INET;
    // 设置接受任何本地IP连接
    address.sin_addr.s_addr = INADDR_ANY;
    // 设置端口号
    address.sin_port = htons(port);

    // 绑定socket到指定端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        LOG_ERROR("Server bind failed, please try again");
        exit(EXIT_FAILURE);
    }
    LOG_INFO("Server bind succeeded");

    // 监听端口，最大挂起连接数为3
    if (listen(server_fd, 3) < 0) {
        LOG_ERROR("Server listen failed, please try again");
        exit(EXIT_FAILURE);
    }
    LOG_INFO("Server listening on port %d", port);

    // 循环等待客户端连接
    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) >= 0) {
        LOG_INFO("New client connected");
        // TODO: 处理客户端请求
    }

    // 如果accept返回值小于0表示连接失败
    if (new_socket < 0) {
        LOG_ERROR("Server accept client connection failed, please try again");
        exit(EXIT_FAILURE);
    }

    // 关闭套接字
    close(server_fd);
    LOG_INFO("Server socket closed");
}
