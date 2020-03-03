#include "gtest/gtest.h"

#include "game_field.h"

TEST(GameField, cellPlacement){
    // create a blank field of a size of 10 x 10 
    GameField field(10, 10);

    // cycle through all the cell positions on the GameField
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            // check if all cells are blank initially
            ASSERT_FALSE(field[i][j].isLife());
        }
    }
}

TEST(GameField, spawnCell){
    GameField field(10, 10);

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            // spawn live cell
            field[i][j].bornLife();
            ASSERT_TRUE(field[i][j].isLife());
        }
    }
}

TEST(GameField, killCell){
    GameField field(10, 10);

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            // spawn live cell
            field[i][j].bornLife();

            // kill cell
            field[i][j].kill();
            ASSERT_FALSE(field[i][j].isLife());
        }
    }
}

TEST(GameField, spawnOutOfBounds){
    GameField field(10, 10);

    // spawn a cell out of bands
    field[-1][0].bornLife();
    ASSERT_TRUE(field[-1][0].isLife());
    ASSERT_TRUE(field[9][0].isLife());
}

TEST(GameField, killOutOfBounds){
    GameField field(10, 10);

    // spawn and kill a cell out of bounds
    field[9][0].bornLife();
    field[-1][0].kill();
    ASSERT_FALSE(field[-1][0].isLife());
    ASSERT_FALSE(field[9][0].isLife());
}