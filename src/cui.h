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
    const int STATUS_MENU_WIDTH = 30;
    void initialize_curses() const;
    void start_game() const;
    void update_game_frame(WINDOW* game_window, WINDOW* status_window, const game::Game& game, bool is_resized) const;
};

}//namespace cui
