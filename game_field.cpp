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
    field = new std::vector<std::vector<bool>>(width);
    for (size_t i = 0; i < width; i++)
        (*field)[i] = std::vector<bool>(height);
}

// TODO - Может заменить как-нибудь без выделения на куче?
GameField::~GameField() {
    delete field;
}

GameField::SubGameField GameField::operator[](int pos) const {
    return SubGameField(roundPos(pos, width), height, (*field));
}

MutableGameField GameField::edit() {
    return MutableGameField(*this);
}

GameField::SubGameField::Cell GameField::SubGameField::operator[](int pos) const {
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

void MutableGameField::apply() const {
    (*original.field) = field;
}

MutableGameField::MutableSubGameField
    MutableGameField::operator[](int pos) {
    return MutableSubGameField(roundPos(pos, original.width), original.height, field);
}

MutableGameField::MutableSubGameField::MutableCell
    MutableGameField::MutableSubGameField::operator[](int pos) const {
    return MutableCell(posX, roundPos(pos, height), field);
}

void MutableGameField::MutableSubGameField::MutableCell::bornLife() {
    field[posX][posY] = true;
}

void MutableGameField::MutableSubGameField::MutableCell::kill() {
    field[posX][posY] = false;
}
