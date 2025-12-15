#include "crypto.h"

/* Encrypt a file using XOR cipher */
int encrypt_file(const char *input_file, const char *output_file, unsigned char key) {
    FILE *input_fp = fopen(input_file, "rb");
    if (!input_fp) {
        perror("Failed to open input file for encryption");
        return ERROR_FILE_IO;
    }

    FILE *output_fp = fopen(output_file, "wb");
    if (!output_fp) {
        perror("Failed to open output file for encryption");
        fclose(input_fp);
        return ERROR_FILE_IO;
    }

    unsigned char *buffer = (unsigned char *)malloc(DEFAULT_BUFFER_SIZE);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(input_fp);
        fclose(output_fp);
        return ERROR_MEMORY;
    }

    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, DEFAULT_BUFFER_SIZE, input_fp)) > 0) {
        encrypt_buffer(buffer, bytes_read, key);
        if (fwrite(buffer, 1, bytes_read, output_fp) != bytes_read) {
            perror("Failed to write encrypted data");
            free(buffer);
            fclose(input_fp);
            fclose(output_fp);
            return ERROR_FILE_IO;
        }
    }

    free(buffer);
    fclose(input_fp);
    fclose(output_fp);
    return SUCCESS;
}

/* Decrypt a file using XOR cipher */
int decrypt_file(const char *input_file, const char *output_file, unsigned char key) {
    FILE *input_fp = fopen(input_file, "rb");
    if (!input_fp) {
        perror("Failed to open input file for decryption");
        return ERROR_FILE_IO;
    }

    FILE *output_fp = fopen(output_file, "wb");
    if (!output_fp) {
        perror("Failed to open output file for decryption");
        fclose(input_fp);
        return ERROR_FILE_IO;
    }

    unsigned char *buffer = (unsigned char *)malloc(DEFAULT_BUFFER_SIZE);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(input_fp);
        fclose(output_fp);
        return ERROR_MEMORY;
    }

    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, DEFAULT_BUFFER_SIZE, input_fp)) > 0) {
        decrypt_buffer(buffer, bytes_read, key);
        if (fwrite(buffer, 1, bytes_read, output_fp) != bytes_read) {
            perror("Failed to write decrypted data");
            free(buffer);
            fclose(input_fp);
            fclose(output_fp);
            return ERROR_FILE_IO;
        }
    }

    free(buffer);
    fclose(input_fp);
    fclose(output_fp);
    return SUCCESS;
}

/* Encrypt buffer in place */
int encrypt_buffer(unsigned char *buffer, size_t size, unsigned char key) {
    if (!buffer) {
        return ERROR_MEMORY;
    }
    for (size_t i = 0; i < size; i++) {
        buffer[i] ^= key;
    }
    return SUCCESS;
}

/* Decrypt buffer in place (XOR is symmetric) */
int decrypt_buffer(unsigned char *buffer, size_t size, unsigned char key) {
    return encrypt_buffer(buffer, size, key);  // XOR is symmetric
}

/* Encrypt data in memory and return new buffer */
unsigned char* encrypt_data_in_memory(unsigned char *data, size_t size, unsigned char key) {
    if (!data) {
        return NULL;
    }
    unsigned char *encrypted = (unsigned char *)malloc(size);
    if (!encrypted) {
        return NULL;
    }
    memcpy(encrypted, data, size);
    encrypt_buffer(encrypted, size, key);
    return encrypted;
}

/* Decrypt data in memory and return new buffer */
unsigned char* decrypt_data_in_memory(unsigned char *data, size_t size, unsigned char key) {
    return encrypt_data_in_memory(data, size, key);  // XOR is symmetric
}


