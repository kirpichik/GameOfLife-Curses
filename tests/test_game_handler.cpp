//
//  test_game_handler.cpp
//  GameOfLiveTests
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include <string>
#include <sstream>
#include "gtest/gtest.h"

#include "game_handler.h"

std::string fieldToString(const GameField& field) {
    std::ostringstream str;
    str << field;
    return str.str();
}

class TestingListener : public ViewHandler {
public:
    
    void updateField(const GameField& field, size_t stepsCount) override {
        if (sample)
            ASSERT_EQ((*sample), field);
    }
    
    void updateKeyboardCursor(size_t posX, size_t posY) override {}
    
    void updateCommandLine(const std::string& commandOutput) override {}

    std::string readCommandInput() override {
        return "";
    }
    
    const InputResult waitForInput(uint8_t timeout) override {
        return InputResult();
    }
    
    bool canCrateFieldWithSizes(size_t width, size_t height) override {
        return true;
    }
    
    void setSampleEquality(const GameField* sample) {
        ASSERT_TRUE(sample != nullptr);
        if (this->sample)
            delete this->sample;
        this->sample = sample;
    }
    
    void cancelCathing() {
        sample = nullptr;
    }
private:
    const GameField* sample = nullptr;
};

void testWrongParseField(const std::string& str) {
    ASSERT_THROW(const GameField field(str), BadGameFieldException);
}

void testParseField(const std::string& str) {
    const GameField field(str);
    ASSERT_EQ(str, fieldToString(field));
}

void placeFieldOnField(GameField& to, const GameField& from) {
    for (int i = 0; i < from.getWidth(); i++)
        for (int j = 0; j < from.getHeight(); j++)
            if (from[i][j].isLife())
                to[i][j].bornLife();
            else
                to[i][j].kill();
}

void testGameStep(const std::string& from, const std::string& to) {
    TestingListener catcher;
    GameField fieldFrom(from);
    GameField bigFrom(10, 10);
    placeFieldOnField(bigFrom, fieldFrom);
    GameManager game(bigFrom, catcher);
    
    GameField field(to);
    GameField sample(10, 10);
    placeFieldOnField(sample, field);
    
    catcher.setSampleEquality(&sample);
    
    game.nextStep();
}

TEST(GameHandler, ParseFieldRight) {
    testParseField("");
    testParseField("###\n###\n###");
    testParseField("######");
    testParseField("......");
    testParseField("...\n...\n...");
    testParseField(".");
    testParseField("#");
    testParseField("###\n###");
    testParseField("##\n##\n##");
    testParseField(".\n.\n.\n.\n.");
}

TEST(GameHandler, ParseFieldWrongSymbols) {
    testWrongParseField("a");
    testWrongParseField("##\nbc\n##");
    testWrongParseField("...\n.de.\n...");
    testWrongParseField("#\nf");
}

TEST(GameHandler, ParseFieldWrongSize) {
    testWrongParseField("##\n#");
    testWrongParseField("...\n..");
    testWrongParseField(".\n\n\n");
    testWrongParseField("##\n.\n##");
}

TEST(GameHandler, NextStep) {
    testGameStep(".#..\n.#..\n.#..", "....\n###.\n....");
    testGameStep("#", ".");
    testGameStep("....\n.###\n.###\n.###", "..#..\n.#.#.\n#...#\n.#.#.\n..#..");
}
/*
 .....  ..#..
 .###.  .#.#.
 .###.  #...#
 .###.  .#.#.
 .....  ..#..
*/
TEST(GameHandler, Reset) {
    TestingListener catcher;
    GameManager game(10, 10, catcher);
    const GameField sample = game.getCurrentField();
    catcher.setSampleEquality(&sample);
    
    game.reset(game.getCurrentField().getWidth(), game.getCurrentField().getHeight());
    
    catcher.cancelCathing();
    for (int i = 0; i < 10; i++)
        game.setCellAt(i, i);
    
    catcher.setSampleEquality(&sample);
    
    game.reset(game.getCurrentField().getWidth(), game.getCurrentField().getHeight());
}

TEST(GameHandler, StepBack) {
    TestingListener catcher;
    GameManager game(10, 10, catcher);
    const GameField sample = game.getCurrentField();
    ASSERT_FALSE(game.stepBack());
    
    game.setCellAt(5, 5);
    
    catcher.setSampleEquality(&sample);
    
    ASSERT_TRUE(game.stepBack());
    ASSERT_FALSE(game.stepBack());
    
    catcher.cancelCathing();
    
    for (int i = 0; i < 10; i++)
        game.setCellAt(i, i);
    
    for (int i = 0; i < 10; i++)
        game.setCellAt(i, i);
    
    game.setCellAt(0, 0);
    
    catcher.setSampleEquality(&sample);
    
    game.stepBack();
}
