#include <vector>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>

static const int WIDTH = 12;
static const int HEIGHT = 18;

static const int FILLED_LINE = 8;
static const int BOUNDARY = 9;
static const int EMPTY_SQUARE = 0;

unsigned char *field = nullptr;
std::string tetromino[7];

int rotate(int tetrominoX, int tetrominoY, int rotation) {
    int squareIndex = 0;
    switch (rotation % 4) {
        case 0: 
            squareIndex = tetrominoY * 4 + tetrominoX;
            break;
        case 1: 
            squareIndex = 12 + tetrominoY - (tetrominoX * 4);
            break;
        case 2: 
            squareIndex = 15 - (tetrominoY * 4) - tetrominoX;
            break;
        case 3: 
            squareIndex = 3 - tetrominoY + (tetrominoX * 4);
            break;
    }
    return squareIndex;
}

int randTetromino() {
    return rand() % 7;
}

bool isCollision(int tetrominoIndex, int rotation, int x, int y) {
    for (int tetrominoX = 0; tetrominoX < 4; tetrominoX++) {
        for (int tetrominoY = 0; tetrominoY < 4; tetrominoY++) {
            int squareIndex = rotate(tetrominoX, tetrominoY, rotation);
            int fieldIndex = (y + tetrominoY) * WIDTH + x + tetrominoX;

            if (x + tetrominoX >= 0 && x + tetrominoX < WIDTH) {
                if (y + tetrominoY >= 0 && y + tetrominoY < HEIGHT) {
                    if (tetromino[tetrominoIndex][squareIndex] == 'X' && field[fieldIndex] != 0)
                        return true;
                }
            }
        }
    }

    return false;
}

void drawField() {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            mvaddch(y, x, " ABCDEFG=#"[field[y * WIDTH + x]]);
        }
    }
}

void drawTetromino(int currentTetromino, int currentX, int currentY, int currentRotation) {
    for (int tetrominoX = 0; tetrominoX < 4; tetrominoX++) {
        for (int tetrominoY = 0; tetrominoY < 4; tetrominoY++) {
            if (tetromino[currentTetromino][rotate(tetrominoX, tetrominoY, currentRotation)] == 'X') {
                mvaddch(currentY + tetrominoY, currentX + tetrominoX, currentTetromino + 65);
            }
        }
    }
}

void drawNextTetromino(int nextTetromino) {
    mvprintw(0, WIDTH + 3, "Next tetromino:");

    for (int tetrominoX = 0; tetrominoX < 4; tetrominoX++) {
        for (int tetrominoY = 0; tetrominoY < 4; tetrominoY++) {
            mvaddch(2 + tetrominoY, WIDTH + 7 + tetrominoX, ' ');
        }
    }

    drawTetromino(nextTetromino, WIDTH + 7, 2, 0);
}

void animateFilledLines(std::vector<int> filledLines) {
    if (!filledLines.empty()) {
            refresh();
            usleep(400000);

            for (auto &line : filledLines) {
                for (int x = 1; x < WIDTH - 1; x++) {
                    for (int lineY = line; lineY > 0; lineY--) {
                        field[lineY * WIDTH + x] = field[(lineY - 1) * WIDTH + x];
                        field[x] = EMPTY_SQUARE;
                    }
                }
            }

            filledLines.clear();
        }

}

void initField() {
    field = new unsigned char[WIDTH * HEIGHT];
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            field[y * WIDTH + x] = (x == 0 || x == WIDTH - 1 || y == HEIGHT - 1) ? BOUNDARY : EMPTY_SQUARE;
        }
    }
}

void initTetromino() {
    tetromino[0].append("..X.");
    tetromino[0].append("..X.");
    tetromino[0].append("..X.");
    tetromino[0].append("..X.");

    tetromino[1].append("..X.");
    tetromino[1].append(".XX.");
    tetromino[1].append(".X..");
    tetromino[1].append("....");
    
    tetromino[2].append(".X..");
    tetromino[2].append(".XX.");
    tetromino[2].append("..X.");
    tetromino[2].append("....");
 
    tetromino[3].append("....");
    tetromino[3].append(".XX.");
    tetromino[3].append(".XX.");
    tetromino[3].append("....");
 
    tetromino[4].append("..X.");
    tetromino[4].append(".XX.");
    tetromino[4].append("..X.");
    tetromino[4].append("....");
 
    tetromino[5].append("....");
    tetromino[5].append(".XX.");
    tetromino[5].append("..X.");
    tetromino[5].append("..X.");
 
    tetromino[6].append("....");
    tetromino[6].append(".XX.");
    tetromino[6].append(".X..");
    tetromino[6].append(".X..");
}

