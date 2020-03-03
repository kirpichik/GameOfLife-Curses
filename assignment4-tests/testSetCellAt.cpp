#include <string>
#include <sstream>
#include <vector>
#include "gtest/gtest.h"

#include "game_handler.h"
#include "mockViewHandler.cpp"

TEST(GameManager::setCellAt, FiveFiveT) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  int posX = 5;
  int posY = 5;
  
  ASSERT_EQ(GameManager::setCellAt(posX, posY), False);
}

TEST(GameManager::setCellAt, FiveFiveF) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  int posX = 5;
  int posY = 5;
  
  ASSERT_EQ(GameManager::setCellAt(posX, posY), True);
}

TEST(GameManager::setCellAt, ZeroZeroT) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  int posX = 0;
  int posY = 0;
  
  ASSERT_EQ(GameManager::setCellAt(posX, posY), False);
}


TEST(GameManager::setCellAt, ZeroZeroF) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  int posX = 0;
  int posY = 0;
  
  ASSERT_EQ(GameManager::setCellAt(posX, posY), True);
}


TEST(GameManager::setCellAt, TenTenT) {
  MockViewHandler mockViewHandler;
  GameManager game(10, 10, mockViewHandler);
  int posX = 10;
  int posY = 10;
  
  ASSERT_EQ(GameManager::setCellAt(posX, posY), False);
}