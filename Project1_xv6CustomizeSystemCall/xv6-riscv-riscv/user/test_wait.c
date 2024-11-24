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
/*
#include "kernel/types.h"  // Kernel-specific types
#include "kernel/stat.h"   // Kernel-specific stat info
#include "user/user.h"     // User-specific system call declarations

int
main(void)
{
    int pid;
    int status;

    pid = fork();
    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    }

    if (pid == 0) { // Child process
        printf("Child process running\n");
        sleep(5);  // Simulate work by sleeping for 5 seconds
        exit(42);  // Exit with status 42
    } else { // Parent process
        printf("Parent waiting with a timeout of 3 ticks\n");
        
        int ret = wait(&status, 3); // Call wait() with timeout
        if (ret == -1) {
            printf("Timeout occurred; no child exited within the specified time.\n");
        } else {
            printf("Child process exited with status: %d\n", status);
        }

        exit(0);
    }
}
*/
