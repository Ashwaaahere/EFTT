#include "common.h"
#include "crypto.h"
#include "logger.h"

/* Global variables */
static int server_socket = -1;
static volatile sig_atomic_t running = 1;

/* Structure to pass client info to thread */
typedef struct {
    int client_socket;
    struct sockaddr_in client_addr;
} client_info_t;

/* Signal handler for graceful shutdown */
void signal_handler(int sig) {
    printf("\nReceived signal %d. Shutting down gracefully...\n", sig);
    running = 0;
    if (server_socket >= 0) {
        close(server_socket);
    }
    close_logger();
    exit(EXIT_SUCCESS);
}

/* Handle file transfer from client */
void* handle_client(void *arg) {
    client_info_t *client = (client_info_t *)arg;
    int client_socket = client->client_socket;
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client->client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client->client_addr.sin_port);
    
    printf("Client connected: %s:%d\n", client_ip, client_port);
    log_message(LOG_INFO, "Client connected: %s:%d", client_ip, client_port);
    
    /* Receive filename - read byte by byte until null terminator */
    char filename[MAX_FILENAME_LEN];
    memset(filename, 0, sizeof(filename));
    int filename_idx = 0;
    char c;
    
    while (filename_idx < MAX_FILENAME_LEN - 1) {
        ssize_t bytes_received = recv(client_socket, &c, 1, 0);
        if (bytes_received <= 0) {
            printf("Failed to receive filename from client\n");
            log_message(LOG_ERROR, "Failed to receive filename from %s:%d", client_ip, client_port);
            close(client_socket);
            free(client);
            pthread_exit(NULL);
        }
        filename[filename_idx] = c;
        if (c == '\0') {
            break;
        }
        filename_idx++;
    }
    
    printf("Receiving file: %s\n", filename);
    
    /* Receive file size - ensure we receive all bytes */
    size_t file_size = 0;
    size_t size_bytes_received = 0;
    while (size_bytes_received < sizeof(file_size)) {
        ssize_t bytes_received = recv(client_socket, ((char*)&file_size) + size_bytes_received, 
                             sizeof(file_size) - size_bytes_received, 0);
        if (bytes_received <= 0) {
            printf("Failed to receive file size\n");
            log_message(LOG_ERROR, "Failed to receive file size from %s:%d", client_ip, client_port);
            close(client_socket);
            free(client);
            pthread_exit(NULL);
        }
        size_bytes_received += bytes_received;
    }
    
    printf("File size: %zu bytes\n", file_size);
    
    /* Allocate buffer for encrypted data */
    unsigned char *encrypted_buffer = (unsigned char *)malloc(file_size);
    if (!encrypted_buffer) {
        perror("Failed to allocate memory for encrypted data");
        log_message(LOG_ERROR, "Memory allocation failed for file %s", filename);
        close(client_socket);
        free(client);
        pthread_exit(NULL);
    }
    
    /* Receive encrypted file data */
    size_t total_received = 0;
    while (total_received < file_size) {
        ssize_t bytes_received = recv(client_socket, encrypted_buffer + total_received, 
                             file_size - total_received, 0);
        if (bytes_received <= 0) {
            printf("Error receiving file data\n");
            free(encrypted_buffer);
            close(client_socket);
            free(client);
            pthread_exit(NULL);
        }
        total_received += bytes_received;
    }
    
    printf("Received %zu bytes of encrypted data\n", total_received);
    
    /* Decrypt the data */
    decrypt_buffer(encrypted_buffer, file_size, ENCRYPTION_KEY);
    
    /* Save decrypted file */
    char output_path[MAX_PATH_LEN];
    snprintf(output_path, sizeof(output_path), "%s/%s", RECEIVED_FILES_DIR, filename);
    
    FILE *output_file = fopen(output_path, "wb");
    if (!output_file) {
        perror("Failed to create output file");
        log_message(LOG_ERROR, "Failed to create output file: %s", output_path);
        free(encrypted_buffer);
        close(client_socket);
        free(client);
        pthread_exit(NULL);
    }
    
    size_t bytes_written = fwrite(encrypted_buffer, 1, file_size, output_file);
    fclose(output_file);
    free(encrypted_buffer);
    
    if (bytes_written != file_size) {
        printf("Warning: File size mismatch (expected %zu, wrote %zu)\n", file_size, bytes_written);
    }
    
    printf("File saved successfully: %s\n", output_path);
    log_transfer(client_ip, client_port, filename, file_size, "SUCCESS");
    
    /* Send acknowledgment */
    const char *ack = "File received successfully";
    send(client_socket, ack, strlen(ack), 0);
    
    close(client_socket);
    free(client);
    printf("Client %s:%d disconnected\n", client_ip, client_port);
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;
    
    /* Setup signal handlers */
    setup_signal_handlers(signal_handler);
    
    /* Initialize logger */
    if (init_logger() != SUCCESS) {
        fprintf(stderr, "Failed to initialize logger\n");
        return EXIT_FAILURE;
    }
    
    /* Create directories */
    create_directory_if_not_exists(RECEIVED_FILES_DIR);
    create_directory_if_not_exists(LOGS_DIR);
    
    /* Create server socket */
    server_socket = create_socket();
    
    /* Set socket options for reuse */
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        error_exit("setsockopt failed");
    }
    
    /* Setup server address */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    /* Bind socket */
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error_exit("Bind failed");
    }
    
    /* Listen for connections */
    if (listen(server_socket, 5) < 0) {
        error_exit("Listen failed");
    }
    
    printf("EFTT Server started on port %d\n", port);
    printf("Waiting for client connections...\n");
    log_message(LOG_INFO, "Server started on port %d", port);
    
    /* Accept connections in loop */
    while (running) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            if (running) {
                perror("Accept failed");
            }
            continue;
        }
        
        /* Create thread for client */
        pthread_t thread_id;
        client_info_t *client = (client_info_t *)malloc(sizeof(client_info_t));
        if (!client) {
            perror("Failed to allocate memory for client info");
            close(client_socket);
            continue;
        }
        
        client->client_socket = client_socket;
        client->client_addr = client_addr;
        
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client) != 0) {
            perror("Failed to create thread");
            free(client);
            close(client_socket);
            continue;
        }
        
        /* Detach thread so it cleans up automatically */
        pthread_detach(thread_id);
    }
    
    close(server_socket);
    close_logger();
    printf("Server shutdown complete\n");
    
    return EXIT_SUCCESS;
}


