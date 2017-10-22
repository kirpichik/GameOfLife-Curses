//
//  view_handler.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include <ncurses.h>

#include "view_handler.h"

// Код клавиши выхода
const int KEY_Q = 113;

ViewHandler::ViewHandler(size_t width, size_t height) :
game(GameManager(width, height, (*this))) {
    initscr();
    
#ifdef XCODE_EXTERNAL_DEBUG
    getchar();
#endif

    clear();
    noecho();
    cbreak();
    
    mousemask(BUTTON1_PRESSED, NULL);
    keypad(stdscr, TRUE);
    curs_set(0);
}

void ViewHandler::drawInterface() const {
    // Вывод подсказок по горячим клавишам
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
    
    // Вывод количества шагов
    move(i + 1, coloumn);
    printw("Step: %ld", game.getStepsCount());
    
    // Вывод командного режима
    move((int) game.getCurrentField().getHeight() + 1, 0);
    clrtobot();
    printw(game.getLastCommandOutput().c_str());
}

void ViewHandler::drawField(const GameField& field) const {
    for (int i = 0; i < field.getWidth(); i++)
        for (int j = 0; j < field.getHeight(); j++) {
            move(j, i * 2);
            addch(field[i][j].isLife() ? '#' : '.');
        }
}

void ViewHandler::onUpdate(const GameField& field) {
    clear();
    drawField(field);
    drawInterface();
    refresh();
}

int ViewHandler::runGame() {
    MEVENT event;
    int input;
    
    if (!game.canCreateFieldWithSizes(game.getCurrentField().getWidth(),
                                 game.getCurrentField().getHeight())) {
        endwin();
        printf("Cannot place %ldx%ld game field on this terminal size.\n",
               game.getCurrentField().getWidth(), game.getCurrentField().getHeight());
        printf("Please, resize terminal and restart the game.\n");
        return 1;
    }
    
    game.update();
    
    while(true) {
        input = getch();
        if (input == KEY_MOUSE) {
            if (getmouse(&event) == OK)
                game.onMousePressed(event.x, event.y);
        } else if (input == KEY_Q) // Завершить работу
            break;
        else
            game.onKeyPressed(input);
    }
    
    endwin();
    return 0;
}
