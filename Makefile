CC = gcc -Wall -g

all:	main
	    make clean

main:	main.o
			$(CC)	-o	main	main.o	-lz

main.o:	main.c
				$(CC)	-c	main.c	-lz

clean:
			rm	-rf	.o

.SILENT:
