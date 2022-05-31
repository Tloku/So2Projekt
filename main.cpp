#include <ncurses.h>
#include <iostream>
#include "Sources/Ball.h"
#include <unistd.h>
#include <cstdlib>
#include <thread>
#include <list>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <vector>

struct Box;
void start(Ball *ball, Box* myBox);
void startBox(Box* myBox);
void drawBoard(WINDOW* playWin, Box* myBox);
void ballCollisionWithRectangle(Ball *ball, Box *myBox);
void detectKeyPressAndCloseApp(WINDOW* win);
bool isInSquare(Ball *ball, Box *myBox);
void countBallsInsideBox(Box *myBox);

struct Box {
    int y1, x1, length;
    bool goingDown;
    bool box_moving;
    WINDOW* window;
};

// kulki wpadają do prostokąta i w nim się odbijają 3 razy, po 3 odbicu wychodzą z prostokąta,
// gdy kulka jest w prostokącie to prostokąt się nie porusza

// wyświetlać ile kulek jest złapanych w danym momencie w prostokącie

std::condition_variable cv;
std::list<Ball> balls;
bool quit = false;
std::mutex isInside;
int ballsCounter;
std::mutex ballCounterMutex;

int main(int argc, char **argv) {
    srand(time(nullptr));
    initscr();
    noecho();
    cbreak();
    curs_set(false);

    WINDOW * playWin = newwin(30, 50, 3, 5);
    char models[] = {'O', 'C', 'D', 'E', 'J'};
    box(playWin, 0, 0);
    Box myBox = {6, 5, 10, true, true,playWin};

    std::thread myBoxThread(startBox, &myBox);
    std::thread drawBoardThread(drawBoard, playWin, &myBox);
    std::thread keyboardInputThread(detectKeyPressAndCloseApp, playWin);
    std::thread ballCounterThread(countBallsInsideBox, &myBox);
    int counter = 0;
    std::list<std::thread> ballThreads;
    while(!quit) {
        if(counter == INT32_MAX) {
            counter = 0;
        }

        balls.emplace_back(playWin, 25, 30, 'l', 5, models[counter++%4], rand() % 8 + 4);
        ballThreads.emplace_back(start, &(balls.back()), &myBox);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        balls.emplace_back(playWin, 25, 30, 'p', 5, models[counter%4+1], rand() % 8 + 4);
        ballThreads.emplace_back(start, &(balls.back()), &myBox);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    for(auto & i : ballThreads) {
        i.join();
    }
    ballCounterThread.join();
    keyboardInputThread.join();
    drawBoardThread.join();
    myBoxThread.join();

    endwin();
    return 0;
}

void countBallsInsideBox(Box *myBox) {
    int counter;

    while(!quit) {
        counter = 0;
        for(Ball ball : balls){
            if(isInSquare(&ball, myBox)) {
                counter++;
            }
        }
        ballsCounter = counter;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void drawBoard(WINDOW* playWin, Box* myBox) {
    while(true) {

        if(quit) {
            break;
        }

        for(Ball ball : balls) {
            if(ball.getBounces() >= ball.getMaxBounces()){
                continue;
            }
            mvwaddch(playWin, ball.getY(), ball.getX(), ball.getModel());
        }
        for(int j = 0; j < myBox->length; j++) {
            mvwaddch(playWin, myBox->y1, myBox->x1 + j, '#'); //pozioma górna ściana
            mvwaddch(playWin, myBox->y1 + j, myBox->x1,  '#'); //lewa pionowa ściana
            mvwaddch(playWin, myBox->y1 + myBox->length, myBox->x1 + j, '#'); //pozioma dolna ściana
            mvwaddch(playWin, myBox->y1 + j, myBox->x1 + myBox->length - 1, '#'); // prawa pionowa ściana
        }
        std::string text = "Zlapane kulki: " + std::to_string(ballsCounter);
        mvwaddstr(playWin, myBox->y1 + myBox->length, myBox->x1 + myBox->length + 1, text.c_str());

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

        isInside.lock();
            if(isInSquare(ball, myBox)) {
                myBox->box_moving = false;
            } else if(ballsCounter == 0){
                cv.notify_one();
                myBox->box_moving = true;
            }
        isInside.unlock();

        ballCollisionWithRectangle(ball, myBox);
        std::this_thread::sleep_for(std::chrono::microseconds((int)600000 / ball->getSpeed()));
    }
}

void ballCollisionWithRectangle(Ball *ball, Box *myBox) {
    if(ball->goingLeft()) {
        if(ball->getX() <= myBox->x1 + 1 && ball->getX() <= myBox->x1 + myBox->length &&
           ball->getY() >= myBox->y1 && ball->getY() <= myBox->y1 + myBox->length && ball->getBouncesInside() < 3) {
            ball->setIsGoingLeft(false);
            ball->setBouncesInside(ball->getBouncesInside() + 1);
        }
    } else if(!ball->goingLeft()) {
        if(ball->getX() >= myBox->x1 +myBox->length - 1 && ball->getX() < myBox->x1 +myBox->length &&
           ball->getY() >= myBox->y1 && ball->getY() <= myBox->y1 + myBox->length  && ball->getBouncesInside() < 3) {
            ball->setIsGoingLeft(true);
            ball->setBouncesInside(ball->getBouncesInside() + 1);
        }
    }
    if(ball->goingUp()) {
        if(ball->getY() <= myBox->y1 && ball->getY() <= myBox->y1 + myBox->length &&
           ball->getX() >= myBox->x1 && ball->getX() <= myBox->x1 + myBox->length  && ball->getBouncesInside() < 3) {
            ball->setIsGoingUp(false);
            ball->setBouncesInside(ball->getBouncesInside() + 1);
        }

    } else if (!ball->goingUp()) {
        if(ball->getY() >= myBox->y1 + myBox->length -1 && ball->getY() >= myBox->y1  &&
           ball->getX() >= myBox->x1 && ball->getX() <= myBox->x1 + myBox->length  && ball->getBouncesInside() < 3) {
            ball->setIsGoingUp(true);
            ball->setBouncesInside(ball->getBouncesInside() + 1);
        }
    }

}

bool isInSquare(Ball* ball, Box *myBox) {
    if(ball->getY() >= myBox->y1 && ball->getY() < myBox->y1 + myBox->length &&
        ball->getX() >= myBox->x1 && ball->getX() < myBox->x1 + myBox->length &&
        ball->getBouncesInside() < 3) {
        return true;
    }
    return false;
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
                speedOfBox = rand() % 7 + 5;
            }
        } else if (!myBox->goingDown) {
            myBox->y1--;
            if(myBox->y1 <= 2) {
                myBox->goingDown = true;
                speedOfBox = rand() % 7 + 5;
            }
        }

        {
            std::unique_lock<std::mutex> ul(isInside);

            while(!myBox->box_moving) {
                cv.wait(ul);
            }
        }
        std::this_thread::sleep_for(std::chrono::microseconds((int)1200000 / speedOfBox));
    }
}
