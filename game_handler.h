//
//  game_handler.h
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef game_handler_h
#define game_handler_h

#include "game_field.h"

/**
 * Обработчик события обновления поля.
 * */
class FieldUpdateListener {
public:
    
    /**
     * Вызывается при обновлении поля и принимает новый объект поля.
     * */
    virtual void onUpdate(const GameField& field) = 0;
};

/**
 * Управляющий всем состоянием текущей игры.
 * Хранит поле и выполянет ходы.
 * */
class GameManager {
public:
    
    /**
     * @param width Ширина поля
     * @param height Высота поля
     * @param listener Обработчик обновления поля.
     * */
    GameManager(size_t width, size_t height, FieldUpdateListener& listener) :
        width(width), height(height),
        gameField(GameField(width, height)), updateListener(listener) {}
    
    /**
     * Выполняет следующий ход.
     * После выполнения хода вызывается обработчик события обновления.
     * */
    void nextMove();
    
    /**
     * Выполняет несколько ходов.
     * После выполнения каждого хода вызывается обработчик события обновления.
     *
     * @param count Количество ходов.
     * */
    void manyMoves(size_t count);
    
    /**
     * Если клетка существовала на данной позиции, она умирает.
     * Если клетки не было, она полявляется.
     * После изменения вызывается обработчик события обновления.
     *
     * @param posX Позиция по X
     * @param posY Позиция по Y
     * */
    void setCellAt(int posX, int posY);

private:
    const size_t width, height;
    GameField gameField;
    FieldUpdateListener& updateListener;
    
    /**
     * Подсчитывает количество живых клеток вокруг данной клетки.
     *
     * @param posX Позиция X
     * @param posY Позиция Y
     *
     * @return Количество живых клеток вокруг
     * */
    size_t countLifeAround(int posX, int posY) const;
};

#endif /* game_handler_h */
