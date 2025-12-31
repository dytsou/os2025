#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    
    int fd = open("shared_file1", O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char *shared_data = mmap(NULL, 1024, PROT_READ, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    
    printf("Shared data: %s\n", shared_data);
    
    if (munmap(shared_data, 1024) == -1) {
        perror("munmap");
        return 1;
    }
    
    close(fd);
    
    return 0;
}

