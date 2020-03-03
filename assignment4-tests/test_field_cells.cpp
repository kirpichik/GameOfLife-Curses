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

            // spawn live cell
            field[i][j].bornLife();
            ASSERT_TRUE(field[i][j].isLife());

            // kill cell
            field[i][j].kill();
            ASSERT_FALSE(field[i][j].isLife());
        }
    }



}