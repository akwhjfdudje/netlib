#include "log.h"
#include <stdarg.h>
#include <time.h>

static net_log_level_t global_log_level = NET_LOG_INFO;

void net_log_set_level(net_log_level_t level) {
    global_log_level = level;
}

void _net_log(net_log_level_t level, const char *fmt, ...) {
    if (level > global_log_level || level == NET_LOG_NONE) {
        return;
    }

    const char *level_str = "INFO";
    switch (level) {
        case NET_LOG_ERROR: level_str = "ERROR"; break;
        case NET_LOG_WARN:  level_str = "WARN";  break;
        case NET_LOG_INFO:  level_str = "INFO";  break;
        case NET_LOG_DEBUG: level_str = "DEBUG"; break;
        default: break;
    }

    time_t now;
    time(&now);
    struct tm *t = localtime(&now);
    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%H:%M:%S", t);

    printf("[%s] %s: ", time_buf, level_str);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}
