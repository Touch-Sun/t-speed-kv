//
// Created by lee on 2024/7/30. 日志工具
//

#include "../includes/log_util.h"
#include <stdarg.h>

void log_message(const char *level, const char *file, int line, const char *format, ...) {
    // 获取当前时间
    time_t now;
    time(&now);
    // 格式化时间
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

    printf("[%s] %s %s:%d: ", level, time_str, file, line);

    // 打印日志内容
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}
