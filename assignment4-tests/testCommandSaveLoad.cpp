#include <string>
#include <sstream>
#include <vector>
#include "gtest/gtest.h"

#include "game_handler.h"
#include "mockViewHandler.cpp"

//load a file that doesn't exist
TEST(commandLoad, loadBad) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  std::vector<std::string> args = {"doesntExist.fld"};
  
  commandLoad(args, game, std::cout);
  
  ASSERT_EQ(out, "Cannot load file \"doesntExist.fld\"\n");
  //ASSERT_EQ(file.is_open(), False);
}

//save a test file
TEST(commandSave, saveTest) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  std::vector<std::string> args = {"testFile1.fld"};
  
  commandSave(args, game, std::cout);
  
  //ASSERT_EQ(file.is_open(), True);
  ASSERT_EQ(out, "Game field saved to \"testFile1.fld\".\n");
}

//load a test file
TEST(commandLoad, loadTest) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  std::vector<std::string> args = {"testFile1.fld"};
  
  commandLoad(args, game, std::cout);
  
  //ASSERT_EQ(file.is_open(), True);
  ASSERT_EQ(out, "Game \"testFile1.fld\" loaded successfully.\n");
}

//save a file with no name
TEST(commandSave, saveNone) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  std::vector<std::string> args = {};             //array empty?
  
  commandSave(args, game, std::cout);
  
  //ASSERT_EQ(file.is_open(), True);
  ASSERT_EQ(out, "Game field saved to \"game_of_life.fld\".\n");
}

//load a file with no name
TEST(commandLoad, loadNone) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  std::vector<std::string> args = {};             //array empty?
  
  commandLoad(args, game, std::cout);
  
  //ASSERT_EQ(file.is_open(), True);
  ASSERT_EQ(out, "Game \"game_of_life.fld\" loaded successfully.\n");
}