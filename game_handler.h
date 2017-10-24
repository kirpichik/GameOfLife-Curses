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
#include <cstdint>

#include "game_field.h"

/**
 * Результат ожидания ввода.
 * */
class InputResult {
public:
    
    /**
     * Событие брошено по таймауту.
     * */
    InputResult() : timedout(true) {}
    
    /**
     * Событие брошено по нажатию мыши.
     *
     * @param posX Позиция нажатия по X
     * @param posY Позиция нажатия по Y
     * */
    InputResult(size_t posX, size_t posY) : timedout(false), keyboard(false), posX(posX), posY(posY) {}
    
    /**
     * Событие брошено по нажатию клавиши.
     *
     * @param key Код клавиши.
     * */
    InputResult(int key) : timedout(false), keyboard(true), key(key) {}
    
    int getKey() const {
        return isKeyboard() ? key : 0;
    }
    
    size_t getPosX() const {
        return isKeyboard() ? 0 : posX;
    }
    
    size_t getPosY() const {
        return isKeyboard() ? 0 : posY;
    }
    
    bool isTimedOut() const {
        return timedout;
    }
    
    bool isKeyboard() const {
        return keyboard && !timedout;
    }
    
    const InputResult& operator=(const InputResult& copy) {
        posX = copy.posX;
        posY = copy.posY;
        keyboard = copy.keyboard;
        timedout = copy.timedout;
        key = copy.key;
        return (*this);
    }
    
private:
    // Позиция клика мышью
    size_t posX, posY;
    
    // Пришло ли событие с клавиатуры или с мыши
    bool keyboard;
    
    // Если это событие брошено по окончании времени ожидания
    bool timedout;
    
    // Код события
    int key;
};

/**
 * Обработчик события обновления поля.
 * */
class ViewHandler {
public:
    
    /**
     * Отрисовывает поле и подсказки.
     *
     * @param field Игровое поле
     * */
    virtual void updateField(const GameField& field, size_t stepsCount) = 0;
    
    /**
     * Отрисовывает курсор клавиатуры.
     *
     * @param posX Позиция курсора по X
     * @param posY Позиция курсора по Y
     * */
    virtual void updateKeyboardCursor(size_t posX, size_t posY) = 0;
    
    /**
     * Отрисовывает командную строку и вывод в ней
     *
     * @param commandOutput Новая строка вывода командного режима
     * */
    virtual void updateCommandLine(std::string commandOutput) = 0;
    
    /**
     * Считывает строку в командном режиме.
     *
     * @return Результат ввода пользователя
     * */
    virtual std::string readCommandInput() = 0;
    
    /**
     * Ожидает нажатие клавиши/клик мышкой/истечение времени таймаута.
     * Если время ожидания истекает, возвращает специальное состояние объекта InputResult
     *
     * @param timeout Время ожидания ввода в десятых долях секунды(0..255)
     *
     * @return Результат ожидания
     * */
    virtual const InputResult waitForInput(uint8_t timeout) = 0;
    
    /**
     * Проверяет, можно ли создать поле с данными размерами на данном окне.
     *
     * @param width Ширина нового поля
     * @param height Высота нового поля
     *
     * @return true, если поле можно создать.
     * */
    virtual bool canCrateFieldWithSizes(size_t width, size_t height) = 0;
    
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
     * @param viewHandler Обработчик обновления поля.
     * */
    GameManager(size_t width, size_t height, ViewHandler& viewHandler);
    
    /**
     * @param field Игровое поле
     * @param viewHandler Обработчик обновления поля.
     * */
    GameManager(const GameField& field, ViewHandler& viewHandler) :
        width(field.getWidth()), height(field.getHeight()),
        gameField(field), previousStep(GameField(0, 0)),
        viewHandler(viewHandler) {}
    
    /**
     * Запускает игру.
     * */
    int runGame();
    
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
     * Регистрирует функцию обработчика команды.
     * @param name Команда
     * @param cmd Функция обработчик команды
     * */
    void registerCommand(std::string name,
                         void(*cmd)(const std::vector<std::string>&, GameManager&, std::ostream&));
    
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
     * @return Игровое поле в данный момент.
     * */
    const GameField getCurrentField() const;
    
    /**
     * @return Текущая ширина игрового поля.
     * */
    size_t getWidth() const;
    
    /**
     * @return Текущая высота игрового поля.
     * */
    size_t getHeight() const;
    
    /**
     * @return Обработчик взаимодействия с пользователем
     * */
    ViewHandler& getViewHandler();

private:
    size_t width;
    size_t height;
    
    std::map<std::string, void(*)(const std::vector<std::string>&, GameManager&, std::ostream&)> commands;
    
    GameField gameField;
    GameField previousStep;
    
    ViewHandler& viewHandler;
    
    size_t stepsCounter = 0;
    bool hasUndo = false; // Есть ли возможность отменить на данном шаге.
    
    // Позиция клавиатурного курсора на поле
    size_t cursorX = 0;
    size_t cursorY = 0;
    
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
     * Принудительно вызывает обработчик события обновления без внесения изменений
     * в состояние поля.
     * */
    void update();
    
    /**
     * Активирует командный режим и ждет ввода одной команды.
     * После исполнения команды переходит в обычный режим.
     * */
    void executionInCommandMode();
    
    /**
     * Исполняет команду.
     * @param name Команда
     * @param args Аргументы
     *
     * @return true, если команда найдена и исполнена.
     * */
    bool executeCommand(std::string name, std::vector<std::string>& args, std::ostream& output);
    
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
     * Управление курсором для установки клеток на поле при помощи стрелок на клавиатуре.
     *
     * @param key Код клавиши
     * */
    void onKeyboardCursor(int key);
};

#endif /* game_handler_h */
