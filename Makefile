LIBS = -lncurses
CC = g++
CFLAGS = -Wall

tetris: tetris.cpp
	$(CC) tetris.cpp $(CFLAGS) $(LIBS)

clean:
	-rm -f *.out
