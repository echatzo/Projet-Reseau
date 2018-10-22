CC = gcc -Wall -g

all:	sender
		receiver
	    make clean

receiver:	receiver.o
			$(CC)	-o	receiver	receiver.o	-lz

receiver.o:	receiver.c
			$(CC)	-c	src/receiver.c	-lz

sender:	sender.o
		$(CC)	-o	receiver	receiver.o	-lz

sender.o:	sender.c
			$(CC)	-c	src/sender.c	-lz

clean:
		rm	-rf	.o

.SILENT:
