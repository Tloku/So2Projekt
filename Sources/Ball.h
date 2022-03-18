//
// Created by tloku on 12.03.2022.
//
#ifndef SO2PROJEKT_BALL_H
#define SO2PROJEKT_BALL_H
#include <ncurses.h>


class Ball {
private:
    int x, y, bounces, maxBounces, width, height;
    bool isGoingLeft, isGoingUp;
    char direction;
    char model;
    WINDOW * currentWindow;

public:
    Ball(WINDOW *currentWindow, int x, int y, char direction, int maxBounces) {
        this->currentWindow = currentWindow;
        this->x = x;
        this->y = y;
        this->direction = direction;
        getmaxyx(currentWindow, height, width);
        this->model = 'O';
        this->maxBounces = maxBounces;
    }

    void start();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    int getMaxBounces() const;

    void setMaxBounces(int maxBounces);

    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);

    char getModel() const;

    void setModel(char model);

    WINDOW *getCurrentWindow() const;

    void setCurrentWindow(WINDOW *currentWindow);

    char getDirection() const;

    void setDirection(char direction);

    bool goingLeft() const;

    void setIsGoingLeft(bool isGoingLeft);

    bool goingUp() const;

    void setIsGoingUp(bool isGoingUp);

    int getBounces() const;

    void setBounces(int bounces);

};
#endif //SO2PROJEKT_BALL_H
