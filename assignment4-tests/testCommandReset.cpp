include <string>
#include <sstream>
#include <vector>
#include "gtest/gtest.h"

#include "game_handler.h"

static void commandReset(const std::vector<std::string>& args,
                         GameManager& game,
                         std::ostream& out) {
  if (args.size() != 2) {
    if (game.canCreateFieldWithSizes(game.getWidth(), game.getHeight()))
      game.reset(game.getWidth(), game.getHeight());
    else {
      out << "Cannot place game field on this terminal size." << std::endl;
      return;
    }
  } else {
    int width = stoi(args[0]);
    int height = stoi(args[1]);
    if (game.canCreateFieldWithSizes(width, height))
      game.reset(width, height);
    else {
      out << "Cannot place game field on this terminal size." << std::endl;
      return;
    }
  }
  out << "Field reseted to size " << game.getWidth() << "x" << game.getHeight()
      << "" << std::endl;
}

class MockViewHandler : public ViewHandler {
public:
    void updateCommandLine(const std::string& commandOutput) override {}

    void updateField(const GameField& field, size_t stepsCount) override {}

    void updateKeyboardCursor(size_t posX, size_t posY) override {}


    std::string readCommandInput() override {
        return "";
    }

    const InputResult waitForInput(uint8_t timeout) override {
        return InputResult();
    }

    bool canCrateFieldWithSizes(size_t width, size_t height) override {
        return true;
    }

    void setField(const GameField* field) {
        ASSERT_TRUE(field != nullptr);
        if (this->field)
            delete this->field;
        this->field = field;
    }

    void removeField() {
      field = nullptr;
    }
private:
    const GameField* field = nullptr;
};

TEST(GameHandler, ValidInput) {
  MockViewHandler viewhandler;
  GameManager game(10, 10, viewhandler);
  std::vector<std::string> args = {"3", "3"};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.getWidth(), 3);
  ASSERT_EQ(game.getHeight(), 3);
}

TEST(GameHandler, ThreeInputs) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"3", "3", "3"};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.getWidth(), 10);
  ASSERT_EQ(game.getHeight(), 10);
}

TEST(GameHandler, OneInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"3"};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.getWidth(), 10);
  ASSERT_EQ(game.getHeight(), 10);
}

TEST(GameHandler, NoInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.getWidth(), 10);
  ASSERT_EQ(game.getHeight(), 10);
}

TEST(GameHandler, NonsquareInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"2", "6"};
  commandReset(args, game, std::cout);
  ASSERT_EQ(game.getWidth(), 2);
  ASSERT_EQ(game.getHeight(), 6);
}
                                       
