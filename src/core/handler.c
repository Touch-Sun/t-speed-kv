//
// Created by lee on 2024/7/30. Handler for client requests
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../includes/log_util.h"
#include "../includes/handler.h"

// 简单的哈希表实现
#define TABLE_SIZE 256

typedef struct Entry {
    char *key;
    char *value;
    struct Entry *next;
} Entry;

Entry *table[TABLE_SIZE];

/**
 * 计算哈希值索引在哈希链表中的位置
 *
 * @param key 键
 * @return 键计算哈希值在哈希链表里面的索引位置
 */
unsigned int hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    LOG_INFO("key -> %s hashed -> %s index-> %d", key, hash, hash % TABLE_SIZE);
    return hash % TABLE_SIZE;
}

/**
 * 打印哈希表的当前状态
 */
void print_hash_table() {
    printf("哈希表:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *entry = table[i];
        if (entry) {
            printf("[%d]", i);
            while (entry) {
                printf(" -> [%s: %s]", entry->key, entry->value);
                entry = entry->next;
            }
            printf(" -> NULL\n");
        }
    }
}

/**
 * 设置键值对
 *
 * @param key 键
 * @param value 值
 */
void set_value(const char *key, const char *value) {
    unsigned int index = hash(key);
    Entry *entry = table[index];

    while (entry != NULL) {
        LOG_INFO("key is hash conflict -> %s", key);
        // hash冲突时,看冲突的节点的Key是否和当前的Key相同
        if (strcmp(entry->key, key) == 0) {
            // 相同表示同一个Key的hash冲突,更新Value即可
            // 释放原有的值
            free(entry->value);
            // 复制新的值
            entry->value = strdup(value);
            // 打印哈希表的当前状态
            // print_hash_table();
            return;
        }
        // 继续查找下一个节点 反复执行 ↑ 代码
        entry = entry->next;
    }

    // 直到为NULL了 表示 新的节点进入hash表

    // 1. 情况一: 该索引位置没有节点, 直接插入,
    // 此时新节点的[key1: value1]NEXT就是index[0]->NULL，头节点=新节点=[key1: value1]
    // 此时形成的链表就是：{table[0]: [key1: value1]} -> NULL
    // [0] -> NULL
    // [1] -> [key1: value1] -> NULL
    // [2] -> NULL

    // 2. 情况二: 该索引位置有节点 上面的循环已经计算到了某个索引位置的链表的某个位置的Entry
    // 此时新节点的NEXT就是index[1]->[key1: value1] entry->next = table[1]->[key1: value1];
    // 接下来更新hash表此索引处新的头节点 table[index] = entry; table[1] = [key3: value3]
    // 这也就导致形成此处链表为：{table[1]: [key3: value3]} -> {[key1: value1]} -> NULL
    // 再次新来一个节点, {key4: value4}, 则新节点{key4: value4}的NEXT就是index[1]->[key3: value3], 新的头节点就是{key4: value4}
    // 形成的新链表就是：{table[1]: [key4: value4]} -> {[key3: value3]} -> {[key1: value1]} -> NULL
    // [0] -> NULL
    // [1] -> [key3: value3] -> [key1: value1] -> NULL 这种情况为hash冲突
    // [2] -> [key2: value2] -> NULL

    LOG_INFO("no hash conflict -> %s", key);
    entry = malloc(sizeof(Entry));
    if (entry == NULL) {
        LOG_ERROR("Failed to allocate memory for new entry");
        return;
    }
    entry->key = strdup(key);
    entry->value = strdup(value);
    if (entry->key == NULL || entry->value == NULL) {
        LOG_ERROR("Failed to allocate memory for key or value");
        free(entry->key);
        free(entry->value);
        free(entry);
        return;
    }
    // 新节点的下一个节点指向当前链表的头节点
    entry->next = table[index];
    // 更新链表的头节点
    table[index] = entry;

    // 打印哈希表的当前状态
    // print_hash_table();
}

char* get_value(const char *key) {
    unsigned int index = hash(key);
    Entry *entry = table[index];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}

void handle_client(int client_socket) {
    char buffer[1024];
    int valread;

    while ((valread = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[valread] = '\0';
        LOG_INFO("Received from client: %s", buffer);

        char *command = strtok(buffer, " ");
        if (strcmp(command, "SET") == 0) {
            char *key = strtok(NULL, " ");
            char *value = strtok(NULL, " ");
            if (key && value) {
                set_value(key, value);
                send(client_socket, "OK\n", 3, 0);
                LOG_INFO("Set key %s to value %s", key, value);
            } else {
                send(client_socket, "ERROR\n", 6, 0);
            }
        } else if (strcmp(command, "GET") == 0) {
            char *key = strtok(NULL, " ");
            if (key) {
                char *value = get_value(key);
                if (value) {
                    send(client_socket, value, strlen(value), 0);
                    send(client_socket, "\n", 1, 0);
                } else {
                    send(client_socket, "NULL\n", 5, 0);
                }
            } else {
                send(client_socket, "ERROR\n", 6, 0);
            }
        } else {
            send(client_socket, "ERROR\n", 6, 0);
        }
    }

    // 关闭连接
    close(client_socket);
    LOG_INFO("Client disconnected");
}
