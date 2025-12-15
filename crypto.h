#ifndef CRYPTO_H
#define CRYPTO_H

#include "common.h"

/* Encryption/Decryption function prototypes */
int encrypt_file(const char *input_file, const char *output_file, unsigned char key);
int decrypt_file(const char *input_file, const char *output_file, unsigned char key);
int encrypt_buffer(unsigned char *buffer, size_t size, unsigned char key);
int decrypt_buffer(unsigned char *buffer, size_t size, unsigned char key);
unsigned char* encrypt_data_in_memory(unsigned char *data, size_t size, unsigned char key);
unsigned char* decrypt_data_in_memory(unsigned char *data, size_t size, unsigned char key);

#endif /* CRYPTO_H */


