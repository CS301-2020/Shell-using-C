output: shell.o Commands.o support.o
	gcc shell.o Commands.o support.o -o shell
shell.o: shell.c
	gcc -c shell.c
maintain.o: Commands.c Commands.h
	gcc -c Commands.c	
support.o: support.c support.h
	gcc -c support.c support.h	
clean: 
	rm *.o shell

