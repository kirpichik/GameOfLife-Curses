#include <string>
#include <sstream>
#include <vector>
#include "gtest/gtest.h"

#include "game_handler.h"

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
