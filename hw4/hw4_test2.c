#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t pid;

int prng(int x, int a, int b, int p) {
    return ((uint64_t)a * x + b) % p;
}

void sigusr1_handler(int signo) {
    puts("Heathcliff is under attack.");

    const char* shm_name = "/hw4_shm";
    const int shm_size = 0xFFFFF;

    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }
    void* shm_ptr = mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    bool immortal_object = false;
    int off = pid;
    int offs[1016];
    for(int i=0; i<1016; i++) {
        bool ok;
        do {
            ok = true;
            off = prng(off, 0x2022, 0x1106, 0xc8763);
            for(int j=0; j<i; j++) {
                int diff = off - offs[j];
                if (diff >= -11 && diff <= 11) {
                    ok = false;
                    break;
                }
            }
        }
        while(!ok);

        const char KEYS[12] = "mtakooyutha";
        for(int i=0; i<11; i++) {
            /* Notice: Here is 4*i, not i */
            if (*(char*)(shm_ptr + off + i) != KEYS[(4 * i) % 11]) {
                immortal_object = true;
                break;
            }
        }

        if (immortal_object)
            break;

        offs[i] = off;
    }

    munmap(shm_ptr, shm_size);
    close(shm_fd);

    if (immortal_object) {
        puts("Isn't it quite a dramatic plot development?");
        return;
    }
    else {
        puts("You dare use my own spells against me, Kirito?");
        exit(0);
    }
}

int main() {
    puts("This, might be a game, but it isn't meant to be played.\n\t\t\t-by SAO Programmer Kayaba Akihiko");
    pid = getpid();
    printf("Heathcliff's PID: %d\n", pid);

    if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
        perror("signal");
        return 1;
    }
    while(1) // Oops, the logout button is missing from the main menu.
        pause();
    return 0;
}
