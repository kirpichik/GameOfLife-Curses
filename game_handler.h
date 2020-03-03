//
//  game_handler.h
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef GAME_HANDLER_H
#define GAME_HANDLER_H

#include <cstdint>
#include <map>
#include <ostream>
#include <string>

#include "game_field.h"

class InputResult {
 public:
  /**
   * Event is thrown by timeout.
   */
  InputResult() : timedout(true) {}

  /**
   * Event is thrown by mouse click.
   */
  InputResult(size_t posX, size_t posY)
      : timedout(false), keyboard(false), posX(posX), posY(posY) {}

  /**
   * Event is thrown by key pressed.
   */
  InputResult(int key) : timedout(false), keyboard(true), key(key) {}

  int getKey() const { return isKeyboard() ? key : 0; }

  size_t getPosX() const { return isKeyboard() ? 0 : posX; }

  size_t getPosY() const { return isKeyboard() ? 0 : posY; }

  bool isTimedOut() const { return timedout; }

  bool isKeyboard() const { return keyboard && !timedout; }

  InputResult& operator=(const InputResult& copy) {
    if (&copy != this) {
      posX = copy.posX;
      posY = copy.posY;
      keyboard = copy.keyboard;
      timedout = copy.timedout;
      key = copy.key;
    }
    return (*this);
  }

 private:
  // Mouse click position
  size_t posX, posY;

  // If this is keyboard event
  bool keyboard;

  // If this is timeout event
  bool timedout;

  // Key code
  int key;
};

class ViewHandler {
 public:
  /**
   * Draws field and prompts.
   */
  virtual void updateField(const GameField& field, size_t stepsCount) = 0;

  /**
   * Draws keyboard cursor on field.
   */
  virtual void updateKeyboardCursor(size_t posX, size_t posY) = 0;

  /**
   * Draws output from commands.
   */
  virtual void updateCommandLine(const std::string& commandOutput) = 0;

  /**
   * Reads command input from user.
   *
   * @return User input.
   */
  virtual std::string readCommandInput() = 0;

  /**
   * Waits for the key press/mouse click/timeout expiration.
   *
   * @param timeout Input timeout in tenths of a second. From 0 to 255.
   *
   * @return Wait result.
   */
  virtual const InputResult waitForInput(uint8_t timeout) = 0;

  /**
   * Checks whether it is possible to create a field with the given dimensions
   * on this terminal.
   */
  virtual bool canCrateFieldWithSizes(size_t width, size_t height) = 0;
};

class GameManager {
 public:
  GameManager(size_t width, size_t height, ViewHandler& viewHandler);

  GameManager(const GameField& field, ViewHandler& viewHandler)
      : width(field.getWidth()),
        height(field.getHeight()),
        gameField(field),
        previousStep(GameField(0, 0)),
        viewHandler(viewHandler) {}

  int runGame();

  void nextStep();

  /**
   * If life existed at that position, it dies.
   * If there was no life, it borns.
   */
  bool setCellAt(int posX, int posY);

  /**
   * Clears the field, resets the steps counter and creates a field with new
   * dimensions.
   */
  void reset(size_t width, size_t height);

  /**
   * Sets new field and resets the steps counter.
   */
  void reset(const GameField& field);

  /**
   * Cancels last step.
   * You can cancel only one step.
   *
   * @return true, if step succesfully cancelled.
   */
  bool stepBack();

  /**
   * Registers the function of the command handler.
   * @param name Command name.
   * @param cmd Command handler.
   */
  void registerCommand(const std::string& name,
                       void (*cmd)(const std::vector<std::string>&,
                                   GameManager&,
                                   std::ostream&));

  /**
   * Counts the number of living cells around this cell.
   *
   * @return Number of living cells around
   */
  size_t countLifeAround(int posX, int posY) const;

  /**
   * Checks whether it is possible to create a field with the given dimensions
   * on this terminal.
   */
  bool canCreateFieldWithSizes(size_t fieldWidth, size_t fieldHeight) const;

  const GameField getCurrentField() const;

  size_t getWidth() const;

  size_t getHeight() const;

  ViewHandler& getViewHandler();

 private:
  size_t width;
  size_t height;

  std::map<
      std::string,
      void (*)(const std::vector<std::string>&, GameManager&, std::ostream&)>
      commands;

  GameField gameField;
  GameField previousStep;

  ViewHandler& viewHandler;

  size_t stepsCounter = 0;
  bool hasUndo = false;  // Is it possible to cancel at this step

  // Keyboard cursor on field position
  size_t cursorX = 0;
  size_t cursorY = 0;

  /**
   * Forces the update view handler without making any changes to the state of
   * the field.
   */
  void update();

  /**
   * Activates the command mode and waits for one command to be entered.
   * After the execution of the command goes into normal mode.
   */
  void executionInCommandMode();

  /**
   * Execute command by name.
   * @param name Command name.
   * @param args Command arguments.
   *
   * @return true, if command successfully executed.
   */
  bool executeCommand(const std::string& name,
                      const std::vector<std::string>& args,
                      std::ostream& output);

  /**
   * Catch mouse clicks.
   */
  void onMousePressed(int x, int y);

  /**
   * Catch key presses.
   */
  void onKeyPressed(int key);

  /**
   * Manages the cursor to set the cells on the field using the arrows on the
   * keyboard.
   */
  void onKeyboardCursor(int key);
};

#endif /* GAME_HANDLER_H */
