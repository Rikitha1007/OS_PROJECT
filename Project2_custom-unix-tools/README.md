# Custom Shell with Simplified UNIX-like Utilities
## Project Overview
This project implements a custom shell along with several lightweight versions of common UNIX-like utilities. These utilities include commands such as ls, cat, grep, wc, cp, mv, and rm, all of which are re-implemented from scratch in C. The project is split into multiple files:
- custom_command.c: Contains the function implementations for each of the custom utilities.
- custom_command.h: Declares the functions used in custom_command.c to define each utility's behavior.
- custom_shell.c: Acts as the main program that integrates the custom utilities and allows users to interact with them   via a shell interface.

## custom_command.c
custom_ls(): Lists directory contents.
custom_cat(): Displays the contents of a file.
custom_grep(): Searches for a pattern in a file.
custom_wc(): Counts words, lines, and characters in a file.
custom_cp(): Copies files.
custom_mv(): Moves or renames files.
custom_rm(): Removes files or directories.

## custom_command.h
This header file contains the function declarations for the utilities implemented in custom_command.c. It provides the necessary interface for other parts of the program (like custom_shell.c) to interact with the functions.

## custom_shell.c
The main program that integrates all of the custom utilities and allows users to execute commands in a shell-like environment. It handles user input, parses commands, and invokes the appropriate utility functions.

