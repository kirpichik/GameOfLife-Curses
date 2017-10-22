//
//  view_handler.h
//  GameOfLive
//
//  Created by Кирилл on 10.10.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef view_handler_h
#define view_handler_h

#include "game_handler.h"

class ViewHandler : FieldUpdateListener {
public:
    
    ViewHandler(size_t width, size_t height);
    
    void onUpdate(const GameField& field) override;
    
    /**
     * Запускает игру.
     * */
    int runGame();
    
private:
    GameManager game;
    
    /**
     * Рисует поле на экране ncurses
     *
     * @param field Поле
     * */
    void drawField(const GameField& field) const;
    
    /**
     * Рисует остальной интерфейс вокруг поля.
     * */
    void drawInterface() const;
    
};

#endif /* view_handler_h */
