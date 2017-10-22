//
//  game_handler.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include <cstdlib>
#include <ncurses.h>
#include <fstream>

#include "game_handler.h"

const std::string DEFAULT_SAVE_FILENAME = "game_of_life.fld";

// Коды клавиш
const int KEY_N = 110;
const int KEY_B = 98;
const int KEY_R = 114;
const int KEY_C = 99;
const int KEY_I = 105;

#undef KEY_UP
#undef KEY_DOWN
#undef KEY_RIGHT
#undef KEY_LEFT
#undef KEY_ENTER

const int KEY_UP = 259;
const int KEY_DOWN = 258;
const int KEY_LEFT = 260;
const int KEY_RIGHT = 261;
const int KEY_ENTER = 10;

// Задержка в десятых долях секунды при обновлении следующего шага(для множественных шагов)
const size_t STEP_UPDATE_DELAY = 1;

// Максимальная длина команды в командном режиме
const size_t MAX_COMMAND_LEN = 50;

// Количество живых клеток вокруг мертвой для зарождения жизни.
const size_t BORN_LIFE = 3;

// Минимальное количество живых клеток вокруг живой для продолжения жизни.
const size_t DEATH_LONELINESS = 2;

// Максимальное количесвто живых клеток вокруг живой для продолжения жизни.
const size_t DEATH_OVERPOPULATION = 3;

size_t GameManager::getMaxPromptWidth() const {
    size_t max = 0;
    for (std::string prompt : PROMPTS)
        if (prompt.size() > max)
            max = prompt.size();
    return max + 2; // К длине добавляются 2 пробела, обрамляющие символ горячей клавиши.
}

bool GameManager::canCreateFieldWithSizes(size_t fieldWidth, size_t fieldHeight) const {
    if (fieldWidth == 0 || fieldHeight == 0)
        return false;
    size_t maxWidth, maxHeight;
    getmaxyx(stdscr, maxHeight, maxWidth);
    size_t width = fieldWidth + getMaxPromptWidth();
    size_t promptsHeight = PROMPTS.size() + 2; // Смещение для отображения количества шагов
    size_t height = fieldHeight > promptsHeight ? fieldHeight : promptsHeight;
    height += 2; // Смещение для командного режима
    return maxWidth >= width && maxHeight >= height;
}

// ==================== Обработчики команд ====================

/**
 * Очищает поле и сбрасывает счетчик команд.
 * Если передать в аргументы ширину и высоту, создаст поле с данными шириной и высотой.
 * Аргументы: [ширина] [высота]
 * */
static void commandReset(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    if (args.size() != 2) {
        if (game.canCreateFieldWithSizes(game.getCurrentField().getWidth(),
                                    game.getCurrentField().getHeight()))
            game.reset(game.getCurrentField().getWidth(), game.getCurrentField().getHeight());
        else {
            out << "Cannot place game field on this terminal size." << std::endl;
            return;
        }
    }
    else {
        int width = atoi(args[0].c_str());
        int height = atoi(args[1].c_str());
        if (game.canCreateFieldWithSizes(width, height))
            game.reset(width, height);
        else {
            out << "Cannot place game field on this terminal size." << std::endl;
            return;
        }
    }
    out << "Field reseted to size " << game.getCurrentField().getWidth() << "x" <<
    game.getCurrentField().getHeight() << "" << std::endl;
}

/**
 * Устанавливает или удаляет организм в клетке.
 * Аргументы: <позиция X> <позиция Y>
 * */
static void commandSet(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    if (args.size() != 2) {
        out << "Need args: <pos X> <pos Y>" << std::endl;
        return;
    }
    int posX = atoi(args[0].c_str());
    int posY = atoi(args[1].c_str());
    
    out << "Cell " << (game.setCellAt(posX, posY) ? "spawned." : "killed.") << std::endl;
}

/**
 * Выполняет переданное кол-во шагов. Если аргумент отсутствует, выполняет 1 шаг.
 * Если агрумент '-', выполняет бесконечное количество шагов, пока не будет нажата клавиша 'I'.
 * Аргументы: [кол-во шагов]
 * */
static void commandStep(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    size_t steps = 1;
    bool isInfinity = false;
    if (args.size() > 0) {
        if (args[0] == "-")
            isInfinity = true;
        else
            steps = atoi(args[0].c_str());
    }
    
    halfdelay(STEP_UPDATE_DELAY);
    int input;
    game.setLastCommandOutput("Making steps... Press I for interrupt.");
    
    size_t counter = 0;
    for (; counter < steps || isInfinity; counter++) {
        game.nextStep();
        input = getch();
        if (input == KEY_I)
            break;
    }
    
    nocbreak();
    cbreak();
    
    out << "Done " << (counter + 1) << " step(s)." << std::endl;
}

