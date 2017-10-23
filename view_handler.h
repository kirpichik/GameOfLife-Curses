//
//  view_handler.h
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef view_handler_h
#define view_handler_h

#include "game_handler.h"

const std::vector<std::string> PROMPTS = {
    "Q Exit",
    "N Next turn",
    "B Step Back",
    "R Reset",
    "C Command mode"
};

class CursesViewHandler : public ViewHandler {
public:
    
    CursesViewHandler();
    
    /**
     * Отрисовывает поле и подсказки.
     *
     * @param field Игровое поле
     * */
    void updateField(const GameField& field, size_t stepsCount) override;
    
    /**
     * Отрисовывает курсор клавиатуры.
     *
     * @param posX Позиция курсора по X
     * @param posY Позиция курсора по Y
     * */
    void updateKeyboardCursor(size_t posX, size_t posY) override;
    
    /**
     * Обновляет командную строку и вывод в ней
     *
     * @param commandOutput Новая строка вывода командного режима
     * */
    void updateCommandLine(std::string commandOutput) override;
    
    /**
     * Считывает строку в командном режиме.
     *
     * @return Результат ввода пользователя
     * */
    std::string readCommandInput() override;
    
    /**
     * Ожидает нажатие клавиши/клик мышкой/истечение времени таймаута.
     * Если время ожидания истекает, возвращает специальное состояние объекта InputResult
     *
     * @param timeout Время ожидания ввода в тесятых долях секунды(0..255)
     *
     * @return Результат ожидания
     * */
    const InputResult waitForInput(uint8_t timeout) override;
    
    /**
     * Проверяет, можно ли создать поле с данными размерами на данном окне.
     *
     * @param width Ширина нового поля
     * @param height Высота нового поля
     *
     * @return true, если поле можно создать.
     * */
    bool canCrateFieldWithSizes(size_t width, size_t height) override;
    
    ~CursesViewHandler();

private:
    // Вывод командного режима
    std::string commandLine;
    
    // Позиция клавиатурного курсора
    size_t cursorX = 0;
    size_t cursorY = 0;
    
    // Размеры текущего отрисованного игрового поля
    size_t gameWidth = 0;
    size_t gameHeight = 0;
    
    /**
     * Отрисовывает вывод в командной строке
     * */
    void drawCommandLine() const;
    
};

#endif /* view_handler_h */
