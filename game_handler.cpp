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

static const std::string DEFAULT_SAVE_FILENAME = "game_of_life.fld";

// Key codes
static const int KEY_N = 110;
static const int KEY_B = 98;
static const int KEY_R = 114;
static const int KEY_C = 99;
static const int KEY_I = 105;
static const int KEY_Q = 113;

static const int KEY_UP = 259;
static const int KEY_DOWN = 258;
static const int KEY_LEFT = 260;
static const int KEY_RIGHT = 261;
static const int KEY_ENTER = 10;

// Delay in tenths of a second when the next step is updated (for multi steps).
static const size_t STEP_UPDATE_DELAY = 1;

// Number of living cells around the dead for the birth of life.
static const size_t BORN_LIFE = 3;

// Minimum number of living cells around the living to continue life.
static const size_t DEATH_LONELINESS = 2;

// Maximum number of living cells around the living to continue life.
static const size_t DEATH_OVERPOPULATION = 3;

// ==================== Command handlers ====================

/**
 * Clears the field and resets the steps counter.
 * If you pass width and height arguments, it creates a field with given width
 * and height. Arguments: [width] [height]
 */
static void commandReset(const std::vector<std::string>& args,
                         GameManager& game,
                         std::ostream& out) {
  if (args.size() != 2) {
    if (game.canCreateFieldWithSizes(game.getWidth(), game.getHeight()))
      game.reset(game.getWidth(), game.getHeight());
    else {
      out << "Cannot place game field on this terminal size." << std::endl;
      return;
    }
  } else {
    int width = atoi(args[0].c_str());
    int height = atoi(args[1].c_str());
    if (game.canCreateFieldWithSizes(width, height))
      game.reset(width, height);
    else {
      out << "Cannot place game field on this terminal size." << std::endl;
      return;
    }
  }
  out << "Field reseted to size " << game.getWidth() << "x" << game.getHeight()
      << "" << std::endl;
}

/**
 * Sets or removes life in a cell.
 * Arguments: <pos X> <pos Y>
 */
static void commandSet(const std::vector<std::string>& args,
                       GameManager& game,
                       std::ostream& out) {
  if (args.size() != 2) {
    out << "Need args: <pos X> <pos Y>" << std::endl;
    return;
  }
  int posX = atoi(args[0].c_str());
  int posY = atoi(args[1].c_str());

  out << "Cell " << (game.setCellAt(posX, posY) ? "spawned." : "killed.")
      << std::endl;
}

/**
 * Performs the specified number of steps. If there is no argument, it performs
 * 1 step. If the argument is '-', performs an infinite number of steps, until
 * the key 'I' is pressed. Arguments: [steps count or '-']
 */
static void commandStep(const std::vector<std::string>& args,
                        GameManager& game,
                        std::ostream& out) {
  size_t steps = 1;
  bool isInfinity = false;
  if (args.size() > 0) {
    if (args[0] == "-")
      isInfinity = true;
    else
      steps = atoi(args[0].c_str());
  }

  game.getViewHandler().updateCommandLine(
      "Making steps... Press I for interrupt.");

  size_t counter = 0;
  for (; counter < steps || isInfinity; counter++) {
    game.nextStep();
    InputResult result = game.getViewHandler().waitForInput(STEP_UPDATE_DELAY);
    if (result.isKeyboard() && result.getKey() == KEY_I)
      break;
  }

  out << "Done " << counter << " step(s)." << std::endl;
}

/**
 * Cancels the last step.
 * You can not undo more than one step.
 */
static void commandBack(const std::vector<std::string>& args,
                        GameManager& game,
                        std::ostream& out) {
  if (game.stepBack())
    out << "Back step completed." << std::endl;
  else
    out << "It's impossible to step back." << std::endl;
}

/**
 * Saves field to file.
 * Arguments: [filename]
 */
static void commandSave(const std::vector<std::string>& args,
                        GameManager& game,
                        std::ostream& out) {
  std::string filename = DEFAULT_SAVE_FILENAME;
  if (args.size() > 0)
    filename = args[0];

  std::ofstream file(filename);
  if (!file.is_open()) {
    out << "Cannot create file \"" << filename << "\"." << std::endl;
    return;
  }

  file << game.getCurrentField() << std::endl;
  file.close();

  out << "Game field saved to \"" << filename << "\"." << std::endl;
}

/**
 * Loads field from file.
 * Arguments: [filename]
 */
static void commandLoad(const std::vector<std::string>& args,
                        GameManager& game,
                        std::ostream& out) {
  std::string filename = DEFAULT_SAVE_FILENAME;
  if (args.size() > 0)
    filename = args[0];

  std::ifstream file(filename);
  if (!file.is_open()) {
    out << "Cannot load file \"" << filename << "\"" << std::endl;
    return;
  }

  std::ostringstream fileContent;
  std::string line;
  while (std::getline(file, line))
    fileContent << line << std::endl;
  file.close();

  try {
    GameField field(fileContent.str());
    if (!game.canCreateFieldWithSizes(field.getWidth(), field.getHeight())) {
      out << "Cannot place game field on this terminal size." << std::endl;
      return;
    }
    game.reset(field);
  } catch (BadGameFieldException& e) {
    out << "Cannot parse field: " << e.what() << std::endl;
    return;
  }

  out << "Game \"" << filename << "\" loaded successfully." << std::endl;
}

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

  while (true) {
    InputResult result = viewHandler.waitForInput(0);
    if (result.isKeyboard()) {
      if (result.getKey() == KEY_Q)
        break;
      else
        onKeyPressed(result.getKey());
    } else
      onMousePressed((int)result.getPosX(), (int)result.getPosY());
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

void GameManager::registerCommand(std::string name,
                                  void (*cmd)(const std::vector<std::string>&,
                                              GameManager&,
                                              std::ostream&)) {
  commands[name] = cmd;
}

bool GameManager::executeCommand(std::string name,
                                 std::vector<std::string>& args,
                                 std::ostream& output) {
  auto executor = commands.find(name);
  if (executor == commands.end())
    return false;
  executor->second(args, (*this), output);
  return true;
}

static std::vector<std::string> splitString(std::string str) {
  std::istringstream input(str);
  std::string item;
  std::vector<std::string> items;
  while (std::getline(input, item, ' '))
    items.push_back(item);
  return items;
}

void GameManager::executionInCommandMode() {
  std::vector<std::string> split = splitString(viewHandler.readCommandInput());
  
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
      setCellAt((int)cursorX, (int)cursorY);
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
