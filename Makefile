# Makefile for Encrypted File Transfer Tool (EFTT)

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pthread
LDFLAGS = -pthread

# Directories
SRC_DIR = .
BUILD_DIR = .

# Source files
COMMON_SRC = common.c
CRYPTO_SRC = crypto.c
LOGGER_SRC = logger.c
SERVER_SRC = server.c
CLIENT_SRC = client.c

# Object files
COMMON_OBJ = $(BUILD_DIR)/common.o
CRYPTO_OBJ = $(BUILD_DIR)/crypto.o
LOGGER_OBJ = $(BUILD_DIR)/logger.o
SERVER_OBJ = $(BUILD_DIR)/server.o
CLIENT_OBJ = $(BUILD_DIR)/client.o

# Executables
SERVER_EXEC = server
CLIENT_EXEC = client

# Default target
all: $(SERVER_EXEC) $(CLIENT_EXEC)

# Build server
$(SERVER_EXEC): $(SERVER_OBJ) $(COMMON_OBJ) $(CRYPTO_OBJ) $(LOGGER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Server built successfully: $@"

# Build client
$(CLIENT_EXEC): $(CLIENT_OBJ) $(COMMON_OBJ) $(CRYPTO_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Client built successfully: $@"

# Build server only
server: $(SERVER_EXEC)

# Build client only
client: $(CLIENT_EXEC)

# Compile object files
$(SERVER_OBJ): $(SRC_DIR)/server.c common.h crypto.h logger.h
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_OBJ): $(SRC_DIR)/client.c common.h crypto.h
	$(CC) $(CFLAGS) -c $< -o $@

$(COMMON_OBJ): $(SRC_DIR)/common.c common.h
	$(CC) $(CFLAGS) -c $< -o $@

$(CRYPTO_OBJ): $(SRC_DIR)/crypto.c crypto.h common.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LOGGER_OBJ): $(SRC_DIR)/logger.c logger.h common.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC)
	rm -f $(BUILD_DIR)/*.o
	@echo "Cleaned build artifacts"

# Clean everything including received files and logs
clean-all: clean
	rm -rf received_files logs
	@echo "Cleaned all generated files"

# Run server (default port 8080)
run-server: $(SERVER_EXEC)
	./$(SERVER_EXEC)

# Create test file for testing
test-file:
	@echo "This is a test file for EFTT." > test.txt
	@echo "It contains multiple lines of text." >> test.txt
	@echo "Testing encryption and decryption." >> test.txt
	@echo "Created test.txt"

# Help target
help:
	@echo "EFTT Build System"
	@echo "================="
	@echo "Targets:"
	@echo "  all          - Build both server and client (default)"
	@echo "  server       - Build only the server"
	@echo "  client       - Build only the client"
	@echo "  clean        - Remove build artifacts"
	@echo "  clean-all    - Remove build artifacts, received files, and logs"
	@echo "  run-server   - Build and run the server on default port 8080"
	@echo "  test-file    - Create a test file for testing"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  make                - Build everything"
	@echo "  ./server [PORT]     - Run server (default port: 8080)"
	@echo "  ./client IP PORT FILE - Run client to transfer file"

.PHONY: all server client clean clean-all run-server test-file help


