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
        // 1. 定义客户端地址结构体
        struct sockaddr_in client_addr;
        // 2. 定义结构体的长度
        socklen_t client_addr_len = sizeof(client_addr);
        // 3. 获取客户端(对端)地址信息存储在client_addr中
        getpeername(new_socket, (struct sockaddr*)&client_addr, &client_addr_len);

        // 将客户端IP地址和端口号转换为字符串
        char client_ip[INET_ADDRSTRLEN];
        // 1. 将二进制的ip地址形式转化为字符串形式
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        // 2. 获取端口号 使用字节序转换函数ntohs 从网络字节序转换为主机字节序
        // 2.1 传输时统一使用网络字节序，但是在接收时机器不同，架构不同，字节序不同，需要转换
        int client_port = ntohs(client_addr.sin_port);

        LOG_INFO("New client connected: %s:%d", client_ip, client_port);

        // 处理客户端请求
        char buffer[1024] = {0};
        int valread;
        while ((valread = read(new_socket, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[valread] = '\0';
            LOG_INFO("Received from client: %s", buffer);
            // 原样返回给客户端
            send(new_socket, buffer, strlen(buffer), 0);
            LOG_INFO("Echoed back to client: %s", buffer);
        }

        // 关闭连接
        close(new_socket);
        LOG_INFO("Client disconnected: %s:%d", client_ip, client_port);
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
