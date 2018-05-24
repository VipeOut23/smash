CC = gcc
SRC = smash.c error.c utils.c

smash: $(SRC)
	$(CC) -o smash $(SRC)
tags:
	etags -a `(ls *.[ch])`

clean:
	rm -f smash
