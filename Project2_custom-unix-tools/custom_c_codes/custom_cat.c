#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"


void display_file(FILE *file, int number_lines, int number_non_blank, int show_ends, int suppress_blank, int show_tabs) {
    char line[1024];
    int line_number = 1;
    int non_blank_number = 1;
    int last_line_empty = 0;

    while (fgets(line, sizeof(line), file)) {
        int is_blank = (line[0] == '\n');

        if (suppress_blank && last_line_empty && is_blank) {
            continue;
        }

        last_line_empty = is_blank;

        if (number_non_blank && !is_blank) {
            printf("%6d\t", non_blank_number++);
        } else if (number_lines) {
            printf("%6d\t", line_number++);
        }

        for (char *ptr = line; *ptr; ptr++) {
            if (show_tabs && *ptr == '\t') {
                printf("^I");
            } else {
                putchar(*ptr);
            }
        }

        if (show_ends) {
            printf("$");
        }

        if (!show_ends) {
            putchar('\n');
        }
    }
}

int cat_main(int argc, char *argv[]) {
    // Options
    int number_lines = 0, number_non_blank = 0, show_ends = 0, suppress_blank = 0, show_tabs = 0;

    // Parse options
    int file_start = 1;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (char *opt = argv[i] + 1; *opt; opt++) {
                switch (*opt) {
                    case 'n': number_lines = 1; break;
                    case 'b': number_non_blank = 1; break;
                    case 'E': show_ends = 1; break;
                    case 's': suppress_blank = 1; break;
                    case 'T': show_tabs = 1; break;
                    default:
                        fprintf(stderr, "Unknown option: -%c\n", *opt);
                        exit(EXIT_FAILURE);
                }
            }
        } else {
            file_start = i;
            break;
        }
    }

    if (file_start == argc) {
        // No files specified, read from standard input
        display_file(stdin, number_lines, number_non_blank, show_ends, suppress_blank, show_tabs);
    } else {
        // Process each file
        for (int i = file_start; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (!file) {
                perror("Error opening file");
                continue;
            }
            display_file(file, number_lines, number_non_blank, show_ends, suppress_blank, show_tabs);
            fclose(file);
        }
    }

    return 0;
}
