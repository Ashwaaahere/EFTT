# Encrypted File Transfer Tool (EFTT)

## Project Overview

EFTT is a secure file transfer system implemented in C using TCP sockets. The client encrypts files before transmission, and the server decrypts and stores them upon receipt. The project incorporates key systems programming concepts including networking, dynamic memory allocation, file I/O, signal handling, multi-threading, and logging.

## Team Members

1. Ashwathaman Satish Kumar
2. Shreejha Sukanthan
3. Hemapriya Yuganderan

## Features

- **Secure File Transfer**: Files are encrypted on the client side before transmission
- **TCP Socket Communication**: Reliable data transfer using TCP protocol
- **Multi-Client Support**: Server can handle multiple clients concurrently using threads
- **Transfer Logging**: All file transfers are logged for verification and audit
- **Signal Handling**: Graceful shutdown handling for server and client
- **Error Handling**: Comprehensive error checking and recovery

## Project Structure

```
proj 3/
├── README.md           # Project documentation
├── Makefile           # Build configuration
├── server.c           # Server implementation
├── client.c           # Client implementation
├── crypto.c           # Encryption/decryption utilities
├── crypto.h           # Encryption/decryption header
├── logger.c           # Logging utilities
├── logger.h           # Logging header
└── common.h           # Common definitions and utilities
```

## Building the Project

### Prerequisites

- GCC compiler
- POSIX-compliant operating system (Linux/macOS/WSL)
- Make utility

### Compilation

To build both server and client:

```bash
make
```

To build only the server:

```bash
make server
```

To build only the client:

```bash
make client
```

To clean build artifacts:

```bash
make clean
```

## Usage

### Starting the Server

```bash
./server [PORT]
```

If no port is specified, the default port 8080 is used.

The server will:
- Create a `received_files/` directory for storing received files
- Create a `logs/` directory for transfer logs
- Listen for incoming client connections
- Handle multiple clients concurrently using threads

### Running the Client

```bash
./client [SERVER_IP] [SERVER_PORT] [FILE_PATH]
```

Example:

```bash
./client localhost 8080 ./myfile.txt
```

The client will:
- Read the specified file
- Encrypt the file content
- Send the encrypted data to the server
- Display transfer status

## Encryption Method

The project uses a simple XOR-based encryption with a configurable key. For production use, consider implementing more robust encryption algorithms (AES, RSA, etc.).

## System Programming Concepts Demonstrated

1. **Networking**: TCP socket programming (socket, bind, listen, accept, connect, send, recv)
2. **Dynamic Memory Allocation**: malloc, calloc, realloc, free
3. **File I/O**: fopen, fread, fwrite, fclose
4. **Signal Handling**: SIGINT, SIGTERM handlers for graceful shutdown
5. **Multi-threading**: pthread for concurrent client handling
6. **Process Management**: fork, exec (if needed)
7. **Error Handling**: Comprehensive error checking and reporting

## Code Distribution

### Overall Statistics

- **Server Implementation**: ~221 lines
- **Client Implementation**: ~165 lines
- **Encryption/Decryption Module**: ~136 lines (crypto.c + crypto.h)
- **Logging Module**: ~81 lines (logger.c + logger.h)
- **Common Utilities**: ~99 lines (common.c + common.h)

**Total**: ~602 lines of code

---

## Individual Contributions

### 1. Ashwathaman Satish Kumar
**Contribution**: Server Implementation & Multi-threading
- **Files**: `server.c`
- **Lines of Code**: ~221 lines
- **Responsibilities**:
  - TCP server socket implementation (bind, listen, accept)
  - Multi-threaded client handling using pthread
  - File decryption and storage on server side
  - Signal handling for graceful server shutdown
  - Server-side file I/O operations
  - Error handling and client connection management
- **Key Concepts Demonstrated**:
  - TCP socket programming (server-side)
  - Multi-threading (pthread_create, pthread_detach)
  - Signal handling (SIGINT, SIGTERM)
  - Dynamic memory allocation for client data

### 2. Shreejha Sukanthan
**Contribution**: Client Implementation & Common Utilities
- **Files**: `client.c`, `common.c`, `common.h`
- **Lines of Code**: ~264 lines
- **Responsibilities**:
  - TCP client socket implementation (connect, send, recv)
  - File reading and encryption before transmission
  - Client-side file I/O operations
  - Common utility functions (socket creation, directory management)
  - Signal handling for client shutdown
  - Error handling and connection management
- **Key Concepts Demonstrated**:
  - TCP socket programming (client-side)
  - File I/O operations (fopen, fread, fseek, ftell)
  - Dynamic memory allocation for file buffers
  - Common system utilities and helpers

### 3. Hemapriya Yuganderan
**Contribution**: Encryption/Decryption Module & Logging System
- **Files**: `crypto.c`, `crypto.h`, `logger.c`, `logger.h`
- **Lines of Code**: ~217 lines
- **Responsibilities**:
  - XOR-based encryption/decryption implementation
  - File and buffer encryption/decryption functions
  - Logging system implementation
  - Transfer logging and audit trail
  - Timestamp generation and log file management
- **Key Concepts Demonstrated**:
  - Encryption/decryption algorithms
  - File I/O for logging operations
  - Dynamic memory allocation for encrypted data
  - System calls (mkdir, stat) for directory management
  - Formatted logging with timestamps

---

### Contribution Summary

| Team Member | Files | Lines of Code | Primary Focus |
|------------|-------|---------------|---------------|
| **Ashwathaman Satish Kumar** | server.c | ~221 | Server & Multi-threading |
| **Shreejha Sukanthan** | client.c, common.c, common.h | ~264 | Client & Utilities |
| **Hemapriya Yuganderan** | crypto.c, crypto.h, logger.c, logger.h | ~217 | Encryption & Logging |
| **Total** | 9 files | **~702** | Complete EFTT System |

**Note**: The project was developed collaboratively with each team member focusing on their assigned modules. All team members contributed to project planning, testing, and documentation. The line counts include both implementation files (.c) and header files (.h).

## Future Enhancements

- Implement AES encryption for stronger security
- Add file compression before encryption
- Support for directory transfer
- Progress indicators for large file transfers
- Authentication and authorization mechanisms
- Configurable encryption keys per session



