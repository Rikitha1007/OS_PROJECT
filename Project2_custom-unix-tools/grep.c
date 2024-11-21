#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_file(const char *filename, int number_lines, int number_non_blank, int show_ends, int suppress_blank, int show_tabs) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

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

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [options] file...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

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

    // Process each file
    for (int i = file_start; i < argc; i++) {
        display_file(argv[i], number_lines, number_non_blank, show_ends, suppress_blank, show_tabs);
    }

    return 0;
}
