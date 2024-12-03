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
    }

}