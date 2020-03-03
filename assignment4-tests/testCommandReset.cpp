#include <string>
#include <sstream>
#include <vector>
#include "gtest/gtest.h"

#include "game_handler.h"

TEST(GameHandler, ValidInput) {
  TestingListener catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {3, 3};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.width, 3);
  ASSERT_EQ(game.height, 3);
}

TEST(GameHandler, ThreeInputs) {
  TestingListener catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {3, 3, 3};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.width, 10);
  ASSERT_EQ(game.height, 10);
}

TEST(GameHandler, OneInput) {
  TestingListener catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {3};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.width, 10);
  ASSERT_EQ(game.height, 10);
}

TEST(GameHandler, NoInput) {
  TestingListener catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.width, 10);
  ASSERT_EQ(game.height, 10);
}

TEST(GameHandler, NonsquareInput) {
  TestingListener catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = [2, 6];
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.width, 2);
  ASSERT_EQ(game.height, 6);
}
