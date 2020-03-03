#include <string>
#include <sstream>
#include <vector>
#include "gtest/gtest.h"

#include "mockViewHandler.cpp"


TEST(CommandReset, ValidInput) {
  MockViewHandler viewhandler;
  GameManager game(10, 10, viewhandler);
  std::vector<std::string> args = {"3", "3"};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.getWidth(), 3);
  ASSERT_EQ(game.getHeight(), 3);
}

TEST(CommandReset, ThreeInputs) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"3", "3", "3"};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.getWidth(), 10);
  ASSERT_EQ(game.getHeight(), 10);
}

TEST(CommandReset, OneInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"3"};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.getWidth(), 10);
  ASSERT_EQ(game.getHeight(), 10);
}

TEST(CommandReset, NoInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.getWidth(), 10);
  ASSERT_EQ(game.getHeight(), 10);
}

TEST(CommandReset, NonsquareInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"2", "6"};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.getWidth(), 2);
  ASSERT_EQ(game.getHeight(), 6);
}
                                       
