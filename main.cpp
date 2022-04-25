#include <ncurses.h>
#include <iostream>
#include "Sources/Ball.h"
#include <unistd.h>
#include <cstdlib>
#include <thread>
#include <list>

struct Box;
void start(Ball *ball, Box* myBox);
void startBox(Box* myBox);
void drawBoard(WINDOW* playWin, Box* myBox);
void ballCollisionWithRectangle(Ball *ball, Box *myBox);
void detectKeyPressAndCloseApp(WINDOW* win);

struct Box {
    int y1, x1, length;
    bool goingDown;
    WINDOW* window;
};


// kulki wpadają do prostokąta i w nim się odbijają 3 razy, po 3 odbicu wychodzą z prostokąta,
// gdy kulka jest w prostokącie to prostokąt się nie porusza

std::list<Ball> balls;
bool quit = false;

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

    std::thread myBoxThread(startBox, &myBox);
    std::thread drawBoardThread(drawBoard, playWin, &myBox);
    std::thread keyboardInputThread(detectKeyPressAndCloseApp, playWin);
    int counter = 0;
    std::list<std::thread> ballThreads;
    while(!quit) {
        if(counter == INT32_MAX) {
            counter = 0;
        }

        balls.push_back(Ball(playWin, 25, 30, 'l', 5, models[counter++%4], rand() % 10 + 1));
        ballThreads.push_back(std::thread(start, &(balls.back()), &myBox));
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        balls.push_back(Ball(playWin, 25, 30, 'p', 5, models[counter%4+1], rand() % 10 + 1));
        ballThreads.push_back(std::thread(start, &(balls.back()), &myBox));
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    }

    for(auto & i : ballThreads) {
        i.join();
    }
    keyboardInputThread.join();
    drawBoardThread.join();
    myBoxThread.join();

    endwin();
    return 0;
}

void drawBoard(WINDOW* playWin, Box* myBox) {
    while(true) {

        if(quit) {
            break;
        }

        for(Ball ball : balls) {
            mvwaddch(playWin, ball.getY(), ball.getX(), ball.getModel());
        }
        for(int j = 0; j < myBox->length; j++) {
            mvwaddch(playWin, myBox->y1, myBox->x1 + j, '#'); //pozioma górna ściana
            mvwaddch(playWin, myBox->y1 + j, myBox->x1,  '#'); //lewa pionowa ściana
            mvwaddch(playWin, myBox->y1 + myBox->length, myBox->x1 + j, '#'); //pozioma dolna ściana
            mvwaddch(playWin, myBox->y1 + j, myBox->x1 + myBox->length - 1, '#'); // prawa pionowa ściana
        }
        std::this_thread::sleep_for(std::chrono::milliseconds (10));
        wrefresh(playWin);
        werase(playWin);
        box(playWin, 0, 0);
    }
}


void detectKeyPressAndCloseApp(WINDOW* win) {
    int ch = getch();

    if(ch == 113 /*q*/) {
        quit = true;
    }
}

void start(Ball *ball, Box* myBox) {
//    std::this_thread::sleep_for(std::chrono::seconds(seconds++));
    while (true) {
        if(ball->getBounces() >= ball->getMaxBounces() || quit) {
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
        std::this_thread::sleep_for(std::chrono::microseconds((int)600000 / ball->getSpeed()));
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

        if(quit) {
            break;
        }

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
        std::this_thread::sleep_for(std::chrono::microseconds((int)1200000 / speedOfBox));
    }
}