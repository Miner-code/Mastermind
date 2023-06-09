# Makefile

all: server player game main

server: server.c
	gcc server.c -o server

player: player.c
	gcc player.c -o player

game: game.c
	gcc game.c -o game

main: main.c
	gcc main.c -o main

clean:
	rm -f server player game main

