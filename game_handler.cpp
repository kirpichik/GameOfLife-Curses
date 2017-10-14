//
//  game_handler.cpp
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include "game_handler.h"
#include <cstdlib>

// Количество живых клеток вокруг мертвой для зарождения жизни.
static const size_t BORN_LIFE = 3;

// Минимальное количество живых клеток вокруг живой для продолжения жизни.
static const size_t DEATH_LONELINESS = 2;

// Максимальное количесвто живых клеток вокруг живой для продолжения жизни.
static const size_t DEATH_OVERPOPULATION = 3;

// ==================== Обработчики команд ====================

/**
 * Очищает поле и сбрасывает счетчик команд.
 * */
static void commandReset(std::vector<std::string> args, GameManager& game, std::ostream& out) {
    game.reset();
    out << "Reset completed." << std::endl;
}

/**
 * Устанавливает или удаляет организм в клетку.
 * Аргументы: <позиция X> <позиция Y>
 * */
static void commandSet(std::vector<std::string> args, GameManager& game, std::ostream& out) {
    if (args.size() != 2) {
        out << "Need args: <pos X> <pos Y>" << std::endl;
        return;
    }
    int posX = atoi(args[0].c_str());
    int posY = atoi(args[1].c_str());
    
    out << "Cell " << (game.setCellAt(posX, posY) ? "killed." : "spawned.") << std::endl;
}

/**
 * Выполняет переданное кол-во шагов. Если аргумент отсутствует, выполняет 1 шаг.
 * Аргументы: [кол-во шагов]
 * */
static void commandStep(std::vector<std::string> args, GameManager& game, std::ostream& out) {
    size_t steps = 1;
    if (args.size() > 0)
        steps = atoi(args[0].c_str());
    
    out << "Making steps..." << std::endl;
    
    game.manySteps(steps);
    
    out << "Steps done." << std::endl;
}

/**
 * Отменяет последний шаг.
 * Невозможно отменить более одного шага.
 * */
static void commandBack(std::vector<std::string> args, GameManager& game, std::ostream& out) {
    game.stepBack();
    out << "Back step completed." << std::endl;
}

/**
 * Сохраняет состояние поля в файл.
 * Аргументы: <имя файла>
 * */
static void commandSave(std::vector<std::string> args, GameManager& game, std::ostream& out) {
    // TODO - сохранение в файл
    out << "Not yet implemented." << std::endl;
}

/**
 * Загружает состояние поля из файла.
 * Аргументы: <имя файла>
 * */
static void commandLoad(std::vector<std::string> args, GameManager& game, std::ostream& out) {
    // TODO - загрузка из файла
    out << "Not yet implemented." << std::endl;
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

size_t GameManager::countLifeAround(int posX, int posY) const {
    //   1 2 3
    // 1 # # #  }
    // 2 # . #  }- Точка это проверяемая ячейка
    // 3 # # #  }
    
    size_t lifes = 0; // Количество живых клеток вокруг
    
    lifes += gameField[posX - 1][posY].isLife() ? 1 : 0; // 2-1
    lifes += gameField[posX + 1][posY].isLife() ? 1 : 0; // 2-3
    
    // 1-1 -> 1-3
    for (int i = posX - 1; i <= posX + 1; i++)
        lifes += gameField[i][posY - 1].isLife() ? 1 : 0;
    
    // 3-1 -> 3-3
    for (int i = posX - 1; i <= posX + 1; i++)
        lifes += gameField[i][posY + 1].isLife() ? 1 : 0;
    
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

void GameManager::manySteps(size_t count) {
    for (size_t i = 0; i < count; i++)
        nextStep();
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

void GameManager::reset() {
    gameField = GameField(width, height);
    update();
}

bool GameManager::stepBack() {
    if (!hasUndo)
        return false;
    
    gameField = GameField(previousStep);
    hasUndo = false;
    update();
    
    return true;
}

void GameManager::update() const {
    updateListener.onUpdate(gameField);
}

void GameManager::registerCommand(std::string name, void (*cmd)(const std::vector<std::string>, GameManager&, std::ostream&)) {
    commands[name] = cmd;
}

bool GameManager::executeCommand(std::string name, std::vector<std::string> args, std::ostream& output) {
    auto executor = commands.find(name);
    if (executor == commands.end())
        return false;
    executor->second(args, (*this), output);
    return true;
}
