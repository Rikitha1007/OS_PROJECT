#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <dirent.h>
#include <sys/stat.h>
#include "commands.h"

void custom_ls(int argc, char *argv[]) {
    const char *path = (argc > 1) ? argv[1] : "."; 
    struct stat path_stat;

    if (stat(path, &path_stat) != 0) {
        perror("custom_ls");
        return;
    }

    if (S_ISDIR(path_stat.st_mode)) {
        DIR *dir = opendir(path);
        if (!dir) {
            perror("custom_ls");
            return;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {

            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                printf("%s\n", entry->d_name);
            }
        }

        closedir(dir);
    } else {

        printf("%s\n", path);
    }
}

void custom_cat(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: custom_cat <file>\n");
        return;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("custom_cat");
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
}


void custom_wc(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: custom_wc <file>\n");
        return;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("custom_wc");
        return;
    }

    int values[3] = {0}; 

    char each_line[1024]; 
    while (fgets(each_line, sizeof(each_line), file)) {
        values[0]++; 
        int char_count = strlen(each_line); 
 
        if (each_line[char_count - 1] == '\n') {
            char_count--; 
        }
        values[1] += char_count; 

        int word_count = 0, in_word = 0;
        for (int i = 0; i < char_count; i++) {
            char c = each_line[i];
    
            if ((65 <= c && c <= 90) || (97 <= c && c <= 122) || (48 <= c && c <= 57)) {
                if (!in_word) {
                    word_count++;
                    in_word = 1;
                }
            } else {
                in_word = 0; 
            }
        }

        values[2] += word_count; 
    }

    fclose(file);

    printf("Lines: %d, Words: %d, Characters: %d\n", values[0], values[2], values[1]);
}

void custom_cp(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: custom_cp <source> <destination>\n");
        return;
    }

    FILE *src = fopen(argv[1], "r");
    if (!src) {
        perror("custom_cp: source");
        return;
    }

    FILE *dest = fopen(argv[2], "w");
    if (!dest) {
        perror("custom_cp: destination");
        fclose(src);
        return;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);
}

void custom_rm(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: custom_rm <file>\n");
        return;
    }

    if (remove(argv[1]) != 0) {
        perror("custom_rm");
    }
}

void custom_mv(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: custom_mv <source> <destination>\n");
        return;
    }

    if (rename(argv[1], argv[2]) != 0) {
        perror("custom_mv");
    }
}


void shift_left(char *str) {
    if (!str || strlen(str) == 0) {
        return; 
    }

    int length = strlen(str);

    for (int i = 0; i < length - 1; i++) {
        str[i] = str[i + 1];
    }

    str[length - 2] = '\0';
}

void custom_grep(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: custom_grep <pattern> <file>\n");
        return;
    }

    FILE *file = fopen(argv[2], "r");
    if (!file) {
        perror("custom_grep");
        return;
    }

    char buffer[1024];
    char *pattern = argv[1];
    shift_left(pattern);


    while (fgets(buffer, sizeof(buffer), file)) {

        if (strstr(buffer, pattern)!=0) {
          printf("%s\n", buffer);
        }
    }
    fclose(file);
}



