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

class MutableGameField;

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
     * Получение подобъекта оператором квадратных скобок.
     * Учитывается цикличность поля по модулю width.
     * */
    SubGameField operator[](int pos) const;

    /**
     * Создает объект для редактирования поля.
     *
     * @return Объект для редактирования
     * */
    MutableGameField edit();
    
    ~GameField();
    
private:
    const size_t width;
    const size_t height;
    std::vector<std::vector<bool>>* field;
    
    friend MutableGameField;
};

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
    Cell operator[](int pos) const;
    
    /**
     * Запрет присваивания.
     * */
    SubGameField& operator=(SubGameField const&) = delete;
    
protected:
    const size_t posX;
    const size_t height;
    std::vector<std::vector<bool>>& field;
    
    SubGameField(size_t posX, size_t height, std::vector<std::vector<bool>>& field) :
        posX(posX), field(field), height(height) {}
    
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
    virtual bool isLife() const;
    
    /**
     * @return Позиция X
     * */
    virtual size_t getX() const;
    
    /**
     * @return Позиция Y
     * */
    virtual size_t getY() const;

protected:
    const size_t posX;
    const size_t posY;
    std::vector<std::vector<bool>>& field;
    
    Cell(size_t posX, size_t posY, std::vector<std::vector<bool>>& field) :
        posX(posX), posY(posY), field(field) {}
    
    friend SubGameField;
};

/**
 * Редактор игрового поля.
 * */
class MutableGameField {
public:
    
    class MutableSubGameField;
    
    /**
     * Применяет изменения из отредактированного объекта.
     */
    void apply() const;
    
    /**
     * Получение подобъекта оператором квадратных скобок.
     * Учитывается цикличность поля по модулю width.
     * */
    MutableSubGameField operator[](int pos);
    
private:
    GameField& original;
    std::vector<std::vector<bool>> field;
    
    MutableGameField(GameField& gameField) :
        original(gameField), field(std::vector<std::vector<bool>>((*gameField.field))) {}
    
    friend GameField;
};

/**
 * Получение значения ячейки и его редактирование с использованием оператора квадратных скобок.
 * */
class MutableGameField::MutableSubGameField : public GameField::SubGameField {
public:
    
    class MutableCell;
    
    /**
     * Получение значения ячейки на данной позиции.
     * */
    MutableCell operator[](int pos) const;
    
private:
    
    MutableSubGameField(size_t posX, size_t height, std::vector<std::vector<bool>>& field) :
        SubGameField(posX, height, field) {}
    
    friend MutableGameField;
};

/**
 * Редактируемая клетка поля.
 * */
class MutableGameField::MutableSubGameField::MutableCell :
    public GameField::SubGameField::Cell {
    
public:
    
    /**
     * Зарождает жизнь.
     * */
    void bornLife();

    /**
     * Уничтожает жизнь.
     * */
    void kill();
private:
        
    MutableCell(size_t posX, size_t posY, std::vector<std::vector<bool>>& field) :
        Cell(posX, posY, field) {}
        
    friend MutableSubGameField;
};

#endif /* game_field_h */
