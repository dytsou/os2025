#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int x = 1;
int main(void) {
    pid_t c_pid;

    int status;
    c_pid = fork();

    if (c_pid == 0) {
        sleep(3);
        //Child process' variable
        x += 10;
        printf("Child has x = %d\n", x);
    } 
    else if (c_pid > 0) {
        sleep(1);
        wait(&status);
        //Parent process' variable
        x -= 5;
        printf("Parent has x = %d\n", x);
    } 
    else {
        printf("Error!\n");
    }

    return 0;
}

