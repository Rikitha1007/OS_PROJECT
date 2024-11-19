// File: user/test_wait.c
#include "kernel/types.h"  // Add this first
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    int pid;
    int status;

    pid = fork();
    if(pid < 0){
        printf("fork failed\n");
        exit(1);
    }
    
    if(pid == 0) {  // Child process
        printf("Child process running\n");
        //sleep(10);  // Sleep for a bit
        exit(42);   // Exit with status 42
    } else {
        wait(&status);        // Parent process
        printf("Child process exited with status: %d\n", status);
        exit(0);
    }
}