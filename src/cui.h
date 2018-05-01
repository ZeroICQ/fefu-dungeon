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
    void update_game_frame(WINDOW* game_window, WINDOW* status_window, const game::Game& game,
                           std::shared_ptr<game::Actor> main_char, bool is_resized) const;

    void update_status_window(WINDOW* status_window, const game::Game& game,
                              std::shared_ptr<game::Actor> main_char, bool is_resized) const;
    void update_game_window(WINDOW* game_window, const game::Game& game,
                              std::shared_ptr<game::Actor> main_char, bool is_resized) const;

    void print_param_status(WINDOW* status_window, int row, int col,
                            const std::string& name, const std::string& value) const;

    void print_progressbar(WINDOW* status_window, int row, int col, int val, int max) const;
};

}//namespace cui
