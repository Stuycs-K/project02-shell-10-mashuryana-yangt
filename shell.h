#ifndef shell_h
#define shell_h

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

#endif