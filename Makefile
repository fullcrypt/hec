CC=g++
CFLAGS=-c -Wall -Werror
LDFLAGS=-L/usr/lib -lfhew -lfftw3
SOURCES=enc_client.c enc_server.c
OBJECTS=$(SOURCES:.c=.o)
SERVER=serv
CLIENT=cli

all: $(SOURCES) $(SERVER) $(CLIENT) 

$(CLIENT): enc_client.o 
	$(CC) enc_client.o -o cli $(LDFLAGS)

$(SERVER): enc_server.o 
	$(CC) enc_server.o -o serv $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm *o cli serv
