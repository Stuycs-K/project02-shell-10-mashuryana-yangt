
// The working directory:
//         The command cd is not a program to execvp, your shell has to keep track of the current working directory instead.
//         Look at the chdir() and getcwd() functions
// Exiting:
//         To quit your shell use the exit command. This is not a program to execute.
//         Ctrl-D (end of file) shoud terminate the shell. Check your fgets() return value.
//  Implement simple pipes. Only one pipe at a time is required.
//         The pipe operator | redirects one program's output to be another program's input. e.g. ls | wc would run ls and use the output from ls as the input for wc.
//         Check out popen() for the advanced way of doing this, otherwise use a temp file.



