CC = gcc
SRC = smash.c error.c utils.c parser.c

smash: $(SRC)
	$(CC) -lbsd -o smash $(SRC)
tags:
	etags -a `(ls *.[ch])`

clean:
	rm -f smash
