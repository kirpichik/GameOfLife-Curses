//
//  game_handler.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include "game_handler.h"

// Количество живых клеток вокруг мертвой для зарождения жизни.
static const size_t BORN_LIFE = 3;

// Минимальное количество живых клеток вокруг живой для продолжения жизни.
static const size_t DEATH_LONELINESS = 2;

// Максимальное количесвто живых клеток вокруг живой для продолжения жизни.
static const size_t DEATH_OVERPOPULATION = 3;

size_t GameManager::countLifeAround(int posX, int posY) const {
    //   1 2 3
    // 1 # # #  }
    // 2 # . #  }- Точка это проверяемая ячейка
    // 3 # # #  }
    
    size_t lifes = 0; // Количество живых клеток вокруг
    
    lifes += gameField[posX - 1][posY].isLife() ? 1 : 0; // 2-1
    lifes += gameField[posX + 1][posY].isLife() ? 1 : 0; // 2-3
    
    // 1-1 -> 1-3
    for (int i = posX - 1; i <= posX + 1; i++)
        lifes += gameField[i][posY - 1].isLife() ? 1 : 0;
    
    // 3-1 -> 3-3
    for (int i = posX - 1; i <= posX + 1; i++)
        lifes += gameField[i][posY + 1].isLife() ? 1 : 0;
    
    return lifes;
}

void GameManager::nextMove() {
    MutableGameField modifiable = gameField.edit();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            size_t life = countLifeAround(i, j);
            bool hasLife = gameField[i][j].isLife();
            if (hasLife && (life < DEATH_LONELINESS || life > DEATH_OVERPOPULATION))
                modifiable[i][j].kill();
            else if (!hasLife && life == BORN_LIFE)
                modifiable[i][j].bornLife();
        }
    }
}

void GameManager::manyMoves(size_t count) {
    for (size_t i = 0; i < count; i++)
        nextMove();
}

void GameManager::setCellAt(int posX, int posY) {
    MutableGameField modifiable = gameField.edit();
    if (modifiable[posX][posY].isLife())
        modifiable[posX][posY].kill();
    else
        modifiable[posX][posY].bornLife();
    modifiable.apply();
    updateListener.onUpdate(gameField);
}
