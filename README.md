[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)
# Systems Project2 - MyShell
Names: Angela Mashuryan and Tiffany Yang


Creative Group Name: Yangela

Code/Documentation:

The shell can execute standard commands with arguments such as echo and ls. Arguments are parsed and passed to the execvp() function for executing. 

Input (<) and output(>) redirection are supported:
cmd > file.txt: Redirects output of cmd to a file(file.txt)
cmd < file.txt: Takes the input from a file (file.txt) for cmd.

Simple piping (|) is supported between two commands, such as ls | wc -l where the output of ls is passed as the input to wc.

Built-in commands include cd for changing directories and exit for terminating the shell.

The shell prompt displays the current working directory and uses ~ for the home directory.

Function Headers:
char **split_line(char *line, const char *delim);
void mychdir(char *path);
void mygetcwd();
void redirectOut(char *newOut);
void redirectIn(char *filename);
void handle_redirection(char **args);
void execute_command(char *command);
void handle_pipe(char *input);
void execute_line(char *line);
int main();


// Due: Monday 12/09 8:00 am
