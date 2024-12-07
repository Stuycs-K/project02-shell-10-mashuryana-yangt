#ifndef shell_h
#define shell_h


char **split_line(char *line, const char *delim);
void execute_command(char *command);
void execute_line(char *line);

#endif