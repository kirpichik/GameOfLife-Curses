//
//  game_handler.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include "game_handler.h"

void GameManager::nextMove() {
    // TODO - выполнение хода
}

void GameManager::manyMoves(size_t count) {
    for (size_t i = 0; i < count; i++)
        nextMove();
}

void GameManager::setCellAt(size_t posX, size_t posY) {
    GameField::ModifiableGameField modifiable = gameField.edit();
    modifiable[posX][posY] = !modifiable[posX][posY];
    modifiable.apply();
    updateListener.onUpdate(gameField);
}
