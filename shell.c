
//     General Functionality:
//         Prompt the user for input and read a line at a time from stdin.
//         parse the user input line to separate the command from its arguments.
//         The shell should then fork for each command and the child should execvp that command.
//         The parent process (shell) should wait until the exec'd program exits and then it should go back to step 1.
//         Semicolons act as a separator of commands, so you can have multiple commands on the same line. e.g. echo 1;echo 2;echo 3 Important note: The semicolon is the only symbol that will not be separated by spaces (for easy splitting)
//     The working directory:
//         The command cd is not a program to execvp, your shell has to keep track of the current working directory instead.
//         Look at the chdir() and getcwd() functions
//     The prompt:
//         The prompt should includ the cwd path, e.g. "/home/konstans/Documents/ $"
//         The prompt can shorten the path using the ~ as the home directory. e.g. "~/Documents/ $"
//         You should fflush(stdout); after printing so that a partial lines are displayed and the children don't print a second copy of things. (The shell prompt is a partial line!)
//     Exiting:
//         To quit your shell use the exit command. This is not a program to execute.
//         Ctrl-D (end of file) shoud terminate the shell. Check your fgets() return value.
//     Implement simple redirection.
//         Support both redrection oprators: the > (redirecting stdout) and < (redirecting stdin).
//         This will be explained in class.
//     Implement simple pipes. Only one pipe at a time is required.
//         The pipe operator | redirects one program's output to be another program's input. e.g. ls | wc would run ls and use the output from ls as the input for wc.
//         Check out popen() for the advanced way of doing this, otherwise use a temp file.
//     Compilation, running, and testing:
//         The project should be built using make and a makefile, I will only compile by using make compile which should output an executable file shell.out .
//         The project should be runnable by running ./shell after the make compile command.
//         You must be able to test your shell using the syntax: ./shell.out < lines.txt where lines.txt contains commands on each line.
//         All of the features discussed work in bash, so use it to check out the functionality of each feature if you are unclear about anything.


