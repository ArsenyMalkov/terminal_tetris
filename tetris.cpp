#include <vector>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>

static const int width = 12;
static const int height = 18;

static const int FILLED_LINE = 8;
static const int BOUNDARY = 9;
static const int EMPTY_SQUARE = 0;

unsigned char *field = nullptr;
std::string tetromino[7];

//hello

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
            int fieldIndex = (y + tetrominoY) * width + x + tetrominoX;

            if (x + tetrominoX >= 0 && x + tetrominoX < width) {
                if (y + tetrominoY >= 0 && y + tetrominoY < height) {
                    if (tetromino[tetrominoIndex][squareIndex] == 'X' && field[fieldIndex] != 0)
                        return true;
                }
            }
        }
    }

    return false;
}

void drawField() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            mvaddch(y, x, " ABCDEFG=#"[field[y * width + x]]);
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
    mvprintw(0, width + 3, "Next tetromino:");

    for (int tetrominoX = 0; tetrominoX < 4; tetrominoX++) {
        for (int tetrominoY = 0; tetrominoY < 4; tetrominoY++) {
            mvaddch(2 + tetrominoY, width + 7 + tetrominoX, ' ');
        }
    }

    drawTetromino(nextTetromino, width + 7, 2, 0);
}

void animateFilledLines(std::vector<int> filledLines) {
    if (!filledLines.empty()) {
            refresh();
            usleep(400000);

            for (auto &line : filledLines) {
                for (int x = 1; x < width - 1; x++) {
                    for (int lineY = line; lineY > 0; lineY--) {
                        field[lineY * width + x] = field[(lineY - 1) * width + x];
                        field[x] = EMPTY_SQUARE;
                    }
                }
            }

            filledLines.clear();
        }

}

void initField() {
    field = new unsigned char[width * height];
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            field[y * width + x] = (x == 0 || x == width - 1 || y == height - 1) ? BOUNDARY : EMPTY_SQUARE;
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
    int currentX = width / 2;
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

    mvprintw(height + 5, 2, "a, s, d - movements; space - rotation, q - exit");

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
                            field[(currentY + tetrominoY) * width + (currentX + tetrominoX)] = currentTetromino + 1;
                        }
                    }
                }

                tetrominoCount++;
                if (tetrominoCount % 10 == 0) {
                    if (speed >= 10) speed--;
                }

                for (int tetrominoY = 0; tetrominoY < 4; tetrominoY++) {
                    if (currentY + tetrominoY < height - 1) {
                        bool isLineFilled = true;
                        for (int tetrominoX = 1; tetrominoX < width - 1; tetrominoX++) {
                            isLineFilled &= (field[(currentY + tetrominoY) * width + tetrominoX]) != EMPTY_SQUARE;
                        }

                        if (isLineFilled) {
                            for (int tetrominoX = 1; tetrominoX < width - 1; tetrominoX++) {
                                field[(currentY + tetrominoY) * width + tetrominoX] = FILLED_LINE;
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

                currentX = width / 2;
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

        mvprintw(height - 1, width + 5, "Formed lines: %3d", formedLines);
        mvprintw(height - 2, width + 5, "Score: %10d", score);
        
        refresh();
        usleep(50000);
    }

    endwin();

    std::cout << "Game Over! Score: " << score << std::endl;

    return 0;
}