/**
 * Отменяет последний шаг.
 * Невозможно отменить более одного шага.
 * */
static void commandBack(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    if (game.stepBack())
        out << "Back step completed." << std::endl;
    else
        out << "It's impossible to step back." << std::endl;
}

/**
 * Сохраняет состояние поля в файл.
 * Аргументы: [имя файла]
 * */
static void commandSave(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    std::string filename = DEFAULT_SAVE_FILENAME;
    if (args.size() > 0)
        filename = args[0];
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        out << "Cannot create file \"" << filename << "\"." << std::endl;
        return;
    }
    
    file << game.getCurrentField() << std::endl;
    file.close();
    
    out << "Game field saved to \"" << filename << "\"." << std::endl;
}

/**
 * Загружает состояние поля из файла.
 * Аргументы: [имя файла]
 * */
static void commandLoad(const std::vector<std::string>& args, GameManager& game, std::ostream& out) {
    std::string filename = DEFAULT_SAVE_FILENAME;
    if (args.size() > 0)
        filename = args[0];
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        out << "Cannot load file \"" << filename << "\"" << std::endl;
        return;
    }
    
    std::ostringstream fileContent;
    std::string line;
    while(std::getline(file, line))
        fileContent << line << std::endl;
    file.close();
    
    try {
        GameField field(fileContent.str());
        if (!game.canCreateFieldWithSizes(field.getWidth(), field.getHeight())) {
            out << "Cannot place game field on this terminal size." << std::endl;
            return;
        }
        game.reset(field);
    } catch (BadGameFieldException& e) {
        out << "Cannot parse field: " << e.what() << std::endl;
        return;
    }
    
    out << "Game \"" << filename << "\" loaded successfully." << std::endl;
}

GameManager::GameManager(size_t width, size_t height, FieldUpdateListener& listener) :
    width(width), height(height),
    gameField(GameField(width, height)),
    updateListener(listener),
    previousStep(GameField(0, 0))
{
    registerCommand("reset", &commandReset);
    registerCommand("set", &commandSet);
    registerCommand("step", &commandStep);
    registerCommand("back", &commandBack);
    registerCommand("save", &commandSave);
    registerCommand("load", &commandLoad);
}

GameManager::GameManager(const GameField& field, FieldUpdateListener& listener) :
    width(field.getWidth()), height(field.getHeight()),
    gameField(field), updateListener(listener),
    previousStep(GameField(0, 0)) {}

size_t GameManager::countLifeAround(int posX, int posY) const {
    //   1 2 3
    // 1 # # #  }
    // 2 # . #  }- Точка это проверяемая ячейка
    // 3 # # #  }
    
    size_t lifes = 0; // Количество живых клеток вокруг
    
    lifes += previousStep[posX - 1][posY].isLife() ? 1 : 0; // 2-1
    lifes += previousStep[posX + 1][posY].isLife() ? 1 : 0; // 2-3
    
    // 1-1 -> 1-3
    for (int i = posX - 1; i <= posX + 1; i++)
        lifes += previousStep[i][posY - 1].isLife() ? 1 : 0;
    
    // 3-1 -> 3-3
    for (int i = posX - 1; i <= posX + 1; i++)
        lifes += previousStep[i][posY + 1].isLife() ? 1 : 0;
    
    return lifes;
}

void GameManager::nextStep() {
    previousStep = GameField(gameField);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            size_t life = countLifeAround(i, j);
            bool hasLife = previousStep[i][j].isLife();
            if (hasLife && (life < DEATH_LONELINESS || life > DEATH_OVERPOPULATION))
                gameField[i][j].kill();
            else if (!hasLife && life == BORN_LIFE)
                gameField[i][j].bornLife();
        }
    }
    stepsCounter++;
    hasUndo = true;
    update();
}

bool GameManager::setCellAt(int posX, int posY) {
    previousStep = GameField(gameField);
    if (gameField[posX][posY].isLife())
        gameField[posX][posY].kill();
    else
        gameField[posX][posY].bornLife();
    hasUndo = true;
    update();
    return gameField[posX][posY].isLife();
}

void GameManager::reset(size_t width, size_t height) {
    this->width = width;
    this->height = height;
    gameField = GameField(width, height);
    hasUndo = false;
    stepsCounter = 0;
    cursorY = cursorX = 0;
    update();
}

