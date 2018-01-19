//
//  view_handler.h
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef VIEW_HANDLER_H
#define VIEW_HANDLER_H

#include "game_handler.h"

const std::vector<std::string> PROMPTS = {
    "Q Exit", "N Next turn", "B Step Back", "R Reset", "C Command mode"};

class CursesViewHandler : public ViewHandler {
 public:
  CursesViewHandler();

  void updateField(const GameField& field, size_t stepsCount) override;

  void updateKeyboardCursor(size_t posX, size_t posY) override;

  void updateCommandLine(const std::string& commandOutput) override;

  std::string readCommandInput() override;

  const InputResult waitForInput(uint8_t timeout) override;

  bool canCrateFieldWithSizes(size_t width, size_t height) override;

  ~CursesViewHandler();

 private:
  // Commandline output
  std::string commandLine;

  // Keyboard cursor position
  size_t cursorX = 0;
  size_t cursorY = 0;

  // Current field dimentions
  size_t gameWidth = 0;
  size_t gameHeight = 0;

  void drawCommandLine() const;
};

#endif /* VIEW_HANDLER_H */
