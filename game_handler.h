//
//  game_handler.h
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef game_handler_h
#define game_handler_h

#include <map>
#include <string>
#include <ostream>

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
    GameManager(size_t width, size_t height, FieldUpdateListener& listener);
    
    /**
     * Выполняет следующий ход.
     * После выполнения хода вызывается обработчик события обновления.
     * */
    void nextStep();
    
    /**
     * Выполняет несколько ходов.
     * После выполнения каждого хода вызывается обработчик события обновления.
     *
     * @param count Количество ходов.
     * */
    void manySteps(size_t count);
    
    /**
     * Если клетка существовала на данной позиции, она умирает.
     * Если клетки не было, она полявляется.
     * После изменения вызывается обработчик события обновления.
     *
     * @param posX Позиция по X
     * @param posY Позиция по Y
     *
     * @return true, если в ячейке была создана жизнь
     * */
    bool setCellAt(int posX, int posY);
    
    /**
     * Очищает поле и сбрасыает счетчик команд.
     * После изменения вызывается обработчик события обновления.
     * */
    void reset();
    
    /**
     * Отменяет последний шаг.
     * После изменения вызывается обработчик события обновления.
     *
     * @return false, если шаг отменить невозможно.
     * */
    bool stepBack();
    
    /**
     * Принудительно вызывает обработчик события обновления без внесения изменений
     * в состояние поля.
     * */
    void update() const;
    
    /**
     * Регистрирует функцию обработчика команды.
     * @param name Команда
     * @param cmd Функция обработчик команды
     * */
    void registerCommand(std::string name, void(*cmd)(const std::vector<std::string>, GameManager&, std::ostream&));
    
    /**
     * Исполняет команду.
     * @param name Команда
     * @param args Аргументы
     *
     * @return true, если команда найдена и исполнена.
     * */
    bool executeCommand(std::string name, std::vector<std::string> args, std::ostream& output);

private:
    const size_t width;
    const size_t height;
    
    size_t stepsCounter;
    std::map<std::string, void(*)(const std::vector<std::string>, GameManager&, std::ostream&)> commands;
    GameField gameField;
    GameField previousStep;
    bool hasUndo = false; // Есть ли возможность отменить на данном шаге.
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
