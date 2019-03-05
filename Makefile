LIBS = -lncurses
CC = gcc
CFLAGS = -Wall

tetris: tetris.c
	$(CC) tetris.c $(CFLAGS) $(LIBS)

clean:
	-rm -f *.out
