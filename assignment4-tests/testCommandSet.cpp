#include <string>
#include <sstream>
#include <vector>
#include "gtest/gtest.h"

#include "game_handler.h"

static void commandSet(const std::vector<std::string>& args,
                       GameManager& game,
                       std::ostream& out) {
  if (args.size() != 2) {
    out << "Need args: <pos X> <pos Y>" << std::endl;
    return;
  }
  int posX = atoi(args[0].c_str());
  int posY = atoi(args[1].c_str());

  out << "Cell " << (game.setCellAt(posX, posY) ? "spawned." : "killed.")
      << std::endl;
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
 commandSet(args, game, std::cout);
  ASSERT_TRUE(game.gameField[3][3].isLife());
}

TEST(GameHandler, ThreeInputs) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"3", "3", "3"};
  commandSet(args, game, std::cout);
  ASSERT_FALSE(game.gameField[3][3].isLife());
}

TEST(GameHandler, OneInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"3"};
  commandSet(args, game, std::cout);
  ASSERT_FALSE(game.gameField[3][3].isLife());
}

TEST(GameHandler, NoInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {};
  commandSet(args, game, std::cout);
  ASSERT_FALSE(game.gameField[3][3].isLife());
}

TEST(GameHandler, NonsquareInput) {
  MockViewHandler catcher;
  GameManager game(10, 10, catcher);
  std::vector<std::string> args = {"2", "6"};
  commandSet(args, game, std::cout);
  ASSERT_TRUE(game.gameField[2][6].isLife());
}
