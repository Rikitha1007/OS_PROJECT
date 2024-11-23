#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include "commands.h"

void copy_file(const char *source, const char *destination, int preserve) {
    int src_fd = open(source, O_RDONLY);
    if (src_fd < 0) {
        perror("Error opening source file");
        return;
    }

    int dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Error opening/creating destination file");
        close(src_fd);
        return;
    }

    char buffer[4096];
    ssize_t bytes;
    while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dest_fd, buffer, bytes) != bytes) {
            perror("Error writing to destination file");
            break;
        }
    }

    if (bytes < 0) {
        perror("Error reading source file");
    }

    close(src_fd);
    close(dest_fd);

    if (preserve) {
        struct stat st;
        if (stat(source, &st) == 0) {
            chmod(destination, st.st_mode);
            chown(destination, st.st_uid, st.st_gid);
        }
    }
}

void copy_directory(const char *source, const char *destination, int preserve) {
    DIR *dir = opendir(source);
    if (!dir) {
        perror("Error opening source directory");
        return;
    }

    mkdir(destination, 0755);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char src_path[1024], dest_path[1024];
        snprintf(src_path, sizeof(src_path), "%s/%s", source, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", destination, entry->d_name);

        struct stat st;
        if (stat(src_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                copy_directory(src_path, dest_path, preserve);
            } else {
                copy_file(src_path, dest_path, preserve);
            }
        }
    }

    closedir(dir);
}

void print_usage() {
    printf("Usage: cp [options] source_file... target_directory\n");
    printf("Options:\n");
    printf("  -p  Preserve file attributes\n");
    printf("  -r  Copy directories recursively\n");
}

int cp_main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
        exit(EXIT_FAILURE);
    }

    int preserve = 0, recursive = 0;
    int file_start = 1;

    // Parse options
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (char *opt = argv[i] + 1; *opt; opt++) {
                switch (*opt) {
                    case 'p': preserve = 1; break;
                    case 'r': recursive = 1; break;
                    default:
                        fprintf(stderr, "Unknown option: -%c\n", *opt);
                        exit(EXIT_FAILURE);
                }
            }
            file_start++;
        } else {
            break;
        }
    }

    char *target = argv[argc - 1];

    // Check if target is a directory
    struct stat st;
    if (stat(target, &st) == 0 && S_ISDIR(st.st_mode)) {
        for (int i = file_start; i < argc - 1; i++) {
            char dest_path[1024];
            snprintf(dest_path, sizeof(dest_path), "%s/%s", target, strrchr(argv[i], '/') ? strrchr(argv[i], '/') + 1 : argv[i]);

            if (stat(argv[i], &st) == 0 && S_ISDIR(st.st_mode)) {
                if (!recursive) {
                    fprintf(stderr, "Omitting directory '%s' (use -r to copy directories)\n", argv[i]);
                    continue;
                }
                copy_directory(argv[i], dest_path, preserve);
            } else {
                copy_file(argv[i], dest_path, preserve);
            }
        }
    } else if (argc == file_start + 2) {
        // Single source to single target
        if (stat(argv[file_start], &st) == 0 && S_ISDIR(st.st_mode)) {
            fprintf(stderr, "Cannot copy directory '%s' without -r\n", argv[file_start]);
            exit(EXIT_FAILURE);
        }
        copy_file(argv[file_start], target, preserve);
    } else {
        fprintf(stderr, "Target must be a directory for multiple sources\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
