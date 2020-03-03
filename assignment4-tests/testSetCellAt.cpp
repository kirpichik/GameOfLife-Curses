#include <string>
#include <sstream>
#include <vector>
#include "gtest/gtest.h"

#include "mockViewHandler.cpp"

TEST(GameManager, FiveFiveT) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  int posX = 5;
  int posY = 5;
  
  ASSERT_FALSE(game.setCellAt(posX, posY));
}

TEST(GameManager, FiveFiveF) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  int posX = 5;
  int posY = 5;
  
  ASSERT_TRUE(game.setCellAt(posX, posY));
}

TEST(GameManager, ZeroZeroT) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  int posX = 0;
  int posY = 0;
  
  ASSERT_FALSE(game.setCellAt(posX, posY));
}


TEST(GameManager, ZeroZeroF) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  int posX = 0;
  int posY = 0;
  
  ASSERT_TRUE(game.setCellAt(posX, posY));
}


TEST(GameManager, TenTenT) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  int posX = 10;
  int posY = 10;
  
  ASSERT_FALSE(game.setCellAt(posX, posY));
}