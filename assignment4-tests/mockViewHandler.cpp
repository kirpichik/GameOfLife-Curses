#include "game_handler.h"
#include "gtest/gtest.h"

// This class overrides methods that require 
// ncurses and waiting for user input
// so we can test without needing graphics
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