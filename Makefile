CC=g++
CFLAGS=-c -Wall
LDFLAGS=-L/usr/lib -lfhew -lfftw3 libfhew-extensions.a libfhew.a -lpthread -lfftw3 common.o
SOURCES=enc_client.c enc_server.c
OBJECTS=$(SOURCES:.c=.o)
SERVER=serv
CLIENT=cli

all: $(SOURCES) $(SERVER) $(CLIENT)

$(CLIENT): enc_client.c common.h
	$(CC) enc_client.c -o cli $(LDFLAGS)

$(SERVER): enc_server.c common.h
	$(CC) enc_server.c -o serv $(LDFLAGS)


clean:
	rm *o cli serv
