CC = gcc
all: server klient
clear:
	rm -f klient server
.PHONY: all clear
server: Hra.h Hra.c Displej.h Displej.c CitacVzorov.h CitacVzorov.c Bunka.h Bunka.c Vzor.h Vzor.c arraylist.h arraylist.c server.h server.c
	$(CC) $^ -o $@ -lncurses -pthread
klient: Hra.h Hra.c Displej.h Displej.c CitacVzorov.h CitacVzorov.c Bunka.h Bunka.c Vzor.h Vzor.c arraylist.h arraylist.c client.h client.c
	$(CC) $^ -o $@ -lncurses -pthread