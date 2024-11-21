#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

// Function to delete directories recursively
int delete_directory(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    struct dirent *entry;
    char path[1024];

    if (!dir) {
        perror("Error opening directory");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip the "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        struct stat path_stat;
        stat(path, &path_stat);

        if (S_ISDIR(path_stat.st_mode)) {
            // Recursively delete the subdirectory
            delete_directory(path);
        } else {
            // Delete the file
            if (unlink(path) != 0) {
                perror("Error deleting file");
            }
        }
    }

    closedir(dir);

    // Remove the directory itself
    if (rmdir(dir_path) != 0) {
        perror("Error removing directory");
        return -1;
    }

    return 0;
}

void custom_rm(const char *filepath) {
    struct stat path_stat;

    // Check if the path exists
    if (stat(filepath, &path_stat) != 0) {
        perror("Error");
        return;
    }

    // Check if it's a directory
    if (S_ISDIR(path_stat.st_mode)) {
        if (delete_directory(filepath) == 0) {
            printf("Directory '%s' deleted successfully.\n", filepath);
        }
    } else {
        // Delete the file
        if (unlink(filepath) == 0) {
            printf("File '%s' deleted successfully.\n", filepath);
        } else {
            perror("Error deleting file");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [path]\n", argv[0]);
        return 1;
    }

    custom_rm(argv[1]);

    return 0;
}
