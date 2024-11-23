#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

void perform(FILE *ptr, int values[3]) {
    char each_line[100];
    
    while (fgets(each_line, 100, ptr)) {
        values[0]++;  // Increment line count

        int i = 0, word_count = 0;
        int char_count = strlen(each_line); 
        
        if (each_line[char_count - 1] == '\n') 
            char_count--; // Remove newline character if present
        values[1] += char_count; // Add character count to the total

        int in_word = 0; 
        while (i < char_count) {
            char c = each_line[i];
            
            if ((65 <= c && c <= 90) || (97 <= c && c <= 122)) { // Check if character is alphabetic
                if (!in_word) {
                    word_count++; // Start of a new word
                    in_word = 1;
                }
            } else if (48 <= c && c <= 57) { // Check if character is a digit
                if (!in_word) {
                    word_count++; // Start of a new "word" for digits
                    in_word = 1;
                }
            } else {
                in_word = 0; // End of the current word
            }

            i++;
        }

        values[2] += word_count; // Add word count to the total
    }
}

int wc_main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int values[3] = {0}; // [0] = line count, [1] = char count, [2] = word count

    perform(file, values);

    fclose(file);
    
    printf("Number of lines: %d\n", values[0]);
    printf("Number of words: %d\n", values[2]);
    printf("Number of characters: %d\n", values[1]);

    return 0;
}

