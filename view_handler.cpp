//
//  view_handler.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include <ncurses.h>

#include "view_handler.h"

// Maximum command length in command mode
const size_t MAX_COMMAND_LEN = 50;

CursesViewHandler::CursesViewHandler() {
  initscr();

// Need for external debug with curses in some IDEs.
// Define this macro globally, if you use same IDE.
#ifdef EXTERNAL_DEBUG
  getchar();
#endif

  clear();
  noecho();
  cbreak();

  mousemask(BUTTON1_PRESSED, NULL);
  keypad(stdscr, TRUE);
  curs_set(0);
}

void CursesViewHandler::updateField(const GameField& field, size_t stepsCount) {
  // If the field size has changed, clear the field and redraw everything
  if (field.getWidth() != gameWidth || field.getHeight() != gameHeight) {
    gameWidth = field.getWidth();
    gameHeight = field.getHeight();

    clear();

    // Display hotkey prompts
    int i = 0;
    const int coloumn = (int)gameWidth * 2;
    for (std::string prompt : PROMPTS) {
      move(i++, coloumn);
      attron(A_REVERSE);
      printw(" ");
      printw("%c", prompt[0]);
      printw(" ");
      attroff(A_REVERSE);
      printw(prompt.substr(1).c_str());
    }
  }

  // Display steps count
  move((int)PROMPTS.size() + 1, (int)gameWidth * 2);
  clrtoeol();
  printw("Step: %zd", stepsCount);

  // Display commandline
  drawCommandLine();

  // Draws field
  for (int i = 0; i < field.getWidth(); i++)
    for (int j = 0; j < field.getHeight(); j++) {
      move(j, i * 2);
      delch();
      insch(field[i][j].isLife() ? '#' : '.');
    }

  // Draws keyboard cursor
  updateKeyboardCursor(cursorX, cursorY);
}

void CursesViewHandler::updateKeyboardCursor(size_t posX, size_t posY) {
  if (posX != cursorX || posY != cursorY) {
    chtype c = mvinch(cursorY, cursorX * 2) & ~A_REVERSE;
    mvdelch(cursorY, cursorX * 2);
    mvinsch(cursorY, cursorX * 2, c);
  }

  cursorX = posX;
  cursorY = posY;

  chtype c = mvinch(cursorY, cursorX * 2) | A_REVERSE;
  mvdelch(cursorY, cursorX * 2);
  mvinsch(cursorY, cursorX * 2, c);
}

static std::string replace(const std::string& str,
                           const std::string& what,
                           const std::string& repl) {
  std::string result(str);
  size_t index = 0;
  while (true) {
    index = str.find(what, index);
    if (index == std::string::npos)
      break;
    result.replace(index, what.size(), repl);
    index += repl.size();
  }
  return result;
}

void CursesViewHandler::updateCommandLine(const std::string& commandOutput) {
  commandLine = replace(commandOutput, "%", "%%");
  drawCommandLine();
}

std::string CursesViewHandler::readCommandInput() {
  move((int)gameHeight + 1, 0);
  clrtobot();
  printw(">> ");
  keypad(stdscr, FALSE);
  curs_set(1);
  echo();

  char str[MAX_COMMAND_LEN];
  getnstr(str, MAX_COMMAND_LEN);

  keypad(stdscr, TRUE);
  curs_set(0);
  noecho();

  drawCommandLine();

  return std::string(str);
}

const InputResult CursesViewHandler::waitForInput(uint8_t timeout) {
  MEVENT mouse;
  int event;
  InputResult result;

  if (timeout != 0)
    halfdelay(timeout);

  bool repeat = true;
  while (repeat) {
    event = getch();
    if (event == KEY_MOUSE) {
      if (getmouse(&mouse) == OK) {
        int x = mouse.x / 2;
        int y = mouse.y;
        repeat = x >= gameWidth || y >= gameHeight;
        if (!repeat)
          result = InputResult(x, y);
      }
    } else if (event == ERR)
      repeat = (timeout == 0);
    else {
      repeat = false;
      result = InputResult(event);
    }
  }

  if (timeout != 0) {
    nocbreak();
    cbreak();
  }

  return result;
}

static size_t getMaxPromptWidth() {
  size_t max = 0;
  for (std::string prompt : PROMPTS)
    if (prompt.size() > max)
      max = prompt.size();
  return max + 2;  // To the length 2 spaces are added, framing the symbol of
                   // the hotkey.
}

bool CursesViewHandler::canCrateFieldWithSizes(size_t fieldWidth,
                                               size_t fieldHeight) {
  if (fieldWidth == 0 || fieldHeight == 0)
    return false;
  size_t maxWidth, maxHeight;
  getmaxyx(stdscr, maxHeight, maxWidth);  // TODO - check width validate!
  size_t width = fieldWidth + getMaxPromptWidth();
  size_t promptsHeight =
      PROMPTS.size() + 2;  // Offset for displaying the number of steps
  size_t height = fieldHeight > promptsHeight ? fieldHeight : promptsHeight;
  height += 2;  // Offset for command mode displaying
  return maxWidth >= width && maxHeight >= height;
}

void CursesViewHandler::drawCommandLine() const {
  move((int)gameHeight + 1, 0);
  clrtobot();
  printw(commandLine.c_str());
}

CursesViewHandler::~CursesViewHandler() {
  endwin();
}