int main() { 
    initTetromino();
    initField();
  
    int currentTetromino = randTetromino();
    int nextTetromino = randTetromino();
    int currentRotation = 0;
    int currentX = WIDTH / 2;
    int currentY = 0;

    bool gameOver = false;

    int speed = 20;
    int speedCounter = 0;
    bool tetrominoFallDown = false;
    int tetrominoCount = 0;
    int score = 0;
    int formedLines = 0;

    std::vector<int> filledLines;

    initscr();

    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    mvprintw(HEIGHT + 5, 2, "a, s, d - movements; space - rotation, q - exit");

    while (!gameOver) {
        speedCounter++;
        tetrominoFallDown = speedCounter == speed;

        int ch = getch();

        if (ch != ERR) {
            if (ch == 'q') {
                gameOver = true;
            }
            currentX -= (ch == 'a' && !isCollision(currentTetromino, currentRotation, currentX - 1, currentY)) ? 1 : 0;
            currentX += (ch == 'd' && !isCollision(currentTetromino, currentRotation, currentX + 1, currentY)) ? 1 : 0;
            currentY += (ch == 's' && !isCollision(currentTetromino, currentRotation, currentX, currentY + 1)) ? 1 : 0;
            if (ch == ' ') {
                currentRotation += (isCollision(currentTetromino, currentRotation + 1, currentX, currentY)) ? 0 : 1;
            }
        }
        
        if (tetrominoFallDown) {
            if (isCollision(currentTetromino, currentRotation, currentX, currentY + 1)) {
                for (int tetrominoX = 0; tetrominoX < 4; tetrominoX++) {
                    for (int tetrominoY = 0; tetrominoY < 4; tetrominoY++) {
                        if (tetromino[currentTetromino][rotate(tetrominoX, tetrominoY, currentRotation)] == 'X') {
                            field[(currentY + tetrominoY) * WIDTH + (currentX + tetrominoX)] = currentTetromino + 1;
                        }
                    }
                }

                tetrominoCount++;
                if (tetrominoCount % 10 == 0) {
                    if (speed >= 10) speed--;
                }

                for (int tetrominoY = 0; tetrominoY < 4; tetrominoY++) {
                    if (currentY + tetrominoY < HEIGHT - 1) {
                        bool isLineFilled = true;
                        for (int tetrominoX = 1; tetrominoX < WIDTH - 1; tetrominoX++) {
                            isLineFilled &= (field[(currentY + tetrominoY) * WIDTH + tetrominoX]) != EMPTY_SQUARE;
                        }

                        if (isLineFilled) {
                            for (int tetrominoX = 1; tetrominoX < WIDTH - 1; tetrominoX++) {
                                field[(currentY + tetrominoY) * WIDTH + tetrominoX] = FILLED_LINE;
                            }

                            filledLines.push_back(currentY + tetrominoY);
                        }
                    }
                }

                score += 25;
                if (!filledLines.empty()) {
                    formedLines += filledLines.size();
                    score += (1 << filledLines.size()) * 100;
                }

                currentX = WIDTH / 2;
                currentY = 0;
                currentRotation = 0;
                currentTetromino = nextTetromino;
                nextTetromino = randTetromino();

                gameOver = isCollision(currentTetromino, currentRotation, currentX, currentY);
            } else {
                currentY++;
            }

            speedCounter = 0;
        }

        drawField();
        drawTetromino(currentTetromino, currentX, currentY, currentRotation);
        drawNextTetromino(nextTetromino);
        animateFilledLines(filledLines);

        mvprintw(HEIGHT - 1, WIDTH + 5, "Formed lines: %3d", formedLines);
        mvprintw(HEIGHT - 2, WIDTH + 5, "Score: %10d", score);
        
        refresh();
        usleep(50000);
    }

    endwin();

    std::cout << "Game Over! Score: " << score << std::endl;

    return 0;
}
