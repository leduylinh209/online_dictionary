CC=gcc

all: server client

server: server.o
	$(CC) server.o -o server

server.o: server.c
	$(CC) -c server.c

client: client.o
	$(CC) client.o -o client -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

client.o: client.c
	$(CC) -c client.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic


