#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>

/* Default configuration */
#define DEFAULT_PORT 8080
#define DEFAULT_BUFFER_SIZE 4096
#define MAX_FILENAME_LEN 256
#define MAX_PATH_LEN 512
#define ENCRYPTION_KEY 0xAA  // Simple XOR key (can be enhanced)

/* Protocol constants */
#define MAX_PACKET_SIZE 4096
#define HEADER_SIZE 256

/* Error codes */
#define SUCCESS 0
#define ERROR_SOCKET -1
#define ERROR_BIND -2
#define ERROR_LISTEN -3
#define ERROR_ACCEPT -4
#define ERROR_CONNECT -5
#define ERROR_FILE_IO -6
#define ERROR_MEMORY -7
#define ERROR_THREAD -8

/* Directory paths */
#define RECEIVED_FILES_DIR "received_files"
#define LOGS_DIR "logs"
#define LOG_FILE "logs/transfer.log"

/* Function prototypes */
void error_exit(const char *message);
void create_directory_if_not_exists(const char *dir_path);
int create_socket();
void setup_signal_handlers(void (*handler)(int));
char* get_timestamp();

#endif /* COMMON_H */


