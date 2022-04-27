//
// Created by tloku on 12.03.2022.
//

#include "Ball.h"

void Ball::moveLeft() {
    x--;
    if(x == 1) {
        setBounces(getBounces()+1);
        setIsGoingLeft(false);
    }
}

void Ball::moveUp() {
    y--;
    if(y == 1) {
        setBounces(getBounces()+1);
        setIsGoingUp(false);
    }
}

void Ball::moveDown() {
    y++;
//    if(y >= getmaxy(getCurrentWindow()) - 2) {
    if(y >= 30 - 1) {
        setBounces(getBounces()+1);
        setIsGoingUp(true);
    }
}

void Ball::moveRight() {
    x++;
//    if(x >= getmaxx(getCurrentWindow()) - 2) {
    if(x >= 50 - 1) {
        setBounces(getBounces()+1);
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

void Ball::setModel(char model) {
    Ball::model = model;
}

const char Ball::getModel() const {
    return model;
}

int Ball::getSpeed() const {
    return speed;
}

void Ball::setSpeed(int speed) {
    Ball::speed = speed;
}

int Ball::getBouncesInside() const {
    return bouncesInside;
}

void Ball::setBouncesInside(int bouncesInside) {
    Ball::bouncesInside = bouncesInside;
}

bool Ball::isBallInside() const {
    return isInside;
}

void Ball::setIsInside(bool isInside) {
    Ball::isInside = isInside;
}


