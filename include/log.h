#ifndef NETLIB_LOG_H
#define NETLIB_LOG_H

#include <stdio.h>

typedef enum {
    NET_LOG_NONE = 0,
    NET_LOG_ERROR,
    NET_LOG_WARN,
    NET_LOG_INFO,
    NET_LOG_DEBUG
} net_log_level_t;

/**
 * @brief Sets the global logging level.
 * 
 * @param level The desired log level.
 */
void net_log_set_level(net_log_level_t level);

/**
 * @brief Internal logging function. Use macros instead.
 */
void _net_log(net_log_level_t level, const char *fmt, ...);

#define NET_LOG_E(...) _net_log(NET_LOG_ERROR, __VA_ARGS__)
#define NET_LOG_W(...) _net_log(NET_LOG_WARN, __VA_ARGS__)
#define NET_LOG_I(...) _net_log(NET_LOG_INFO, __VA_ARGS__)
#define NET_LOG_D(...) _net_log(NET_LOG_DEBUG, __VA_ARGS__)

#endif
