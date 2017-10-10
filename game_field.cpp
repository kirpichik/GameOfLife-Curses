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

GameField::ModifiableGameField GameField::edit() const {
    return ModifiableGameField(const_cast<GameField&>(*this));
}

bool GameField::SubGameField::operator[](size_t pos) const {
    return subfield[pos];
}

void GameField::ModifiableGameField::apply() const {
    original.field = field;
}

GameField::ModifiableGameField::ModifiableSubGameField::ModifiableSubGameFieldProxy GameField::ModifiableGameField::ModifiableSubGameField::operator[](size_t pos) const {
    return ModifiableSubGameFieldProxy(const_cast<ModifiableSubGameField&>(*this), pos);
}

GameField::ModifiableGameField::ModifiableSubGameField::ModifiableSubGameFieldProxy& GameField::ModifiableGameField::ModifiableSubGameField::ModifiableSubGameFieldProxy::operator=(bool isCell) {
    forEdit.subfield[pos] = isCell;
    return const_cast<ModifiableSubGameFieldProxy&>(*this);
}

GameField::ModifiableGameField::ModifiableSubGameField::ModifiableSubGameFieldProxy::operator bool() const {
    return forEdit.subfield[pos];
}
