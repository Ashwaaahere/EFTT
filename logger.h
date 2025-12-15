#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"

/* Logging function prototypes */
int init_logger();
void log_message(const char *level, const char *format, ...);
void log_transfer(const char *client_ip, int client_port, const char *filename, 
                  size_t file_size, const char *status);
void close_logger();

/* Log levels */
#define LOG_INFO "INFO"
#define LOG_ERROR "ERROR"
#define LOG_WARNING "WARNING"
#define LOG_DEBUG "DEBUG"

#endif /* LOGGER_H */


