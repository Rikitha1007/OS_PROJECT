#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_usage() {
    printf("Usage: mv source_file target_file\n");
    printf("       mv source_file... target_directory\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
        exit(EXIT_FAILURE);
    }

    char *target = argv[argc - 1];

    // Check if target is a directory
    if (access(target, F_OK) == 0 && access(target, X_OK) == 0) {
        // Handle multiple sources
        for (int i = 1; i < argc - 1; i++) {
            char dest_path[1024];
            snprintf(dest_path, sizeof(dest_path), "%s/%s", target, strrchr(argv[i], '/') ? strrchr(argv[i], '/') + 1 : argv[i]);
            
            // Rename (move) file
            if (rename(argv[i], dest_path) != 0) {
                perror("Error moving file");
            }
        }
    } else if (argc == 3) {
        // Single source to single target
        if (rename(argv[1], target) != 0) {
            perror("Error renaming/moving file");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Target must be a directory for multiple sources\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
