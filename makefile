CC = gcc
CFLAGS = -Wall -Wextra

all: server jeu_mastermind

server: server.c server.h
	$(CC) $(CFLAGS) -o server server.c

jeu_mastermind: ihm.c server.c server.h
	$(CC) $(CFLAGS) -o jeu_mastermind ihm.c server.c

clean:
	rm -f server jeu_mastermind

