//
// Created by tloku on 12.03.2022.
//

#include "Ball.h"
#include <unistd.h>

void Ball::start() {
    if(getDirection() == 'l') {
        setIsGoingLeft(true);
    } else if (getDirection() == 'p') {
        setIsGoingLeft(false);
    }
    setIsGoingUp(true);

    while (getBounces() != getMaxBounces()) {
        if (goingLeft()) {
            moveLeft();
        } else {
            moveRight();
        }

        if (goingUp()) {
            moveUp();
        } else {
            moveDown();
        }
        mvwaddch(getCurrentWindow(), getY(), getX(), getModel());
        wrefresh(getCurrentWindow());
        usleep(200000);
    }
}

void Ball::moveLeft() {
    mvwaddch(getCurrentWindow(), getY(), getX(), ' ');
    x--;
    if(x <= 1) {
        setIsGoingLeft(false);
    }
}

void Ball::moveUp() {
    mvwaddch(getCurrentWindow(), getY(), getX(), ' ');
    y--;
    if(y <= 1) {
        setIsGoingUp(false);
    }
}

void Ball::moveDown() {
    mvwaddch(getCurrentWindow(), getY(), getX(), ' ');
    y++;
    if(y >= getmaxy(getCurrentWindow()) - 1) {
        setIsGoingUp(true);
    }
}

void Ball::moveRight() {
    mvwaddch(getCurrentWindow(), getY(), getX(), ' ');
    x++;
    if(x >= getmaxx(getCurrentWindow()) - 1) {
        setIsGoingLeft(true);
    }
}

char Ball::getDirection() const {
    return direction;
}

void Ball::setDirection(char direction) {
    Ball::direction = direction;
}

int Ball::getX() const {
    return x;
}

void Ball::setX(int x) {
    Ball::x = x;
}

int Ball::getY() const {
    return y;
}

void Ball::setY(int y) {
    Ball::y = y;
}

char Ball::getModel() const {
    return model;
}

void Ball::setModel(char model) {
    Ball::model = model;
}

WINDOW *Ball::getCurrentWindow() const {
    return currentWindow;
}

void Ball::setCurrentWindow(WINDOW *currentWindow) {
    Ball::currentWindow = currentWindow;
}

int Ball::getMaxBounces() const {
    return maxBounces;
}

void Ball::setMaxBounces(int maxBounces) {
    Ball::maxBounces = maxBounces;
}

int Ball::getBounces() const {
    return bounces;
}

void Ball::setBounces(int bounces) {
    Ball::bounces = bounces;
}

bool Ball::goingLeft() const {
    return isGoingLeft;
}

void Ball::setIsGoingLeft(bool isGoingLeft) {
    Ball::isGoingLeft = isGoingLeft;
}

bool Ball::goingUp() const {
    return isGoingUp;
}

void Ball::setIsGoingUp(bool isGoingUp) {
    Ball::isGoingUp = isGoingUp;
}

