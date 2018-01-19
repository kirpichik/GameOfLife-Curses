//
//  game_field.h
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <exception>
#include <ostream>
#include <vector>

class BadGameFieldException : public std::exception {
 public:
  BadGameFieldException(size_t line, size_t pos, const std::string& reason);

  const char* what() const noexcept override;

 private:
  std::string reason;
};

class GameField {
 public:
  class SubGameField;

  GameField(size_t width, size_t height);

  GameField(const GameField& toCopy)
      : width(toCopy.width), height(toCopy.height), field(toCopy.field) {}

  /**
   * Parse string and creates field from it.
   * Living Cell: '#'
   * Dead Cell: '.'
   *
   * @param str String for parse.
   */
  GameField(const std::string str);

  SubGameField operator[](int pos);

  const SubGameField operator[](int pos) const;

  size_t getWidth() const;

  size_t getHeight() const;

  GameField& operator=(const GameField& copy);

  bool operator==(const GameField& equal) const;

 private:
  size_t width;
  size_t height;
  std::vector<std::vector<bool>> field;

  friend SubGameField;
  friend std::ostream& operator<<(std::ostream& stream, const GameField& field);
};

/**
 * Outputs field to stream.
 * Living Cell: '#'
 * Dead Cell: '.'
 */
std::ostream& operator<<(std::ostream& stream, const GameField& field);

class GameField::SubGameField {
 public:
  class Cell;

  /**
   * @return Cell at position, considering loop.
   */
  Cell operator[](int pos);

  /**
   * @return Cell at position, considering loop.
   */
  const Cell operator[](int pos) const;

 private:
  const size_t posX;
  const size_t height;
  std::vector<std::vector<bool>>& field;

  SubGameField(size_t posX, GameField& game)
      : posX(posX), field(game.field), height(game.height) {}

  SubGameField& operator=(SubGameField const&) = delete;

  friend GameField;
};

class GameField::SubGameField::Cell {
 public:
  bool isLife() const;

  size_t getX() const;

  size_t getY() const;

  void bornLife();

  void kill();

 private:
  const size_t posX;
  const size_t posY;
  std::vector<std::vector<bool>>& field;

  Cell(size_t posX, size_t posY, std::vector<std::vector<bool>>& field)
      : posX(posX), posY(posY), field(field) {}

  friend SubGameField;
};

#endif /* GAME_FIELD_H */
