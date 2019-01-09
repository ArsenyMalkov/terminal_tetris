LIBS = -lncurses
CC = g++
CFLAGS = -Wall

tetris: tetris.cpp
	$(CC) tetris.c $(CFLAGS) $(LIBS)

clean:
	-rm -f *.out
