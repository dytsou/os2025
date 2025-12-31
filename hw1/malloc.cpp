#include <stdlib.h>
#include <stdio.h>

int main() {
    void *ptr = malloc(1024);
    if (ptr == NULL) {
        perror("malloc error");
        return 1;
    }
    free(ptr);
    return 0;
}

