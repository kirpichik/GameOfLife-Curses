#include "gtest/gtest.h"

#include "game_field.h"
#include "mockViewHandler.cpp"

TEST(CountLife, CountCircle) {
    MockViewHandler mockViewHandler;
    GameManager game(5, 5, mockViewHandler);
    game.setCellAt(0,0);
    game.setCellAt(0,1);
    game.setCellAt(0,2);
    game.setCellAt(1,0);
    game.setCellAt(1,2);
    game.setCellAt(2,0);
    game.setCellAt(2,1);
    game.setCellAt(2,2);
    game.setCellAt(2,2);    // required to update prev field for countLifeAround
    ASSERT_EQ(8, game.countLifeAround(1, 1));
}