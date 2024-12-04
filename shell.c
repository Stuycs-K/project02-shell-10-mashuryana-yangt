#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

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
            //command executer
            
        }
    }
    return 0;

}