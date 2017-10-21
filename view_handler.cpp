//
//  view_handler.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include <ncurses.h>

#include "view_handler.h"

ViewHandler::ViewHandler(size_t width, size_t height) :
game(GameManager(width, height, (*this))) {
    initscr();
    
    getchar(); // FIXME - FOR XCODE EXTERNAL DEBUG
    
    clear();
    noecho();
    cbreak();
    
    mousemask(BUTTON1_PRESSED, NULL);
    keypad(stdscr, TRUE);
    curs_set(0);
}

void ViewHandler::drawPrompts() const {
    int i = 0;
    const int coloumn = (int) game.getCurrentField().getWidth() * 2;
    for (std::string prompt : PROMPTS) {
        move(i++, coloumn);
        attron(A_REVERSE);
        printw(" ");
        printw("%c", prompt[0]);
        printw(" ");
        attroff(A_REVERSE);
        printw(prompt.substr(1).c_str());
    }
    move(i + 1, coloumn);
    printw("Step: %ld", game.getStepsCount());
}

void ViewHandler::drawFieldAt(const GameField& field) const {
    for (int i = 0; i < field.getWidth(); i++)
        for (int j = 0; j < field.getHeight(); j++) {
            move(j, i * 2);
            addch(field[i][j].isLife() ? '#' : '.');
        }
}

void ViewHandler::onUpdate(const GameField& field) {
    clear();
    drawFieldAt(field);
    drawPrompts();
    refresh();
}

int ViewHandler::runGame() {
    MEVENT event;
    int input;
    
    if (!game.canCreateFieldWithSizes(game.getCurrentField().getWidth(),
                                 game.getCurrentField().getHeight())) {
        endwin();
        printf("Cannot place 10x10 game field on this terminal size.\n");
        printf("Please, resize terminal and restart game.\n");
        return 0;
    }
    
    game.update();
    
    while(true) {
        input = getch();
        if (input == KEY_MOUSE) {
            if (getmouse(&event) == OK)
                game.onMousePressed(event.x, event.y);
        } else {
            if (game.onKeyPressed(input))
                break;
        }
    }
    
    endwin();
    return 0;
}
