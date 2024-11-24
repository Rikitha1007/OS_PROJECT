#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_commands.h"

#define MAX_INPUT_SIZE 1024

void parse_input(char *input, int *argc, char **argv) {
    *argc = 0;
    char *token = strtok(input, " \t\n");
    while (token != NULL) {
        argv[(*argc)++] = token;
        token = strtok(NULL, " \t\n");
    }
    argv[*argc] = NULL;
}

void custom_shell_loop() {
    char input[MAX_INPUT_SIZE];
    char *argv[MAX_INPUT_SIZE / 2];
    int argc;

    while (1) {
        printf("Rikitha/OS_PROJECT> ");
        if (!fgets(input, MAX_INPUT_SIZE, stdin)) {
            printf("\nExiting shell.\n");
            break;
        }

        parse_input(input, &argc, argv);

        if (argc == 0) {
            continue;
        }

        // Exit condition
        if (strcmp(argv[0], "exit") == 0) {
            printf("exiting the shell\n");
            break;
        }

        // Dispatch commands to respective functions
        if (strcmp(argv[0], "custom_ls") == 0) {
            custom_ls(argc, argv);
        } else if (strcmp(argv[0], "custom_cat") == 0) {
            custom_cat(argc, argv);
        } else if (strcmp(argv[0], "custom_wc") == 0) {
            custom_wc(argc, argv);
        } else if (strcmp(argv[0], "custom_cp") == 0) {
            custom_cp(argc, argv);
        } else if (strcmp(argv[0], "custom_rm") == 0) {
            custom_rm(argc, argv);
        } else if (strcmp(argv[0], "custom_mv") == 0) {
            custom_mv(argc, argv);
        } else if (strcmp(argv[0], "custom_grep") == 0) {
            custom_grep(argc, argv);
        } else {
            printf("Unknown command: %s\n", argv[0]);
        }
    }
}

int main() {
    printf("Custom Shell. Type 'exit' to quit.\n");
    custom_shell_loop();
    return 0;
}

