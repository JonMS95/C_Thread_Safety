src_main 	= Source_files/main.c

exe_main	= Executable_files/main

shell_dirs	= Shell_files/directories.sh
shell_test	= Shell_files/test.sh

# all: directories clean main mult.o div.o mult.so div.so rm_obj msg test
all: directories clean main msg test

directories:
	@./$(shell_dirs)

main: $(src_main)
	gcc -g $(src_main) -o $(exe_main) -lpthread

clean:
	rm -rf Executable_files/*

msg:
	@echo "**************************************************************************************";
	@echo "Note that the directory were dynamic libraries are stored may differ!"
	@echo "If the path to those is different it may be changed by giving another value to the"
	@echo "MULT_SO_PATH or DIV_SO_PATH 'defines' (which are both found in Source_files/main.h)"
	@echo "**************************************************************************************";

test:
	@./$(shell_test)