CC=gcc
LFLAGS=-lz
CFLAGS=-g -W -Wall -DNDEBUG
SOURCES=$(wildcard /src/*.c)
HEADERS=$(SOURCES:.c=.h)
OBJ=$(SOURCES:.c=.o)


all:	sender
		  receiver

%.o: %.c $(HEADER)
				@echo "$^"
				$(CC) $(LFLAGS) -c $< $(CFLAGS)

receiver: $(OBJ)
				@echo "Begin building receiver"
				$(CC) -o $@ $^ $(LIBR) $(LFLAGS) $(CFLAGS)

sender: $(OBJ)
				@echo "Begin building receiver"
				$(CC) -o $@ $^ $(LIBR) $(LFLAGS) $(CFLAGS)

clean:
				@echo "Accessing src for cleaning"
				(cd src; rm	-rf	*.o)
