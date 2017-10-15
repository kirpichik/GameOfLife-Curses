//
//  game_field.h
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef game_field_h
#define game_field_h

#include <vector>
#include <ostream>

/**
 * Неизменяемое игровое поле.
 * */
class GameField {
public:
    
    class SubGameField;
    
    /**
     * @param width Ширина поля
     * @param height Высота поля
     * */
    GameField(size_t width, size_t height);
    
    /**
     * Конструктор копирования
     * */
    GameField(const GameField& toCopy) : width(toCopy.width), height(toCopy.height), field(toCopy.field) {}
    
    /**
     * Получение подобъекта оператором квадратных скобок.
     * Учитывается цикличность поля по модулю width.
     * */
    SubGameField operator[](int pos);
    
    /**
     * Получение подобъекта оператором квадратных скобок.
     * Учитывается цикличность поля по модулю width.
     * */
    const SubGameField operator[](int pos) const;
    
    /**
     * @return Ширина поля.
     * */
    size_t getWidth() const;
    
    /**
     * @return Высота поля.
     * */
    size_t getHeight() const;
    
    const GameField& operator=(const GameField& copy);
    
    bool operator==(const GameField& equal) const;
    
private:
    size_t width;
    size_t height;
    std::vector<std::vector<bool>> field;
    
    friend SubGameField;
    friend std::ostream& operator<<(std::ostream& stream, const GameField& field);
};

/**
 * TODO - Почему его нельзя перегрузить как член класса??????
 * */
std::ostream& operator<<(std::ostream& stream, const GameField& field);

/**
 * Получение значения ячейки оператором двойных квадратных скобок.
 * */
class GameField::SubGameField {
public:
    
    class Cell;
    
    /**
     * Получение значения ячейки на данной позиции.
     * Учитывается цикличность поля по модулю height.
     * */
    Cell operator[](int pos);
    
    /**
     * Получение значения ячейки на данной позиции.
     * Учитывается цикличность поля по модулю height.
     * */
    const Cell operator[](int pos) const;
    
    /**
     * Запрет присваивания.
     * */
    SubGameField& operator=(SubGameField const&) = delete;
    
private:
    const size_t posX;
    const size_t height;
    std::vector<std::vector<bool>>& field;
    
    SubGameField(size_t posX, GameField& game) :
        posX(posX), field(game.field), height(game.height) {}
    
    friend GameField;
};

/**
 * Клетка на поле.
 * */
class GameField::SubGameField::Cell {
public:
    
    /**
     * @return true, если клетка жива.
     * */
    bool isLife() const;
    
    /**
     * @return Позиция X
     * */
    size_t getX() const;
    
    /**
     * @return Позиция Y
     * */
    size_t getY() const;
    
    /**
     * Зарождает жизнь.
     * */
    void bornLife();
    
    /**
     * Уничтожает жизнь.
     * */
    void kill();

private:
    const size_t posX;
    const size_t posY;
    std::vector<std::vector<bool>>& field;
    
    Cell(size_t posX, size_t posY, std::vector<std::vector<bool>>& field) :
        posX(posX), posY(posY), field(field) {}
    
    friend SubGameField;
};

#endif /* game_field_h */
