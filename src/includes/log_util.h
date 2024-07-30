#ifndef LOG_UTIL_H
#define LOG_UTIL_H

#include <stdio.h>
#include <time.h>

#define LOG_INFO(format, ...) log_message("INFO", __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) log_message("ERROR", __FILE__, __LINE__, format, ##__VA_ARGS__)

// 处理没有可变参数的情况
#define LOG_INFO(format, ...) log_message("INFO", __FILE__, __LINE__, format, ##__VA_ARGS__, NULL)
#define LOG_ERROR(format, ...) log_message("ERROR", __FILE__, __LINE__, format, ##__VA_ARGS__, NULL)

void log_message(const char *level, const char *file, int line, const char *format, ...);

#endif // LOG_UTIL_H
