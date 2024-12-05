// The working directory:
//         The command cd is not a program to execvp, your shell has to keep track of the current working directory instead.
//         Look at the chdir() and getcwd() functions
// Exiting:
//         To quit your shell use the exit command. This is not a program to execute.
//         Ctrl-D (end of file) shoud terminate the shell. Check your fgets() return value.
//  Implement simple pipes. Only one pipe at a time is required.
//         The pipe operator | redirects one program's output to be another program's input. e.g. ls | wc would run ls and use the output from ls as the input for wc.
//         Check out popen() for the advanced way of doing this, otherwise use a temp file.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// Change the directory
void mychdir(char *path) {
    path[strcspn(path, "\n")] = '\0'; 
    if (chdir(path) != 0) {
        perror("chdir failed");
    }
}

// Print the directory
void mygetcwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd failed");
    }
}


void handle_exit(char *input) {
    if (input == NULL || strcmp(input, "exit\n") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }
}

void run(char *cmd) {
    char *args[64];
    int i = 0;
    args[i] = strtok(cmd, " \t\n");
    while (args[i] != NULL) {
        args[++i] = strtok(NULL, " \t\n");
    }

    if (args[0] == NULL) return; // Skip empty input

    if (fork() == 0) {
        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
}

// | 
void handle_pipe(char *input) {
    char *cmd1 = strtok(input, "|");
    char *cmd2 = strtok(NULL, "|");

    if (cmd1 == NULL || cmd2 == NULL) {
        fprintf(stderr, "Invalid pipe syntax\n");
        return;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return;
    }

    if (fork() == 0) {
        // Child 1: Output to pipe
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        run(cmd1);
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) {
        // Child 2: Input from pipe
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        run(cmd2);
        exit(EXIT_FAILURE);
    }

    // Parent: Close pipe and wait
    close(pipefd[0]);
    close(pipefd[1]);
    wait(NULL);
    wait(NULL);
}

// Redirect output to a file
int redirectOut(char *newOut) {
    int fd = open(newOut, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open failed");
        return -1;
    }
    int backup_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return backup_stdout;
}

void restore_stdout(int backup_stdout) {
    dup2(backup_stdout, STDOUT_FILENO);
    close(backup_stdout);
}


int main() {
    char input[1024];

    while (1) {
        printf("shell> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break; // Handle Ctrl+D
        }

        handle_exit(input);

        if (strncmp(input, "cd ", 3) == 0) {
            mychdir(input + 3);
            continue;
        }
        if (strcmp(input, "pwd\n") == 0) {
            mygetcwd();
            continue;
        }

        if (strchr(input, '|')) {
            handle_pipe(input);
            continue;
        }

        run(input);
    }

    return 0;
}
