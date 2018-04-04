#pragma once

#include "game.h"

namespace cui {

class Ui
{
public:
    Ui() = default;
    ~Ui();

    void show_menu();

private:
    void initialize_curses() const;
    void start_game() const;
    void update_game_frame(WINDOW* game_window, std::vector<std::vector<game::MapCell*>>& game_level) const;
};

}//namespace cui
