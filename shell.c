#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

char **split_line(char *line, const char *delim){
    char **tokens = NULL;
    char *token;
    int count = 0;

    while((token=strsep(&line, delim)) != NULL){
        if(token != '\0'){
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
void execute_command(char *command){
    char **args = split_line(command, " ");
    if (args == NULL || args[0] == NULL){
        free(args);
        return;
    }
    //cd & exit here
}
void execute_line(char *line){ //process line into commands
    char **commands = split_line(line, ";"); //splitter func
    for(int i =0; commands[i]!=NULL; i++){
        execute_command(commands[i]); //command executer func
    }
    free(commands);

}
int main(){
    char directory[1024];
    while(1){
        if (getcwd(directory, sizeof(directory) != NULL)){
            printf("%s $", directory);
            fflush(stdout);
        }
        char line[1024];
        if(fgets(line, 1024, stdin) == NULL){
            printf("Exiting shell");
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