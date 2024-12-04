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
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void mychdir()
void mygetcwd()

//fgets() when ctrl D'd
void run(){
    
}

void main(char* args){
    pid
    //seperate args if there's a | into an array
    for (args in args{
        run(args)
    })
    popen();
}
int redirectOut(char * newOut){
  int fd1 = open(newOut, O_WRONLY | O_TRUNC | O_CREAT);
  int stdout = STDOUT_FILENO;
  int backup_stdout = dup( stdout ); // save stdout for later
  dup2(fd1, stdout); 
  return backup_stdout;
}
void exit_process(int backup_stdout){
  fflush(stdout);
  int stdout = STDOUT_FILENO;
  dup2(backup_stdout, stdout);
}