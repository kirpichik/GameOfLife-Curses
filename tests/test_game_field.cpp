//
//  test_game_field.cpp
//  GameOfLiveTests
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include "gtest/gtest.h"

#include "game_field.h"

TEST(GameField, LifeManipulations) {
    GameField field(10, 10);
    
    for (int i = 0; i < 10; i++) {
        ASSERT_FALSE(field[i][i].isLife());
    
        field[i][i].kill();
        ASSERT_FALSE(field[i][i].isLife());
    
        field[i][i].bornLife();
        ASSERT_TRUE(field[i][i].isLife());
    }

    for (int i = 0; i < 10; i++) {
        field[i][i].kill();
        ASSERT_FALSE(field[i][i].isLife());
    }
}

TEST(GameField, LifeManipulationsWithOverflow) {
    GameField field(10, 10);
    
    ASSERT_FALSE(field[-1][0].isLife());
    ASSERT_FALSE(field[9][0].isLife());
    
    ASSERT_FALSE(field[0][-1].isLife());
    ASSERT_FALSE(field[0][9].isLife());
    
    ASSERT_FALSE(field[10][0].isLife());
    ASSERT_FALSE(field[0][0].isLife());
    
    ASSERT_FALSE(field[0][10].isLife());
    ASSERT_FALSE(field[0][0].isLife());
    
    ASSERT_FALSE(field[-1][-1].isLife());
    ASSERT_FALSE(field[9][9].isLife());
    
    ASSERT_FALSE(field[10][10].isLife());
    ASSERT_FALSE(field[0][0].isLife());
    
    field[-1][0].bornLife();
    field[0][-1].bornLife();
    field[10][0].bornLife();
    field[0][10].bornLife();
    field[-1][-1].bornLife();
    field[10][10].bornLife();
    
    ASSERT_TRUE(field[-1][0].isLife());
    ASSERT_TRUE(field[9][0].isLife());
    
    ASSERT_TRUE(field[0][-1].isLife());
    ASSERT_TRUE(field[0][9].isLife());
    
    ASSERT_TRUE(field[10][0].isLife());
    ASSERT_TRUE(field[0][0].isLife());
    
    ASSERT_TRUE(field[0][10].isLife());
    ASSERT_TRUE(field[0][0].isLife());
    
    ASSERT_TRUE(field[-1][-1].isLife());
    ASSERT_TRUE(field[9][9].isLife());
    
    ASSERT_TRUE(field[10][10].isLife());
    ASSERT_TRUE(field[0][0].isLife());
}
