#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>  // for chdir() and mkdir() functions
#include <process.h>  // for executing commands
#include <windows.h>


void userInput(char *input) {
    printf("Please enter input:\n");
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';
    printf("Your command is: %s\n", input);

    int isValid = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isspace((unsigned char)input[i])) {
            isValid = 1;
            break;
        }
    }

    if (!isValid) {
        printf("Invalid input. Please try again.\n");
        userInput(input);
    }
}

void tokenize(char *command) {
    char *token = strtok(command, " ");

    while (token != NULL) {
        printf("%s\n", token);

        if (strcmp(token, "cd") == 0) {
            token = strtok(NULL, " ");
            if (token == NULL) {
                fprintf(stderr, "cd: missing directory argument\n");
            } else {
                if (_chdir(token) != 0) {
                    perror("cd");
                }
            }
        } else if (strcmp(token, "history") == 0) {
            printf("Command history:\n");
            // Implement code to display command history
        } else if (strcmp(token, "pwd") == 0) {
            char cwd[256];
            if (_getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Current working directory: %s\n", cwd);
            } else {
                perror("pwd");
            }
        } else if (strcmp(token, "echo") == 0) {
            token = strtok(NULL, "");
            if (token == NULL) {
                fprintf(stderr, "echo: missing text argument\n");
            } else {
                printf("%s\n", token);
            }
        } else if (strcmp(token, "help") == 0) {
            printf("Available commands:\n");
            printf("  cd <directory>   Change directory\n");
            printf("  ls               List files in the current directory\n");
            printf("  cat <file>       Read and display contents\n");
            printf("  mkdir <directory> Create a new directory\n");
            printf("  rm <file or directory> Removes a file or directory\n");
            printf("  echo <input>  Returns the input as output\n");
            printf("  pwd           Returns the current directory \n");
        } else if (strcmp(token, "cat") == 0) {
            token = strtok(NULL, " ");
            if (token == NULL) {
                fprintf(stderr, "cat: missing file argument\n");
            } else {
                FILE* file = fopen(token, "r");
                if (file == NULL) {
                    perror("cat");
                } else {
                    char line[256];
                    while (fgets(line, sizeof(line), file)) {
                        printf("%s", line);
                    }
                    fclose(file);
                }
            }
        } else if (strcmp(token, "ls") == 0) {
            system("dir");
        } else if (strcmp(token, "rm") == 0) {
            token = strtok(NULL, " ");
            if (token == NULL) {
                fprintf(stderr, "rm: missing file argument\n");
            } else {
                if (remove(token) == 0) {
                    printf("File removed: %s\n", token);
                } else {
                    perror("rm");
                }
            }
        } else if (strcmp(token, "mkdir") == 0) {
            token = strtok(NULL, " ");
            if (token == NULL) {
                fprintf(stderr, "mkdir: missing directory argument\n");
            } else {
                if (_mkdir(token) != 0) {
                    perror("mkdir");
                }
            }
        } else {
            // CreateProcess variables
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            // Create the command string
            char commandStr[100];
            snprintf(commandStr, sizeof(commandStr), "%s", token);

            // Create the child process
            if (!CreateProcess(NULL, commandStr, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                printf("Failed to execute command: %s\n", token);
                break;
            }

            // Wait for the child process to finish
            WaitForSingleObject(pi.hProcess, INFINITE);

            // Close process and thread handles
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        token = strtok(NULL, " ");
    }
}

int main() {
    char input[100];
    userInput(input);
    tokenize(input);

    return 0;
}

