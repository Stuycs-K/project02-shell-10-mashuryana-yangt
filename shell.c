#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shell.h"

char **split_line(char *line, const char *delim){
    char **tokens = NULL;
    char *token;
    int count = 0;

    while((token=strsep(&line, delim)) != NULL){
        if(token[0] != '\0'){
            tokens = realloc(tokens, (count+2) *sizeof(char *));
            if(!tokens){
                perror("realloc");
                exit(1);
            }
            tokens[count++] = token;
        }
    }
    if(tokens != NULL){
        tokens[count] = NULL;
    }
    return tokens;
}

void mychdir(char *path) { //added for merging
    path[strcspn(path, "\n")] = '\0'; 
    if (chdir(path) != 0) {
        perror("chdir failed");
    }
}
void mygetcwd() { //added for merging
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if(strcmp(cwd, "/") == 0){ //added
            printf("~");
        }
        else{
        printf("%s", cwd);
        }
    } else {
        perror("getcwd failed");
    }
}

void redirectOut(char *newOut) { //added for merge 
    int fd = open(newOut, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }
    // int backup_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    // return backup_stdout;
}

void redirectIn(char *filename){
    int fd = open(filename, O_RDONLY);
    if (fd < 0){
        perror("open failed");
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void handle_redirection(char **args){
    for (int i= 0; args[i] != NULL; i++){
        if (strcmp(args[i], ">")== 0){
            redirectOut(args[i+1]); 
            args[i] = NULL; 
            break;
        } else if(strcmp(args[i], "<")== 0){
            redirectIn(args[i+1]); 
            args[i] = NULL; 
            break;
        }
    }
}

void execute_command(char *command){
    char **args = split_line(command, " ");
    if (args == NULL || args[0] == NULL){
        free(args);
        return;
    }
    handle_redirection(args);
    // if (strchr(command, '>')){
    //     char *output = args[2];
    //     redirectOut(output);
    //     args[2] = NULL;
    // }
    // else if(strchr(command, '<')){
    //     char *input = args[2];
    //     redirectIn(input);
    //     args[2] = NULL;
    // }
    if (strcmp(args[0], "cd") == 0){
        if(args[1] != NULL){
            mychdir(args[1]);
        }
        else{
            printf("cd missing argument\n");
        }
        free(args);
        return;
        }
    if(strcmp(args[0], "exit") == 0){
        printf("Exiting shell...\n");
        free(args);
        exit(0);
    }
       pid_t pid = fork();
       if(pid == 0){
        execvp(args[0], args);
        perror("execvp");
        exit(1);
       }
       else if(pid >0){
        int status;
        wait(&status);
        // if (WIFEXITED(status)){
        //     printf("Process exited with status: %d\n", WEXITSTATUS(status));
        // }
       }
       else{
        perror("Fork");
       }
    
    free(args);
}

void handle_pipe(char *input) { //added for merge
    char *cmd1 = strsep(&input, "|"); //strsep instead of strtok
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
    pid_t pid1 = fork();    // changed this part a bit
    int status;
    if (pid1 == 0) {
        // Child 1: Output to pipe
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // run(cmd1);
        // exit(EXIT_FAILURE);

        char **args1 = split_line(cmd1, " ");
        handle_redirection(args1);
        execvp(args1[0], args1);
        perror("execvp failed for the first command");
        exit(1);

    }
    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child 2: Input from pipe
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        // run(cmd2);
        // exit(EXIT_FAILURE);
        char **args2 = split_line(cmd2, " ");
        handle_redirection(args2);
        execvp(args2[0], args2);
        perror("execvp failed for the second command");
        exit(1);
    }

    // Parent: Close pipe and wait
    close(pipefd[0]);
    close(pipefd[1]);
    wait(&status);
    // if(WIFEXITED(status)){
    //     printf("First child exited with status %d\n", WEXITSTATUS(status));
    // }
    wait(&status);
    // if(WIFEXITED(status)){
    //     printf("First child exited with status %d\n", WEXITSTATUS(status));
    // }
}

void execute_line(char *line){ //process line into commands
    char **commands = split_line(line, ";"); //splitter func
    for(int i =0; commands[i]!=NULL; i++){
        if(strchr(commands[i], '|')){
            handle_pipe(commands[i]);
        }
        else{
        execute_command(commands[i]); //command executer func
    }
    }
    free(commands);

}
int main(){
    char directory[1024];
    while(1){
        mygetcwd();
            printf(" $");
            fflush(stdout);
        
        char line[1024];
        if(fgets(line, 1024, stdin) == NULL){
            printf("\nExiting shell...\n");
            break;
        }
        int length = 0;
        while(line[length] != '\0'){
            length++;
        }
        if (length>0 && line[length-1] == '\n'){
            line[length-1] = '\0';
        }
        if(line[0] != '\0'){
            execute_line(line); // line executer

        }
    }
    return 0;

}