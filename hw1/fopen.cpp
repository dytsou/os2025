#include <stdio.h>

int main() {
    FILE *fp = fopen("test.txt", "r");
    if (fp == NULL) {
        return 1;
    }
    fclose(fp);
    return 0;
}

