#include <ncurses.h>
#include "Sources/Ball.h"
#include <thread>
#include <unistd.h>
#include <vector>

int main(int argc, char **argv) {
    initscr();
    noecho();
    cbreak();
    Ball * balls[50];
    std::vector<std::thread> threads;
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW * playwin = newwin(20, 50, (yMax/2)-10, 10);
    box(playwin, 0, 0);
    refresh();
    wrefresh(playwin);

    for(int i = 0; i < 50; i++) {
        balls[i] =  new Ball(playwin, 10, 10,  'l', 5);
        std::thread t(&balls[i], balls[i]->start());
    }


    getch();
    endwin();
    return 0;
}
