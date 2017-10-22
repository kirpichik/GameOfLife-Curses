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

const std::vector<std::string> PROMPTS = {
    "Q Exit",
    "N Next turn",
    "B Step Back",
    "R Reset",
    "C Command mode"
};

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
     * @param field Поле для инициализации
     * @param listener Обработчик обновления поля
     * */
    GameManager(const GameField& field, FieldUpdateListener& listener);
    
    /**
     * Выполняет следующий ход.
     * После выполнения хода вызывается обработчик события обновления.
     * */
    void nextStep();
    
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
     * Очищает поле, сбрасывает счетчик команд и создает поле с новыми размерами.
     * После изменения вызывается обработчик события обновления.
     *
     * @param width Новая ширина
     * @param height Новая высота
     * */
    void reset(size_t width, size_t height);
    
    /**
     * Устанавливает новое поле и сбрасывает счетчик команд.
     * После установки вызывается обработчик события обновления.
     *
     * @param field Новое поле
     * */
    void reset(const GameField& field);
    
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
    void registerCommand(std::string name,
                         void(*cmd)(const std::vector<std::string>&, GameManager&, std::ostream&));
    
    /**
     * Исполняет команду.
     * @param name Команда
     * @param args Аргументы
     *
     * @return true, если команда найдена и исполнена.
     * */
    bool executeCommand(std::string name, std::vector<std::string>& args, std::ostream& output);
    
    /**
     * Проверяет, можно ли создать поле с данными размерами на данном терминале.
     *
     * @param fieldWidth Ширина нового поля
     * @param fieldHeight Высота нового поля
     *
     * @return true, если поле можно создать.
     * */
    bool canCreateFieldWithSizes(size_t fieldWidth, size_t fieldHeight) const;
    
    /**
     * Обработка нажатий мышью
     *
     * @param x Координата X
     * @param y Координата Y
     * */
    void onMousePressed(int x, int y);
    
    /**
     * Обработка нажатий клавиатуры
     *
     * @param key Код клавиши
     *
     * */
    void onKeyPressed(int key);
    
    /**
     * @return Игровое поле в данный момент.
     * */
    const GameField getCurrentField() const;
    
    /**
     * @return Количество шагов с начала игры.
     * */
    size_t getStepsCount() const;
    
    /**
     * @return Последний вывод из командного режима.
     * */
    std::string getLastCommandOutput() const;
    
    /**
     * Устанавливает последний вывод командного режима(для мнгновенного обновления).
     *
     * @param output Вывод
     * */
    void setLastCommandOutput(const std::string output);

private:
    size_t width;
    size_t height;
    
    size_t stepsCounter = 0;
    std::map<std::string, void(*)(const std::vector<std::string>&, GameManager&, std::ostream&)> commands;
    GameField gameField;
    GameField previousStep;
    bool hasUndo = false; // Есть ли возможность отменить на данном шаге.
    FieldUpdateListener& updateListener;
    std::string lastCommandOutput;
    
    /**
     * Подсчитывает количество живых клеток вокруг данной клетки.
     *
     * @param posX Позиция X
     * @param posY Позиция Y
     *
     * @return Количество живых клеток вокруг
     * */
    size_t countLifeAround(int posX, int posY) const;
    
    /**
     * Активирует командный режим и ждет ввода одной команды.
     * После исполнения команды переходит в обычный режим.
     * */
    void executionInCommandMode();
    
    /**
     * Вычисляет максимальную ширину подсказки на экране
     * в количестве занимаемых ею символов.
     *
     * @return Количество символов, которое занимает самая длинная подсказка.
     * */
    size_t getMaxPromptWidth() const;
};

#endif /* game_handler_h */
