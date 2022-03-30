#include <ncurses.h>
#include <iostream>
#include "Sources/Ball.h"
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <thread>

using namespace std;

struct Box;
void start(Ball *ball, Box* myBox);
void startBox(Box* myBox);
void drawBoard(WINDOW* playWin, Box* myBox);
void ballCollisionWithRectangle(Ball *ball, Box *myBox);

struct Box {
    int y1, x1, length;
    bool goingDown;
    WINDOW* window;
};

vector<thread> ballThreads;
vector<Ball> balls;

int main(int argc, char **argv) {
    srand(time(NULL));
    initscr();
    noecho();
    cbreak();
    curs_set(false);

    WINDOW * playWin = newwin(30, 50, 3, 5);
    box(playWin, 0, 0);
    char models[] = {'O', 'C', 'D', 'E', 'J'};

    Box myBox = {6, 5, 10, true, playWin};
    balls = {
            Ball(playWin, 25, 30, 'l', 5, models[0], 5), //rand() % 10 + 3
            Ball(playWin, 25, 30, 'p', 5, models[1], 6),
            Ball(playWin, 25, 30, 'l', 5, models[2], 7),
            Ball(playWin, 25, 30, 'p', 5, models[3], 3),
            Ball(playWin, 25, 30, 'p', 5, models[4], 4)
    };

    for(int i = 0; i < balls.size(); i++) {
        ballThreads.push_back(thread(start, &(balls[i]), &myBox));
    }
    thread myBoxThread(startBox, &myBox);
    thread drawBoardThread(drawBoard, playWin, &myBox);

    drawBoardThread.join();
    myBoxThread.join();

    for(int i = 0; i < ballThreads.size(); i++) {
        ballThreads[i].join();
        sleep(5);
    }

    getch();
    endwin();
    return 0;
}

void drawBoard(WINDOW* playWin, Box* myBox) {
    while(true) {
        for(int i = 0; i < ballThreads.size(); i++) {
            mvwaddch(playWin, balls[i].getY(), balls[i].getX(), balls[i].getModel());
        }
        for(int j = 0; j < myBox->length; j++) {
            mvwaddch(playWin, myBox->y1, myBox->x1 + j, '#'); //pozioma górna ściana
            mvwaddch(playWin, myBox->y1 + j, myBox->x1,  '#'); //lewa pionowa ściana
            mvwaddch(playWin, myBox->y1 + myBox->length, myBox->x1 + j, '#'); //pozioma dolna ściana
            mvwaddch(playWin, myBox->y1 + j, myBox->x1 + myBox->length - 1, '#'); // prawa pionowa ściana
        }
        wrefresh(playWin);
        fflush(stdout);
        napms(100);
        werase(playWin);
        box(playWin, 0, 0);
    }
}

void start(Ball *ball, Box* myBox) {
    while (true) {
        if(ball->getBounces() >= ball->getMaxBounces()) {
            break;
        }
        if (ball->goingLeft()) {
            ball->moveLeft();
        } else {
            ball->moveRight();
        }
        if (ball->goingUp()) {
            ball->moveUp();
        } else {
            ball->moveDown();
        }
        ballCollisionWithRectangle(ball, myBox);
        usleep((int)(600000 / ball->getSpeed()));
    }
}

void ballCollisionWithRectangle(Ball *ball, Box *myBox) {
    if(ball->goingLeft()) {
        if(ball->getX() <= myBox->x1 + myBox->length + 1 && ball->getX() >= myBox->x1 &&
                ball->getY() >= myBox->y1 && ball->getY() <= myBox->y1 + myBox->length) {
            ball->setIsGoingLeft(false);
        }
    } else if(!ball->goingLeft()) {
        if(ball->getX() >= myBox->x1 - 1 && ball->getX() <= myBox->x1 + myBox->length &&
            ball->getY() >= myBox->y1 && ball->getY() <= myBox->y1 + myBox->length) {
            ball->setIsGoingLeft(true);
        }
    }
    if(ball->goingUp()) {
        if(ball->getY() <= myBox->y1 + myBox->length + 1 && ball->getY() >= myBox->y1 &&
            ball->getX() >= myBox->x1 && ball->getX() <= myBox->x1 + myBox->length) {
            ball->setIsGoingUp(false);
        }
    } else if (!ball->goingUp()) {
        if(ball->getY() >= myBox->y1 - 1 && ball->getY() <= myBox->y1 + myBox->length &&
            ball->getX() >= myBox->x1 && ball->getX() <= myBox->x1 + myBox->length)
            ball->setIsGoingUp(true);
    }
}

void startBox(Box* myBox)
{
    int speedOfBox = rand() % 10 + 1;
    while(true) {
        if(myBox->goingDown) {
            myBox->y1++;

            if(myBox->y1 + myBox->length >= getmaxy(myBox->window) - 1) {
                myBox->goingDown = false;
                speedOfBox = rand() % 10 + 1;
            }
        } else if (!myBox->goingDown) {
            myBox->y1--;
            if(myBox->y1 <= 0) {
                myBox->goingDown = true;
                speedOfBox = rand() % 10 + 1;
            }
        }
        usleep((int)(1200000) / speedOfBox);
    }
}