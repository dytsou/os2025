#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <csignal>
#include <cerrno>
#include <vector>
#include <cstdint>

const char* SHM_NAME = "/hw4_shm";
const size_t SHM_SIZE = 0xFFFFF;
const char KEYS[] = "mtakooyutha";
const size_t KEY_LENGTH = sizeof(KEYS) - 1;
const int NUM_ITERATIONS = 1016;
const int PRNG_A = 0x2022;
const int PRNG_B = 0x1106;
const int PRNG_P = 0xc8763;
const int WAIT_TIME = 5;

// Pseudo-random number generator function
int prng(int x, int a, int b, int p) {
    return ((static_cast<uint64_t>(a) * x + b) % p);
}

// Cleanup function for shared memory resources
void cleanup_shm(void* shm_ptr, size_t shm_size, int shm_fd, const char* shm_name) {
    if (shm_ptr != MAP_FAILED && shm_ptr != nullptr) {
        if (munmap(shm_ptr, shm_size) == -1) {
            std::cerr << "Warning: Failed to unmap shared memory: " 
                      << strerror(errno) << std::endl;
        }
    }
    if (shm_fd != -1) {
        if (close(shm_fd) == -1) {
            std::cerr << "Warning: Failed to close shared memory file descriptor: " << strerror(errno) << std::endl;
        }
    }
    if (shm_name != nullptr) {
        if (shm_unlink(shm_name) == -1 && errno != ENOENT) {
            std::cerr << "Warning: Failed to unlink shared memory: " << strerror(errno) << std::endl;
        }
    }
}

int main() {
    // Get the target process PID
    pid_t target_pid;
    std::cout << "Input Heathcliff's PID: ";
    std::cin >> target_pid;

    if (std::cin.fail() || target_pid <= 0) {
        std::cerr << "Error: Invalid PID" << std::endl;
        return 1;
    }

    // Initialize shared memory resources
    void* shm_ptr = MAP_FAILED;
    int shm_fd = -1;

    // Create shared memory object
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "Error: Failed to create shared memory: " << strerror(errno) << std::endl;
        return 1;
    }

    // Set shared memory size
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        std::cerr << "Error: Failed to set size of shared memory: " << strerror(errno) << std::endl;
        cleanup_shm(nullptr, 0, shm_fd, SHM_NAME);
        return 1;
    }

    // Map shared memory
    shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        std::cerr << "Error: Failed to map shared memory: " << strerror(errno) << std::endl;
        cleanup_shm(nullptr, 0, shm_fd, SHM_NAME);
        return 1;
    }

    // Initialize shared memory to zero
    memset(shm_ptr, 0, SHM_SIZE);

    // Initialize PRNG with target PID
    int offset = target_pid;
    std::vector<int> offsets;  // Store all offsets for collision checking
    offsets.reserve(NUM_ITERATIONS);

    // Write KEYS pattern at various offsets using PRNG
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // Generate a valid offset that doesn't collide with previous offsets
        bool valid_offset;
        do {
            valid_offset = true;
            offset = prng(offset, PRNG_A, PRNG_B, PRNG_P);
            
            // Check if new offset is within KEY_LENGTH bytes of any existing offset
            for (auto prev_offset : offsets) {
                int diff = offset - prev_offset;
                if (diff >= -static_cast<int>(KEY_LENGTH) && 
                    diff <= static_cast<int>(KEY_LENGTH)) {
                    valid_offset = false;
                    break;
                }
            }
        } while (!valid_offset);

        // Check if offset exceeds shared memory size
        if (offset + KEY_LENGTH > SHM_SIZE) {
            std::cerr << "Warning: Offset exceeds shared memory size. Stopping write loop at iteration " << i << std::endl;
            break;
        }

        // Write KEYS in the order expected by the target process
        // Pattern: KEYS[(4 * j) % KEY_LENGTH] for j in [0, KEY_LENGTH - 1]
        char* write_ptr = static_cast<char*>(shm_ptr) + offset;
        for (size_t j = 0; j < KEY_LENGTH; j++) {
            int key_index = (4 * static_cast<int>(j)) % KEY_LENGTH;
            write_ptr[j] = KEYS[key_index];
        }

        offsets.push_back(offset);
    }

    // Sync shared memory data to ensure it's written to disk
    if (msync(shm_ptr, SHM_SIZE, MS_SYNC) == -1) {
        std::cerr << "Warning: Failed to sync shared memory: "  << strerror(errno) << std::endl;
    }

    // Send SIGUSR1 signal to the target process
    if (kill(target_pid, SIGUSR1) == -1) {
        std::cerr << "Error: Failed to send SIGUSR1 signal: " << strerror(errno) << std::endl;
        cleanup_shm(shm_ptr, SHM_SIZE, shm_fd, SHM_NAME);
        return 1;
    }

    std::cout << "SIGUSR1 signal was sent successfully." << std::endl;

    // Wait to ensure the target process reads the data
    sleep(WAIT_TIME);

    // Clean up shared memory resources
    cleanup_shm(shm_ptr, SHM_SIZE, shm_fd, SHM_NAME);

    return 0;
}
