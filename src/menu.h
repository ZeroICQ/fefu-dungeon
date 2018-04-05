#pragma once

#include <vector>
#include <string>
#include <ncurses.h>

#define REAL_KEY_ENTER 10

namespace cui {

class Menu
{
public:
    Menu(const std::string& title, const std::vector<std::string>& entries, int height = 10, int width = 80);
    ~Menu();

    int show();
    void clear() { wclear(menu_window_); wrefresh(menu_window_); }

private:
    std::string title_;
    std::vector<std::string> entries_;
    int height_;
    int width_;
    int selected_;
    WINDOW *menu_window_;

    int center_x() const { return COLS / 2; }
    int center_y() const { return LINES / 2; }
    void select_next();
    void select_prev();
    void print_title(WINDOW* windows) const;
};


class MainMenu: public Menu
{
public:
    //ASK: куда положпить список?
    MainMenu() : Menu("Main Menu", {"Start game", "Exit"}) {}
    enum Actions{start, exit};
};

}//namespace cui
