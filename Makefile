LIBS = -lncurses
CC = g++
CFLAGS = -Wall

tetris: tetris.cc
	$(CC) tetris.cc $(CFLAGS) $(LIBS)

clean:
	-rm -f *.out
