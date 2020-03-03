#include <string>
#include <sstream>
#include <vector>
#include "gtest/gtest.h"

#include "mockViewHandler.cpp"

TEST(CommandSet, ValidInput) {
  MockViewHandler viewhandler;
  GameManager game(10, 10, viewhandler);
  std::vector<std::string> args = {"3", "3"};
 commandSet(args, game, std::cout);
  ASSERT_TRUE(game.getCurrentField()[3][3].isLife());
}

TEST(CommandSet, ThreeInputs) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"3", "3", "3"};
  commandSet(args, game, std::cout);
  ASSERT_FALSE(game.getCurrentField()[3][3].isLife());
}

TEST(CommandSet, OneInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"3"};
  commandSet(args, game, std::cout);
  ASSERT_FALSE(game.getCurrentField()[3][3].isLife());
}

TEST(CommandSet, NoInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {};
  commandSet(args, game, std::cout);
  ASSERT_FALSE(game.getCurrentField()[3][3].isLife());
}

TEST(CommandSet, NonsquareInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"2", "6"};
  commandSet(args, game, std::cout);
  ASSERT_TRUE(game.getCurrentField()[2][6].isLife());
}
