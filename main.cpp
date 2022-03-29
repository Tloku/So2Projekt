#include <ncurses.h>
#include <iostream>
#include "Sources/Ball.h"
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <random>

struct Box;
void* start(void* args);
void* rectangle(void *args);
void ballCollisionWithRectangle(Ball *ball, Box *myBox);

struct Box {
    int y1, x1, length;
    bool goingDown;
    WINDOW* window;
};

struct thread_arg_struct {
    Ball *ball;
    Box *myBox;
};

int main(int argc, char **argv) {
    srand(time(NULL));
    initscr();
    noecho();
    cbreak();
    curs_set(false);
    const int NUMBER_OF_BALLS = 5;

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW * playWin = newwin(30, 50, 3, 5);
    box(playWin, 0, 0);
    refresh();
    wrefresh(playWin);
    std::vector<pthread_t> threads(NUMBER_OF_BALLS);
    pthread_t boxThread;
    Box myBox = {6, 5, 10, true, playWin};

    std::vector<Ball> balls = {
            Ball(playWin, 25, 15, 'l', 5, "o", 5), //rand() % 10 + 3
            Ball(playWin, 25, 15, 'p', 5, "c", 5),
            Ball(playWin, 25, 15, 'l', 5, "d", 5),
            Ball(playWin, 25, 15, 'p', 5, "e", 5),
            Ball(playWin, 25, 15, 'p', 5, "j", 5)
    };

    for(int i = 0; i < NUMBER_OF_BALLS; i++) {
        struct thread_arg_struct args = {&balls[i], &myBox};
        pthread_create(&threads[i], nullptr, &start, (void*)&args);
    }
    pthread_create(&boxThread, nullptr, &rectangle, &myBox);

    while(true) {
        for(int i = 0; i < balls.size(); i++) {
            if(balls[i].getBounces() == balls[i].getMaxBounces()) {
                std::swap(balls[i], balls[balls.size()-1]);
                std::swap(threads[i], threads[threads.size()-1]);

                pthread_cancel(threads[threads.size() - 1]);
                balls.pop_back();
            }
            mvwprintw(playWin, balls[i].getY(), balls[i].getX(), balls[i].getModel());

            for(int j = 0; j < myBox.length; j++) {
                mvwprintw(playWin, myBox.y1, myBox.x1 + j, "#"); //pozioma górna ściana
                mvwprintw(playWin, myBox.y1 + j, myBox.x1,  "#"); //lewa pionowa ściana
                mvwprintw(playWin, myBox.y1 + myBox.length, myBox.x1 + j, "#"); //pozioma dolna ściana
                mvwprintw(playWin, myBox.y1 + j, myBox.x1 + myBox.length - 1, "#"); // prawa pionowa ściana
            }
        }
        wrefresh(playWin);
        fflush(stdout);
        napms(100);
        werase(playWin);
        box(playWin, 0, 0);
    }

    getch();
    endwin();
    return 0;
}

void* start(void* argss) {
    struct thread_arg_struct *args = (struct thread_arg_struct *)argss;
    Ball *ball = args->ball;
    Box *myBox = args->myBox;
    while (ball->getBounces() != ball->getMaxBounces()) {
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
        usleep((int)(1000000 / ball->getSpeed()));
    }
}

void ballCollisionWithRectangle(Ball *ball, Box *myBox) {
    if(ball->goingLeft()) {
        if(ball->getX() <= myBox->x1 + myBox->length && ball->getX() >= myBox->x1 &&
                ball->getY() >= myBox->y1 && ball->getY() <= myBox->y1 + myBox->length) {
            ball->setIsGoingLeft(false);
        }
    } else if(!ball->goingLeft()) {
        if(ball->getX() >= myBox->x1 && ball->getX() <= myBox->x1 + myBox->length &&
            ball->getY() >= myBox->y1 && ball->getY() <= myBox->y1 + myBox->length) {
            ball->setIsGoingLeft(true);
        }
    }
    if(ball->goingUp()) {
        if(ball->getY() <= myBox->y1 + myBox->length && ball->getY() >= myBox->y1 &&
            ball->getX() >= myBox->x1 && ball->getX() <= myBox->x1 + myBox->length) {
            ball->setIsGoingUp(false);
        }
    } else if (!ball->goingUp()) {
        if(ball->getY() >= myBox->y1 && ball->getY() <= myBox->y1 + myBox->length &&
            ball->getX() >= myBox->x1 && ball->getX() <= myBox->x1 + myBox->length)
            ball->setIsGoingUp(true);
    }
}

void *rectangle(void *args)
{
    Box *myBox = (Box*)args;
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