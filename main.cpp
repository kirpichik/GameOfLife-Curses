//
//  main.cpp
//  GameOfLive
//
//  Created by Кирилл on 15.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include "view_handler.h"
#include "game_handler.h"

// Размеры стандартного поля
const size_t FIELD_WIDTH = 10;
const size_t FIELD_HEIGHT = 10;

int main(int argc, const char * argv[]) {
    CursesViewHandler view;
    GameManager control(FIELD_WIDTH, FIELD_HEIGHT, view);
    return control.runGame();
}
