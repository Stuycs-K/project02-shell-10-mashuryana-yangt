.PHONY: clean run compile

clean: 
	@rm -f *.o shell
compile: shell
	@gcc -o shell
shell: shell.c
	@gcc -c shell.c shell.h
run: compile
	@./shell $(ARGS)
