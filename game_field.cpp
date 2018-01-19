//
//  game_field.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include <sstream>

#include "game_field.h"

const char ALIVE_CELL = '#';
const char NO_CELL = '.';

/**
 * Makes the position looped in.
 * The neighbors of the upper cells are lower,
 * the neighbors of the right cells are left.
 *
 * @param pos Position.
 * @param module Loop module (field size).
 *
 * @return Looped in position.
 */
static int loopCoordinate(int pos, size_t module) {
  if (pos < 0)
    return (static_cast<int>(module)) - (-pos) % module;
  return pos % module;
}

BadGameFieldException::BadGameFieldException(size_t line,
                                             size_t pos,
                                             const std::string& reason) {
  std::ostringstream out;
  out << reason << " at line " << line << ", position " << pos;
  this->reason = out.str();
}

const char* BadGameFieldException::what() const noexcept {
  return reason.c_str();
}

GameField::GameField(size_t width, size_t height)
    : width(width), height(height) {
  field = std::vector<std::vector<bool>>(width);
  for (size_t i = 0; i < width; i++)
    field[i] = std::vector<bool>(height);
}

GameField::GameField(const std::string str) {
  field.push_back(std::vector<bool>());
  size_t line = 0;
  size_t maxWidth = 0;
  size_t currWidth = 0;
  for (size_t i = 0; i < str.size(); i++) {
    switch (str[i]) {
      case '\n':
        if (maxWidth != currWidth)
          throw BadGameFieldException(
              line, currWidth, "Invalid number of characters in the line");
        line++;
        currWidth = 0;
        field.push_back(std::vector<bool>());
        break;
      case ALIVE_CELL:
        if (line == 0)
          maxWidth++;
        currWidth++;
        field[line].push_back(true);
        break;
      case NO_CELL:
        if (line == 0)
          maxWidth++;
        currWidth++;
        field[line].push_back(false);
        break;
      case ' ':
      case '\r':
        continue;
      default:
        throw BadGameFieldException(
            line, currWidth, std::string("Unknown character '") + str[i] + "'");
    }
  }
  if (maxWidth != currWidth && currWidth != 0)
    throw BadGameFieldException(line, currWidth,
                                "Invalid number of characters in the line");
  if (maxWidth != 0)
    line++;
  if (currWidth == 0 && line != 0) {
    field.pop_back();
    line--;
  }

  width = line;
  height = maxWidth;
}

GameField::SubGameField GameField::operator[](int pos) {
  return SubGameField(loopCoordinate(pos, width), (*this));
}

const GameField::SubGameField GameField::operator[](int pos) const {
  return SubGameField(loopCoordinate(pos, width),
                      const_cast<GameField&>(*this));
}

size_t GameField::getWidth() const {
  return width;
}

size_t GameField::getHeight() const {
  return height;
}

GameField& GameField::operator=(const GameField& copy) {
  if (&copy != this) {
    width = copy.width;
    height = copy.height;
    field = copy.field;
  }
  return (*this);
}

bool GameField::operator==(const GameField& equal) const {
  return field == equal.field;
}

std::ostream& operator<<(std::ostream& stream, const GameField& field) {
  for (int i = 0; i < field.width; i++) {
    for (int j = 0; j < field.height; j++)
      stream << (field.field[i][j] ? ALIVE_CELL : NO_CELL);
    if (i != field.width - 1)
      stream << std::endl;
  }
  return stream;
}

GameField::SubGameField::Cell GameField::SubGameField::operator[](int pos) {
  return Cell(posX, loopCoordinate(pos, height), field);
}

const GameField::SubGameField::Cell GameField::SubGameField::operator[](
    int pos) const {
  return Cell(posX, loopCoordinate(pos, height), field);
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
