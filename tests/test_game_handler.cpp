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

/**
 * Вывод поля в строку.
 * TODO - передвинуть в сохранение/загрузку состояния игры.
 * */
std::string fieldToString(const GameField& field) {
    std::ostringstream str;
    str << field;
    return str.str();
}

/**
 * Создание из поля строки.
 * # - ячейка с клеткой
 * . - ячейка без клетки
 * TODO - передвинуть в сохранение/загрузку состояния игры.
 * */
const GameField* parseField(std::string str) {
    std::vector<std::vector<bool>> field;
    field.push_back(std::vector<bool>());
    size_t line = 0;
    size_t maxWidth = 0;
    size_t currWidth = 0;
    for (size_t i = 0; i < str.size(); i++) {
        switch (str[i]) {
            case '\n':
                if (maxWidth != currWidth)
                    return nullptr;
                line++;
                currWidth = 0;
                field.push_back(std::vector<bool>());
                break;
            case '#':
                if (line == 0)
                    maxWidth++;
                currWidth++;
                field[line].push_back(true);
                break;
            case '.':
                if (line == 0)
                    maxWidth++;
                currWidth++;
                field[line].push_back(false);
                break;
            case ' ':
            case '\r':
                continue;
            default:
                return nullptr;
        }
    }
    if (maxWidth != currWidth)
        return nullptr;
    if (maxWidth != 0)
        line++;
    if (currWidth == 0 && line != 0) {
        field.pop_back();
        line--;
    }
    
    GameField* game = new GameField(line, maxWidth);
    for (int i = 0; i < line; i++)
        for (int j = 0; j < maxWidth; j++)
            if (field[i][j])
                (*game)[i][j].bornLife();
    
    return game;
}

/**
 * Тестовый слушатель обновления поля.
 * */
class TestingListener : public FieldUpdateListener {
public:
    
    void onUpdate(const GameField& field) override {
        if (sample)
            ASSERT_EQ((*sample), field);
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

/**
 * Тестирование неправильных случаев создания поля из строки.
 * */
void testWrongParseField(std::string str) {
    const GameField* field = parseField(str);
    ASSERT_EQ(field, nullptr);
}

/**
 * Тестирование правильных случаев создания поля из строки.
 * */
void testParseField(std::string str) {
    const GameField* field = parseField(str);
    ASSERT_TRUE(field != NULL) << "Value: " << field;
    ASSERT_EQ(str, fieldToString((*field)));
    delete field;
}

/**
 * Размещает в игре клетки из поля.
 * */
void importGameField(GameManager& game, const GameField& field) {
    game.reset();
    for (int i = 0; i < field.getWidth(); i++)
        for (int j = 0; j < field.getHeight(); j++)
            if (field[i][j].isLife())
                game.setCellAt(i, j);
}

/**
 * Размещает меньшее поле в левом верхнем углу большего поля.
 * */
void placeFieldOnField(GameField& to, const GameField& from) {
    for (int i = 0; i < from.getWidth(); i++)
        for (int j = 0; j < from.getHeight(); j++)
            if (from[i][j].isLife())
                to[i][j].bornLife();
            else
                to[i][j].kill();
}

/**
 * Тестирование шагов.
 * */
void testGameStep(std::string from, std::string to) {
    TestingListener catcher;
    GameManager game(10, 10, catcher);
    
    const GameField* field = parseField(from);
    importGameField(game, (*field));
    delete field;
    
    field = parseField(to);
    GameField sample(10, 10);
    placeFieldOnField(sample, (*field));
    
    catcher.setSampleEquality(&sample);
    delete field;
    
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
    testWrongParseField(".\n");
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
    
    game.reset();
    
    catcher.cancelCathing();
    for (int i = 0; i < 10; i++)
        game.setCellAt(i, i);
    
    catcher.setSampleEquality(&sample);
    
    game.reset();
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
