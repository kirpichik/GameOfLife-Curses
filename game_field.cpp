//
//  game_field.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include "game_field.h"

/**
 * Делает позицию цикличной.
 * Cоседями верхних клеток являются нижние,
 * соседями правых клеток являются левые.
 *
 * @param pos Позиция
 * @param module Модуль цикличности(размер поля)
 *
 * @return Цикличная позиция
 * */
static int roundPos(int pos, size_t module) {
    if (pos < 0)
        return ((int) module) - (-pos) % module;
    return pos % module;
}

GameField::GameField(size_t width, size_t height) : width(width), height(height) {
    field = std::vector<std::vector<bool>>(width);
    for (size_t i = 0; i < width; i++)
        field[i] = std::vector<bool>(height);
}

GameField::SubGameField GameField::operator[](int pos) {
    return SubGameField(roundPos(pos, width), (*this));
}

const GameField::SubGameField GameField::operator[](int pos) const {
    return SubGameField(roundPos(pos, width), const_cast<GameField&>(*this));
}

size_t GameField::getWidth() const {
    return width;
}

size_t GameField::getHeight() const {
    return height;
}

const GameField& GameField::operator=(const GameField &copy) {
    width = copy.width;
    height = copy.height;
    field = copy.field;
    return (*this);
}

bool GameField::operator==(const GameField& equal) const {
    return field == equal.field;
}

std::ostream& operator<<(std::ostream& stream, const GameField& field) {
    for (int i = 0; i < field.width; i++) {
        for (int j = 0; j < field.height; j++)
            stream << (field.field[i][j] ? '#' : '.');
        if (i != field.width - 1)
            stream << std::endl;
    }
    return stream;
}

GameField::SubGameField::Cell GameField::SubGameField::operator[](int pos) {
    return Cell(posX, roundPos(pos, height), field);
}

const GameField::SubGameField::Cell GameField::SubGameField::operator[](int pos) const {
    return Cell(posX, roundPos(pos, height), field);
}

bool GameField::SubGameField::Cell::isLife() const {
    return field[posX][posY];
}

size_t GameField::SubGameField::Cell::getX() const {
    return posX;
}

size_t GameField::SubGameField::Cell::getY() const {
    return posY;
}

void GameField::SubGameField::Cell::bornLife() {
    field[posX][posY] = true;
}

void GameField::SubGameField::Cell::kill() {
    field[posX][posY] = false;
}
