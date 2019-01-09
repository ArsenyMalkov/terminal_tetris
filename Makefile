LIBS = -lncurses
CC = g++
CFLAGS = -Wall

tetris: tetris.c
	$(CC) tetris.c $(CFLAGS) $(LIBS)

clean:
	-rm -f *.out
