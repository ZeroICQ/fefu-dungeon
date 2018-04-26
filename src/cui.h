#pragma once

#include <ncurses.h>
#include "game.h"

namespace cui {

class Ui
{
public:
    ~Ui();
    void run();

private:
    void initialize_curses() const;
    void start_game() const;
    void update_game_frame(WINDOW* game_window, const game::Game& game) const;
};

}//namespace cui
