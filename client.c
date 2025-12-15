#include "common.h"
#include "crypto.h"

/* Signal handler for graceful shutdown */
void signal_handler(int sig) {
    printf("\nReceived signal %d. Closing connection...\n", sig);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port> [file_path]\n", argv[0]);
        fprintf(stderr, "Example: %s localhost 8080 ./myfile.txt\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    const char *file_path = (argc == 4) ? argv[3] : "test.txt";
    
    /* Setup signal handlers */
    setup_signal_handlers(signal_handler);
    
    /* Validate file path */
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }
    
    /* Get file size */
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size < 0) {
        fprintf(stderr, "Failed to determine file size\n");
        fclose(file);
        return EXIT_FAILURE;
    }
    
    printf("File: %s\n", file_path);
    printf("Size: %ld bytes\n", file_size);
    
    /* Extract filename from path */
    const char *filename = strrchr(file_path, '/');
    if (!filename) {
        filename = strrchr(file_path, '\\');
    }
    filename = filename ? filename + 1 : file_path;
    printf("Filename: %s\n", filename);
    
    /* Read file into memory */
    unsigned char *file_data = (unsigned char *)malloc(file_size);
    if (!file_data) {
        perror("Failed to allocate memory for file data");
        fclose(file);
        return EXIT_FAILURE;
    }
    
    size_t bytes_read = fread(file_data, 1, file_size, file);
    fclose(file);
    
    if (bytes_read != (size_t)file_size) {
        fprintf(stderr, "Failed to read entire file (read %zu of %ld bytes)\n", bytes_read, file_size);
        free(file_data);
        return EXIT_FAILURE;
    }
    
    printf("File read successfully\n");
    
    /* Encrypt file data */
    printf("Encrypting file...\n");
    encrypt_buffer(file_data, file_size, ENCRYPTION_KEY);
    printf("File encrypted\n");
    
    /* Create socket */
    int client_socket = create_socket();
    
    /* Setup server address */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid server IP address: %s\n", server_ip);
        free(file_data);
        close(client_socket);
        return EXIT_FAILURE;
    }
    
    /* Connect to server */
    printf("Connecting to server %s:%d...\n", server_ip, server_port);
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        free(file_data);
        close(client_socket);
        return EXIT_FAILURE;
    }
    
    printf("Connected to server\n");
    
    /* Send filename */
    if (send(client_socket, filename, strlen(filename) + 1, 0) < 0) {
        perror("Failed to send filename");
        free(file_data);
        close(client_socket);
        return EXIT_FAILURE;
    }
    
    printf("Filename sent: %s\n", filename);
    
    /* Send file size */
    size_t size_to_send = (size_t)file_size;
    if (send(client_socket, &size_to_send, sizeof(size_to_send), 0) < 0) {
        perror("Failed to send file size");
        free(file_data);
        close(client_socket);
        return EXIT_FAILURE;
    }
    
    printf("File size sent: %zu bytes\n", size_to_send);
    
    /* Send encrypted file data */
    printf("Sending encrypted file data...\n");
    size_t total_sent = 0;
    while (total_sent < size_to_send) {
        ssize_t bytes_sent = send(client_socket, file_data + total_sent, 
                                  size_to_send - total_sent, 0);
        if (bytes_sent < 0) {
            perror("Failed to send file data");
            free(file_data);
            close(client_socket);
            return EXIT_FAILURE;
        }
        total_sent += bytes_sent;
        printf("Sent %zu/%zu bytes (%.1f%%)\r", total_sent, size_to_send, 
               (double)total_sent / size_to_send * 100);
        fflush(stdout);
    }
    printf("\n");
    
    printf("File data sent successfully\n");
    free(file_data);
    
    /* Receive acknowledgment */
    char ack_buffer[256];
    memset(ack_buffer, 0, sizeof(ack_buffer));
    ssize_t ack_bytes = recv(client_socket, ack_buffer, sizeof(ack_buffer) - 1, 0);
    
    if (ack_bytes > 0) {
        ack_buffer[ack_bytes] = '\0';
        printf("Server response: %s\n", ack_buffer);
    } else {
        printf("No acknowledgment received from server\n");
    }
    
    close(client_socket);
    printf("File transfer completed. Connection closed.\n");
    
    return EXIT_SUCCESS;
}


