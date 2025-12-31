#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <setjmp.h>
jmp_buf jump_buffer;
int jump_case;
void handler(int signum) {
    longjmp(jump_buffer, jump_case);
}

int main(void)
{
    pid_t ppid, cpid;
    ppid = getpid();
    printf("Input jump_case : ");
    scanf("%d",&jump_case);
    signal(SIGINT, handler);
    cpid = fork();
    if(cpid < 0)
    {
        perror("Fail to fork\n");
        exit(1);
    }
    else if(cpid == 0)
    { 
        int jp = setjmp(jump_buffer);
        if (jp == 0) {
            while(1){
                printf("Child is waiting for signal \n");
                
                sleep(1);
            }
        }

        else {
            printf("\n");
            while(1){
                printf("Child is waiting for kill\n");
                
                sleep(1);
            }
        }
        
    }

    else
    {   
        int jp = setjmp(jump_buffer);
        if (jp == 0) {
            while(1){
                printf("Parent is waiting for signal \n");          
                sleep(1);
            }
        }

        else {
            sleep(1);
            switch(jp)
            {
            case 1:
                sleep(2);
                printf("\nGoing to kill child\n");
                sleep(3);
                if(kill(cpid, SIGKILL) < 0)
                {
                    perror("Fail to send signal\n");
                    exit(1);
                }
                sleep(1);
                printf("Child has been killed.\n");
                
                break;
            default: 
                while(1){
                    printf("\n Unknown Error \n");
                    sleep(1);
                }
                break;
            }
        }
        
    }
    printf("END\n");
    return 0;
}