void GameManager::reset(const GameField &field) {
    width = field.getWidth();
    height = field.getHeight();
    gameField = GameField(field);
    hasUndo = false;
    stepsCounter = 0;
    cursorY = cursorX = 0;
    update();
}

bool GameManager::stepBack() {
    if (!hasUndo)
        return false;
    
    gameField = GameField(previousStep);
    hasUndo = false;
    stepsCounter--;
    update();
    
    return true;
}

void GameManager::update() const {
    updateListener.onUpdate(gameField);
    updateKeyboadCursor();
}

void GameManager::registerCommand(std::string name, void (*cmd)(const std::vector<std::string>&, GameManager&, std::ostream&)) {
    commands[name] = cmd;
}

bool GameManager::executeCommand(std::string name, std::vector<std::string>& args, std::ostream& output) {
    auto executor = commands.find(name);
    if (executor == commands.end())
        return false;
    executor->second(args, (*this), output);
    return true;
}

/**
 * Разбивает строку на пробелы.
 *
 * @param str Строка для разбития
 *
 * @return Вектор разбитых строк
 * */
static std::vector<std::string> splitString(std::string str) {
    std::istringstream input(str);
    std::string item;
    std::vector<std::string> items;
    while(std::getline(input, item, ' '))
        items.push_back(item);
    return items;
}

void GameManager::executionInCommandMode() {
    move((int) gameField.getHeight() + 1, 0);
    clrtobot();
    printw(">> ");
    keypad(stdscr, FALSE);
    curs_set(1);
    echo();
    
    char str[MAX_COMMAND_LEN];
    getnstr(str, MAX_COMMAND_LEN);
    
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    
    std::vector<std::string> split = splitString(std::string(str));
    
    std::string command = split[0];
    std::vector<std::string> args(split.begin() + 1, split.begin() + split.size());
    std::ostringstream out;
    
    lastCommandOutput = "";
    
    if (!executeCommand(command, args, out))
        out << "Command \"" << command << "\" not found.";
    
    move((int) gameField.getHeight() + 1, 0);
    clrtobot();
    lastCommandOutput = out.str();
    printw(lastCommandOutput.c_str());
}

void GameManager::updateKeyboadCursor() const {
    chtype c = mvinch(cursorY, cursorX * 2) | A_REVERSE;
    mvdelch(cursorY, cursorX * 2);
    mvinsch(cursorY, cursorX * 2, c);
}

void GameManager::onKeyboardCursor(int key) {
    // Удаление предыдущей метки
    chtype c = mvinch(cursorY, cursorX * 2) & ~A_REVERSE;
    mvdelch(cursorY, cursorX * 2);
    mvinsch(cursorY, cursorX * 2, c);
    switch (key) {
        case KEY_UP:
            if (cursorY != 0)
                cursorY--;
            break;
        case KEY_DOWN:
            if (cursorY + 1 != height)
                cursorY++;
            break;
        case KEY_LEFT:
            if (cursorX != 0)
                cursorX--;
            break;
        case KEY_RIGHT:
            if (cursorX + 1 != width)
                cursorX++;
            break;
    }
    updateKeyboadCursor();
}

void GameManager::onMousePressed(int x, int y) {
    x /= 2;
    if (x < 0 ||
        y < 0 ||
        x >= width ||
        y >= height)
        return;
    cursorX = x;
    cursorY = y;
    setCellAt(x, y);
}

void GameManager::onKeyPressed(int key) {
    switch (key) {
        case KEY_N:
            nextStep();
            break;
        case KEY_B:
            lastCommandOutput = stepBack() ? "Back step completed."
                : "It's impossible to step back.";
            break;
        case KEY_R:
            lastCommandOutput = "Field reseted.";
            reset(width, height);
            break;
        case KEY_C:
            executionInCommandMode();
            break;
        case KEY_ENTER:
            setCellAt((int) cursorX, (int) cursorY);
            break;
        case KEY_LEFT:
        case KEY_RIGHT:
        case KEY_UP:
        case KEY_DOWN:
            onKeyboardCursor(key);
            break;
        default:
            break;
    }
}

const GameField GameManager::getCurrentField() const {
    return gameField;
}

size_t GameManager::getStepsCount() const {
    return stepsCounter;
}

std::string GameManager::getLastCommandOutput() const {
    return lastCommandOutput;
}

void GameManager::setLastCommandOutput(const std::string output) {
    lastCommandOutput = output;
}
