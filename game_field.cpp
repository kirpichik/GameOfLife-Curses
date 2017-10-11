//
//  game_field.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include "game_field.h"

GameField::GameField(size_t width, size_t height) {
    field = std::vector<std::vector<bool>>(width);
    for (size_t i = 0; i < width; i++)
        field[i] = std::vector<bool>(height);
}

GameField::SubGameField GameField::operator[](size_t pos) const {
    return SubGameField(field[pos]);
}

GameField::ModifiableGameField GameField::edit() {
    return ModifiableGameField(*this);
}

bool GameField::SubGameField::operator[](size_t pos) const {
    return subfield[pos];
}

GameField& GameField::ModifiableGameField::getOriginalField() const {
    return original;
}

void GameField::ModifiableGameField::apply() const {
    original.field = field;
}

GameField::ModifiableGameField::ModifiableSubGameField
    GameField::ModifiableGameField::operator[](size_t pos) {
    return ModifiableSubGameField(field[pos]);
}

GameField::ModifiableGameField::ModifiableSubGameField::ModifiableSubGameFieldProxy
    GameField::ModifiableGameField::ModifiableSubGameField::operator[](size_t pos) {
    return ModifiableSubGameFieldProxy(*this, pos);
}

bool GameField::ModifiableGameField::ModifiableSubGameField::operator[](size_t pos) const {
    return subfield[pos];
}

GameField::ModifiableGameField::ModifiableSubGameField::ModifiableSubGameFieldProxy&
    GameField::ModifiableGameField::ModifiableSubGameField::ModifiableSubGameFieldProxy::
        operator=(bool isCell) {
    forEdit.subfield[pos] = isCell;
    return const_cast<ModifiableSubGameFieldProxy&>(*this);
}

GameField::ModifiableGameField::ModifiableSubGameField::ModifiableSubGameFieldProxy::
    operator bool() const {
    return forEdit.subfield[pos];
}
