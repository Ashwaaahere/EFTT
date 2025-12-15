#include "logger.h"
#include <stdarg.h>

static FILE *log_file = NULL;

/* Initialize logger */
int init_logger() {
    create_directory_if_not_exists(LOGS_DIR);
    
    log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return ERROR_FILE_IO;
    }
    
    log_message(LOG_INFO, "Logger initialized");
    return SUCCESS;
}

/* Log a message with level and format */
void log_message(const char *level, const char *format, ...) {
    if (!log_file) {
        return;
    }
    
    char *timestamp = get_timestamp();
    fprintf(log_file, "[%s] [%s] ", timestamp, level);
    
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
    
    fprintf(log_file, "\n");
    fflush(log_file);
}

/* Log file transfer details */
void log_transfer(const char *client_ip, int client_port, const char *filename, 
                  size_t file_size, const char *status) {
    if (!log_file) {
        return;
    }
    
    char *timestamp = get_timestamp();
    fprintf(log_file, "[%s] [TRANSFER] Client: %s:%d | File: %s | Size: %zu bytes | Status: %s\n",
            timestamp, client_ip, client_port, filename, file_size, status);
    fflush(log_file);
}

/* Close logger */
void close_logger() {
    if (log_file) {
        log_message(LOG_INFO, "Logger closing");
        fclose(log_file);
        log_file = NULL;
    }
}


