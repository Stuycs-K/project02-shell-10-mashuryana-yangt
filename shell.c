#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shell.h"

/**
 * Splits a line into tokens using a delimiter.
 * @param line Input string to split.
 * @param delim Delimiter for splitting.
 * @return Array of tokens (NULL-terminated).
 */
char **split_line(char *line, const char *delim) {
    char **tokens = NULL;
    char *token;
    int count = 0;

    while ((token = strsep(&line, delim)) != NULL) {
        if (token[0] != '\0') {
            tokens = realloc(tokens, (count + 2) * sizeof(char *));
            if (!tokens) {
                perror("realloc");
                exit(1);
            }
            tokens[count++] = token;
        }
    }
    if (tokens != NULL) {
        tokens[count] = NULL;
    }
    return tokens;
}

/**
 * Changes the current working directory.
 * @param path Directory to change to.
 */
void mychdir(char *path) {
    path[strcspn(path, "\n")] = '\0'; // Remove newline
    if (chdir(path) != 0) {
        perror("chdir failed");
    }
}

/**
 * Prints the current working directory.
 * Uses "~" for the root directory.
 */
void mygetcwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if (strcmp(cwd, "/") == 0) {
            printf("~");
        } else {
            printf("%s", cwd);
        }
    } else {
        perror("getcwd failed");
    }
}

/**
 * Redirects stdout to a file.
 * @param newOut File for output redirection.
 */
void redirectOut(char *newOut) {
    int fd = open(newOut, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

/**
 * Redirects stdin from a file.
 * @param filename File for input redirection.
 */
void redirectIn(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

/**
 * Handles input/output redirection in a command.
 * Removes redirection operators from the command.
 * @param args Array of command arguments.
 */
void handle_redirection(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            redirectOut(args[i + 1]);
            args[i] = NULL;
            break;
        } else if (strcmp(args[i], "<") == 0) {
            redirectIn(args[i + 1]);
            args[i] = NULL;
            break;
        }
    }
}

/**
 * Executes a single command.
 * Handles built-in commands like "cd" and "exit".
 * @param command Command to execute.
 */
void execute_command(char *command) {
    char **args = split_line(command, " ");
    if (args == NULL || args[0] == NULL) {
        free(args);
        return;
    }
    handle_redirection(args);
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] != NULL) {
            mychdir(args[1]);
        } else {
            printf("cd missing argument\n");
        }
        free(args);
        return;
    }
    if (strcmp(args[0], "exit") == 0) {
        printf("Exiting shell...\n");
        free(args);
        exit(0);
    }
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork failed");
    }
    free(args);
}

/**
 * Executes a command with a pipe.
 * Splits the command into two parts and connects them.
 * @param input Command string with a pipe.
 */
void handle_pipe(char *input) {
    char *cmd1 = strsep(&input, "|");
    char *cmd2 = input;

    if (cmd1 == NULL || cmd2 == NULL) {
        fprintf(stderr, "Invalid pipe syntax\n");
        return;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // First command: write to pipe
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        char **args1 = split_line(cmd1, " ");
        handle_redirection(args1);
        execvp(args1[0], args1);
        perror("execvp failed");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Second command: read from pipe
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        char **args2 = split_line(cmd2, " ");
        handle_redirection(args2);
        execvp(args2[0], args2);
        perror("execvp failed");
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    wait(NULL);
    wait(NULL);
}

/**
 * Splits and executes multiple commands in a line.
 * Handles commands with semicolons or pipes.
 * @param line Input line of commands.
 */
void execute_line(char *line) {
    char **commands = split_line(line, ";");
    for (int i = 0; commands[i] != NULL; i++) {
        if (strchr(commands[i], '|')) {
            handle_pipe(commands[i]);
        } else {
            execute_command(commands[i]);
        }
    }
    free(commands);
}

/**
 * Main loop of the shell.
 * Displays a prompt, reads input, and executes commands.
 */
int main() {
    while (1) {
        mygetcwd();
        printf(" $");
        fflush(stdout);

        char line[1024];
        if (fgets(line, 1024, stdin) == NULL) {
            printf("\nExiting shell...\n");
            break;
        }
        line[strcspn(line, "\n")] = '\0';
        if (line[0] != '\0') {
            execute_line(line);
        }
    }
    return 0;
}
