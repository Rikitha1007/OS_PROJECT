// File: user/test_pipe.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int pid;
    int p[2];
    char buf[100];
    int n;

    // Create pipe
    if(pipe(p) < 0) 
    {
        printf("pipe failed\n");
        exit(1);
    }

    // Fork a child process
    pid = fork();
    if(pid < 0) 
    {
        printf("fork failed\n");
        exit(1);
    }

    if(pid == 0) 
    {
        char *message = "Hello, pipe!";
        printf("Child: Writing to pipe...\n");
        n = write(p[1], message, strlen(message));
        if(n > 0) 
        {
            printf("Child wrote %d bytes\n", n);
        } 
        else 
        {
            printf("Child: Write failed or pipe closed\n");
        }  
        exit(0);
    } 
    else {     
        wait(0);
        printf("Parent: Reading from pipe...\n");
        n = read(p[0], buf, sizeof(buf));
        if(n > 0)
        {
            buf[n] = 0;  // Null terminate the string
            printf("Parent read %d bytes: %s\n", n, buf);
        } 
        else 
        {
            printf("Parent: Read failed or pipe closed\n");
        }
        
    }
}