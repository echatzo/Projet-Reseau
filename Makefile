CC=gcc
LFLAGS=-lz
CFLAGS=-g -W -Wall -DNDEBUG
SOURCES=$(wildcard src/*.c)
HEADERS=$(SOURCES:.c=.h)
OBJ=$(SOURCES:.c=.o)
ROBJ=src/create_socket.o src/packet_implem.o src/read_write_loop.o src/real_address.o src/receiver.o src/wait_for_client.o
SOBJ=src/create_socket.o src/packet_implem.o src/read_write_loop.o src/real_address.o src/sender.o src/wait_for_client.o


all:	sender
		  receiver

src/%.o: %.c $(HEADER)
				@echo "$^"
				$(CC) $(LFLAGS) -c $< $(CFLAGS)

receiver: $(ROBJ)
				@echo "Begin building receiver"
				$(CC) -o $@ $^ $(LFLAGS) $(CFLAGS)

sender: $(SOBJ)
				@echo "Begin building sender"
				$(CC) -o $@ $^ $(LFLAGS) $(CFLAGS)

clean:
	(cd src; rm -f *.o)
	@echo "cleaning"
