#pragma once

#include <vector>
#include <string>
#include <ncurses.h>

#define REAL_KEY_ENTER 10

namespace cui {

class Menu
{
public:
    //ASK: что делать?
    Menu(const std::string &title, const std::vector<std::string> &entries, int height = 10, int width = 80)
            : title_(title), entries_(entries), height_(height), width_(width),
              selected_(entries.size() > 1 ? 0 : -1) {}

    int show();

private:
    std::string title_;
    std::vector<std::string> entries_;
    int height_;
    int width_;
    int selected_;

    int center_x() const { return COLS / 2; }
    int center_y() const { return LINES / 2; }
    void select_next();
    void select_prev();
    void print_title(WINDOW* windows) const;
};


class MainMenu: public Menu
{
public:
    MainMenu() : Menu("Main Menu", {"Start game", "Exit"}) {};
private:
//    std::vector<std::string> predefined_entries = {"Start game", "Exit"};
//    std::vector<std::string> predefined_entries_;
};

}//namespace cui

