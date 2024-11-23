#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "commands.h"

void custom_ls(const char *path) {
    struct stat path_stat;

    // Check if the path exists
    if (stat(path, &path_stat) != 0) {
        perror("Error");
        return;
    }

    // Check if the path is a directory
    if (S_ISDIR(path_stat.st_mode)) {
        DIR *dir = opendir(path);
        struct dirent *entry;

        if (!dir) {
            perror("Error opening directory");
            return;
        }

        while ((entry = readdir(dir)) != NULL) {
            // Skip the "." and ".." entries
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                printf("%s\n", entry->d_name);
            }
        }

        closedir(dir);
    } else {
        // If the path is a file, print its name
        printf("%s\n", path);
    }
}

int ls_main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [path]\n", argv[0]);
        return 1;
    }

    custom_ls(argv[1]);

    return 0;
}
