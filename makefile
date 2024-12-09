.PHONY: clean run compile
all: clean compile run
clean: 
	@rm -f *.o shell
compile: shell
	@gcc -o shell shell.o
shell.o: shell.c shell.h
	@gcc -c shell.c -o shell.o
shell: shell.o
	@gcc shell.o -o shell
run: compile
	@./shell $(ARGS)