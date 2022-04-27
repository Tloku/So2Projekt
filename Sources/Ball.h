//
// Created by tloku on 12.03.2022.
//
#ifndef SO2PROJEKT_BALL_H
#define SO2PROJEKT_BALL_H
#include <ncurses.h>


class Ball {
private:
    int x, y, bounces, maxBounces, width, height, speed, bouncesInside;
    bool isGoingLeft, isGoingUp, isInside;
    char direction;
    char model;
    WINDOW * currentWindow;

public:
    Ball(WINDOW *currentWindow, int x, int y, char direction, int maxBounces, char model, int speed) {
        this->currentWindow = currentWindow;
        this->x = x;
        this->y = y;
        this->direction = direction;
        getmaxyx(currentWindow, height, width);
        this->model = model;
        this->maxBounces = maxBounces;
        this->speed = speed;
        this->bounces = 0;
        this->bouncesInside = 0;
        this->isInside = false;
        if(this->getDirection() == 'l') {
            this->setIsGoingLeft(true);
        } else if (getDirection() == 'p') {
            this->setIsGoingLeft(false);
        }
        this->setIsGoingUp(true);
    }

    int getSpeed() const;
    void setSpeed(int speed);
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
    const char getModel() const;
    void setModel(char model);
    int getBouncesInside() const;
    void setBouncesInside(int bouncesInside);
    bool isBallInside() const;
    void setIsInside(bool isInside);
};
#endif //SO2PROJEKT_BALL_H
