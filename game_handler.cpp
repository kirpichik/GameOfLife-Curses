//
//  game_handler.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "game_handler.h"

static const int KEY_UP = 259;
static const int KEY_DOWN = 258;
static const int KEY_LEFT = 260;
static const int KEY_RIGHT = 261;
static const int KEY_ENTER = 10;

GameManager::GameManager(size_t width, size_t height, ViewHandler& viewHandler)
    : width(width),
      height(height),
      gameField(GameField(width, height)),
      viewHandler(viewHandler),
      previousStep(GameField(0, 0)) {
  registerCommand("reset", &commandReset);
  registerCommand("set", &commandSet);
  registerCommand("step", &commandStep);
  registerCommand("back", &commandBack);
  registerCommand("save", &commandSave);
  registerCommand("load", &commandLoad);
}

int GameManager::runGame() {
  if (!canCreateFieldWithSizes(width, height)) {
    std::cerr << "Cannot create game field with " << width << "x" << height
              << " size." << std::endl;
    std::cerr << "Please resize your terminal window and restart game."
              << std::endl;
    return -1;
  }

  update();
  viewHandler.updateKeyboardCursor(cursorX, cursorY);

  while (true) {
    InputResult result = viewHandler.waitForInput(0);
    if (result.isKeyboard()) {
      if (result.getKey() == KEY_Q)
        break;
      else
        onKeyPressed(result.getKey());
    } else
      onMousePressed(static_cast<int>(result.getPosX()),
                     static_cast<int>(result.getPosY()));
  }

  return 0;
}

void GameManager::nextStep() {
  previousStep = gameField;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      size_t life = countLifeAround(i, j);
      bool hasLife = previousStep[i][j].isLife();
      if (hasLife && (life < DEATH_LONELINESS || life > DEATH_OVERPOPULATION))
        gameField[i][j].kill();
      else if (!hasLife && life == BORN_LIFE)
        gameField[i][j].bornLife();
    }
  }
  stepsCounter++;
  hasUndo = true;
  update();
}

bool GameManager::setCellAt(int posX, int posY) {
  previousStep = gameField;
  if (gameField[posX][posY].isLife())
    gameField[posX][posY].kill();
  else
    gameField[posX][posY].bornLife();
  hasUndo = true;
  update();
  return gameField[posX][posY].isLife();
}

void GameManager::reset(size_t width, size_t height) {
  this->width = width;
  this->height = height;
  gameField = GameField(width, height);
  hasUndo = false;
  stepsCounter = 0;
  cursorY = cursorX = 0;
  viewHandler.updateKeyboardCursor(cursorX, cursorY);
  update();
}

void GameManager::reset(const GameField& field) {
  width = field.getWidth();
  height = field.getHeight();
  gameField = GameField(field);
  hasUndo = false;
  stepsCounter = 0;
  cursorY = cursorX = 0;
  viewHandler.updateKeyboardCursor(cursorX, cursorY);
  update();
}

bool GameManager::stepBack() {
  if (!hasUndo)
    return false;

  gameField = previousStep;
  hasUndo = false;
  if (stepsCounter)
    stepsCounter--;
  update();

  return true;
}

size_t GameManager::countLifeAround(int posX, int posY) const {
  //   1 2 3
  // 1 # # #  }
  // 2 # . #  }- Point is a checking cell
  // 3 # # #  }

  size_t lifes = 0;  // Number of living cells around

  lifes += previousStep[posX - 1][posY].isLife() ? 1 : 0;  // 2-1
  lifes += previousStep[posX + 1][posY].isLife() ? 1 : 0;  // 2-3

  // 1-1 -> 1-3
  for (int i = posX - 1; i <= posX + 1; i++)
    lifes += previousStep[i][posY - 1].isLife() ? 1 : 0;

  // 3-1 -> 3-3
  for (int i = posX - 1; i <= posX + 1; i++)
    lifes += previousStep[i][posY + 1].isLife() ? 1 : 0;

  return lifes;
}

void GameManager::update() {
  viewHandler.updateField(gameField, stepsCounter);
}

void GameManager::registerCommand(const std::string& name,
                                  void (*cmd)(const std::vector<std::string>&,
                                              GameManager&,
                                              std::ostream&)) {
  commands[name] = cmd;
}

bool GameManager::executeCommand(const std::string& name,
                                 const std::vector<std::string>& args,
                                 std::ostream& output) {
  auto executor = commands.find(name);
  if (executor == commands.end())
    return false;
  executor->second(args, (*this), output);
  return true;
}

static std::vector<std::string> splitString(const std::string& str) {
  std::istringstream input(str);
  std::string item;
  std::vector<std::string> items;
  while (std::getline(input, item, ' '))
    items.push_back(item);
  return items;
}

void GameManager::executionInCommandMode() {
  const std::string commandInput(viewHandler.readCommandInput());
  std::vector<std::string> split(splitString(commandInput));

  if (split.empty()) {
    viewHandler.updateCommandLine("");
    return;
  }

  std::string& command = split[0];
  std::vector<std::string> args(split.begin() + 1,
                                split.begin() + split.size());
  std::ostringstream out;

  viewHandler.updateCommandLine("");

  if (!executeCommand(command, args, out))
    out << "Command \"" << command << "\" not found.";

  viewHandler.updateCommandLine(out.str());
}

void GameManager::onKeyboardCursor(int key) {
  switch (key) {
    case KEY_UP:
      if (cursorY != 0)
        cursorY--;
      break;
    case KEY_DOWN:
      if (cursorY + 1 != height)
        cursorY++;
      break;
    case KEY_LEFT:
      if (cursorX != 0)
        cursorX--;
      break;
    case KEY_RIGHT:
      if (cursorX + 1 != width)
        cursorX++;
      break;
  }
  viewHandler.updateKeyboardCursor(cursorX, cursorY);
}

void GameManager::onMousePressed(int x, int y) {
  cursorX = x;
  cursorY = y;
  viewHandler.updateKeyboardCursor(cursorX, cursorY);
  setCellAt(x, y);
}

void GameManager::onKeyPressed(int key) {
  switch (key) {
    case KEY_N:
      nextStep();
      break;
    case KEY_B:
      viewHandler.updateCommandLine(stepBack()
                                        ? "Back step completed."
                                        : "It's impossible to step back.");
      break;
    case KEY_R:
      viewHandler.updateCommandLine("Field reseted.");
      reset(width, height);
      break;
    case KEY_C:
      executionInCommandMode();
      break;
    case KEY_ENTER:
      setCellAt(static_cast<int>(cursorX), static_cast<int>(cursorY));
      viewHandler.updateKeyboardCursor(cursorX, cursorY);
      break;
    case KEY_LEFT:
    case KEY_RIGHT:
    case KEY_UP:
    case KEY_DOWN:
      onKeyboardCursor(key);
      break;
    default:
      break;
  }
}

bool GameManager::canCreateFieldWithSizes(size_t width, size_t height) const {
  return viewHandler.canCrateFieldWithSizes(width, height);
}

const GameField GameManager::getCurrentField() const {
  return gameField;
}

size_t GameManager::getWidth() const {
  return width;
}

size_t GameManager::getHeight() const {
  return height;
}

ViewHandler& GameManager::getViewHandler() {
  return viewHandler;
}
