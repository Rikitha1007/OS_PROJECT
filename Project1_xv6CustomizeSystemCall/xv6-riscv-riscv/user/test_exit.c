#include "kernel/types.h"
#include "user.h"
#include "kernel/stat.h"

int main(void) {
    printf("Testing modified exit functionality...\n");
    
    // Do some work
    for(int i = 0; i < 5; i++) {
        printf( "Working... step %d\n", i+1);
    }
    
    // Exit with status 42
    exit(42);
    
    return 0;  // Never reached
}