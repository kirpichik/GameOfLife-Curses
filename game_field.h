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

/**
 * Неизменяемое игровое поле.
 * */
class GameField {
public:
    
    class SubGameField;
    class ModifiableGameField;
    
    /**
     * @param width Ширина поля
     * @param height Высота поля
     * */
    GameField(size_t width, size_t height);
    
    /**
     * Получение подобъекта оператором квадратных скобок.
     * */
    SubGameField operator[](size_t pos) const;

    /**
     * Создает объект для редактирования поля.
     *
     * @return Объект для редактирования
     * */
    ModifiableGameField edit();
    
private:
    std::vector<std::vector<bool>> field;
};

/**
 * Получение значения ячейки оператором двойных квадратных скобок.
 * */
class GameField::SubGameField {
public:
    
    /**
     * Получение значения ячейки на данной позиции.
     * */
    bool operator[](size_t pos) const;
    
    /**
     * Запрет присваивания.
     * */
    SubGameField& operator=(SubGameField const&) = delete;
    
private:
    std::vector<bool> subfield;
    
    SubGameField(std::vector<bool> field) : subfield(field) {}
    
    friend GameField;
};

/**
 * Редактор игрового поля.
 * */
class GameField::ModifiableGameField {
public:
    
    class ModifiableSubGameField;
    
    /**
     * @return Возвращает текущее состояние поля.
     * */
    GameField& getOriginalField() const;
    
    /**
     * Применяет изменения из отредактированного объекта.
     */
    void apply() const;
    
    /**
     * Получение подобъекта оператором квадратных скобок.
     * */
    ModifiableSubGameField operator[](size_t pos);
    
private:
    GameField& original;
    std::vector<std::vector<bool>> field;
    
    ModifiableGameField(GameField& gameField) :
        original(gameField), field(std::vector<std::vector<bool>>(gameField.field)) {}
    
    friend GameField;
};

/**
 * Получение значения ячейки и его редактирование с использованием оператора квадратных скобок.
 * */
class GameField::ModifiableGameField::ModifiableSubGameField {
public:
    
    class ModifiableSubGameFieldProxy;
    
    /**
     * Получение прокси ячейки на данной позиции.
     * */
    ModifiableSubGameFieldProxy operator[](size_t pos);
    
    /**
     * Получение значения ячейки на данной позиции.
     * */
    bool operator[](size_t pos) const;
    
    /**
     * Запрет присваивания лишних объектов.
     * */
    ModifiableSubGameField& operator=(ModifiableSubGameField const&) = delete;
    
private:
    std::vector<bool>& subfield;
    
    ModifiableSubGameField(std::vector<bool>& field) : subfield(field) {}
    
    friend ModifiableGameField;
};

/**
 * Прокси для получения/изменения ячейки
 * */
class GameField::ModifiableGameField::ModifiableSubGameField::ModifiableSubGameFieldProxy {
public:

    ModifiableSubGameFieldProxy& operator=(bool isCell);
    
    operator bool() const;
    
private:
    const size_t pos;
    ModifiableSubGameField& forEdit;
    
    ModifiableSubGameFieldProxy(ModifiableSubGameField& edit, size_t position) :
        pos(position), forEdit(edit) {}
    
    friend ModifiableSubGameField;
};

#endif /* game_field_h */
