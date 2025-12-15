#include "common.h"

/* Print error message and exit */
void error_exit(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

/* Create directory if it doesn't exist */
void create_directory_if_not_exists(const char *dir_path) {
    struct stat st = {0};
    if (stat(dir_path, &st) == -1) {
        if (mkdir(dir_path, 0700) == -1) {
            error_exit("Failed to create directory");
        }
        printf("Created directory: %s\n", dir_path);
    }
}

/* Create a TCP socket */
int create_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error_exit("Socket creation failed");
    }
    return sockfd;
}

/* Setup signal handlers */
void setup_signal_handlers(void (*handler)(int)) {
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
}

/* Get current timestamp as string */
char* get_timestamp() {
    static char timestamp[64];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    return timestamp;
}


