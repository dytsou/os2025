#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define FIFO_READ "./hw4_read.fifo"
#define FIFO_WRITE "./hw4_write.fifo"
#define BUFFER_SIZE 33

// Helper function to read exactly n bytes from a file descriptor
int read_exact(int fd, char *buffer, size_t size) {
    size_t total_read = 0;
    while (total_read < size) {
        ssize_t bytes_read = read(fd, buffer + total_read, size - total_read);
        if (bytes_read == 0) {
            // EOF reached
            return 0;
        }
        if (bytes_read == -1) {
            if (errno == EINTR) {
                // Interrupted by signal, retry
                continue;
            }
            return -1;
        }
        total_read += bytes_read;
    }
    return size;
}

int main() {
    // Create FIFOs
    if (mkfifo(FIFO_READ, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Error creating FIFO_READ");
            return 1;
        }
    }
    if (mkfifo(FIFO_WRITE, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Error creating FIFO_WRITE");
            unlink(FIFO_READ);
            return 1;
        }
    }
    printf("FIFO_READ and FIFO_WRITE have been created successfully.\n");

    // Open FIFOs
    int fd_read = open(FIFO_READ, O_RDONLY);
    if (fd_read == -1) {
        perror("Error opening FIFO_READ");
        unlink(FIFO_READ);
        unlink(FIFO_WRITE);
        return 1;
    }

    int fd_write = open(FIFO_WRITE, O_WRONLY);
    if (fd_write == -1) {
        perror("Error opening FIFO_WRITE");
        close(fd_read);
        unlink(FIFO_READ);
        unlink(FIFO_WRITE);
        return 1;
    }
    printf("FIFO_READ and FIFO_WRITE have been opened successfully.\n");

    char plaintext[BUFFER_SIZE], ciphertext[BUFFER_SIZE], key[BUFFER_SIZE];

    // Main communication loop
    while (1) {
        // Read plaintext (exactly 33 bytes)
        if (read_exact(fd_read, plaintext, BUFFER_SIZE) != BUFFER_SIZE) {
            if (errno != 0) {
                perror("Error reading plaintext from FIFO");
            }
            break;
        }

        // Read ciphertext (exactly 33 bytes)
        if (read_exact(fd_read, ciphertext, BUFFER_SIZE) != BUFFER_SIZE) {
            if (errno != 0) {
                perror("Error reading ciphertext from FIFO");
            }
            break;
        }

        // Compute key by XORing plaintext and ciphertext
        for (int i = 0; i < 32; i++) {
            key[i] = plaintext[i] ^ ciphertext[i];
        }
        key[32] = '\0';

        // Print results
        printf("Plaintext: %s\n", plaintext);
        printf("Ciphertext: %s\n", ciphertext);
        printf("Answer: %s\n\n", key);

        // Check for "Well done!" message
        if (strcmp(plaintext, "Well done!") == 0) {
            printf("Plaintext: Well done!\n");
        }

        // Write key back to FIFO
        ssize_t bytes_written = write(fd_write, key, BUFFER_SIZE);
        if (bytes_written == -1) {
            perror("Error writing to FIFO");
            break;
        }
        if (bytes_written != BUFFER_SIZE) {
            fprintf(stderr, "Warning: Incomplete write to FIFO\n");
        }
    }

    // Cleanup
    close(fd_read);
    close(fd_write);

    printf("hw4_write.fifo deleted\n");
    printf("hw4_read.fifo deleted\n");

    unlink(FIFO_WRITE);
    unlink(FIFO_READ);

    return 0;
}